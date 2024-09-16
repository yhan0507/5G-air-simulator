#include "radio-bearer.h"
#include "../device/NetworkNode.h"
#include "../device/UserEquipment.h"
#include "../protocolStack/packet/Packet.h"
#include "../flows/MacQueue.h"
#include "../protocolStack/rlc/rlc-entity.h"
#include "../protocolStack/rlc/am-rlc-entity.h"
#include "queue-agent.h" // 新增 QueueAgent 頭文件

RadioBearer::RadioBearer() {
  m_macQueue = new MacQueue();
  m_queueAgent = new QueueAgent(); // 創建 QueueAgent 對象
  m_application = nullptr;

  RlcEntity *rlc = new AmRlcEntity();
  rlc->SetRadioBearer(this);
  SetRlcEntity(rlc);

  m_averageTransmissionRate = 100000; // 初始值 = 1kbps
  ResetTransmittedBytes();
}

RadioBearer::~RadioBearer() {
  Destroy();
  delete m_macQueue;  // 釋放 MacQueue 內存
  delete m_queueAgent; // 釋放 QueueAgent 內存
}

void RadioBearer::Enqueue(Packet *packet) {
  // 使用 QueueAgent 進行封包入列操作
  m_queueAgent->enqueuePacket(packet);
  m_macQueue->Enqueue(packet); // 繼續使用 MacQueue 的隊列操作

  // 如果發送源是 UE，發送調度請求
  NetworkNode* src = GetSource();
  if (src->GetNodeType() == NetworkNode::TYPE_UE) {
    UserEquipment* ue = (UserEquipment*)src;
    ue->GetMacEntity()->SendSchedulingRequest();
  }
}

int RadioBearer::GetRequiredRBs() {
  return m_queueAgent->calculateRequiredRBs(); // 使用 QueueAgent 計算所需的資源塊
}

void RadioBearer::SetAvailableRBs(int rbs) {
  m_queueAgent->setAvailableRBs(rbs); // 設置可用的資源塊數
}

MacQueue* RadioBearer::GetMacQueue(void) {
  return m_macQueue;
}

void RadioBearer::SetApplication(Application* a) {
  m_application = a;
}

Application* RadioBearer::GetApplication(void) {
  return m_application;
}

void RadioBearer::UpdateTransmittedBytes(int bytes) {
  m_transmittedBytes += bytes;
}

int RadioBearer::GetTransmittedBytes(void) const {
  return m_transmittedBytes;
}

void RadioBearer::ResetTransmittedBytes(void) {
  m_transmittedBytes = 0;
  SetLastUpdate();
}

void RadioBearer::UpdateAverageTransmissionRate() {
  if (Simulator::Init()->Now() > GetLastUpdate()) {
    double rate = (GetTransmittedBytes() * 8) / (Simulator::Init()->Now() - GetLastUpdate());
    double beta = 0.2;
    m_averageTransmissionRate = ((1 - beta) * m_averageTransmissionRate) + (beta * rate);

    if (m_averageTransmissionRate < 1) {
      m_averageTransmissionRate = 1;
    }

    ResetTransmittedBytes();
  }
}

double RadioBearer::GetAverageTransmissionRate(void) const {
  return m_averageTransmissionRate;
}

void RadioBearer::SetLastUpdate(void) {
  m_lastUpdate = Simulator::Init()->Now();
}

double RadioBearer::GetLastUpdate(void) const {
  return m_lastUpdate;
}

Packet* RadioBearer::CreatePacket(int bytes) {
  Packet* p = new Packet();
  p->SetID(Simulator::Init()->GetUID());
  p->SetTimeStamp(Simulator::Init()->Now());

  UDPHeader* udp = new UDPHeader(GetClassifierParameters()->GetSourcePort(),
                                 GetClassifierParameters()->GetDestinationPort());
  p->AddUDPHeader(udp);

  IPHeader* ip = new IPHeader(GetClassifierParameters()->GetSourceID(),
                              GetClassifierParameters()->GetDestinationID());
  p->AddIPHeader(ip);

  PDCPHeader* pdcp = new PDCPHeader();
  p->AddPDCPHeader(pdcp);

  RLCHeader* rlc = new RLCHeader();
  p->AddRLCHeader(rlc);

  PacketTAGs* tags = new PacketTAGs();
  tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER);
  p->SetPacketTags(tags);

  return p;
}

int RadioBearer::GetQueueSize(void) {
  return GetMacQueue()->GetQueueSizeWithMACHoverhead();
}

double RadioBearer::GetHeadOfLinePacketDelay(void) {
  double HOL = 0.;
  double now = Simulator::Init()->Now();
  if (GetRlcEntity()->GetRlcModel() == RlcEntity::AM_RLC_MODE) {
    AmRlcEntity* amRlc = (AmRlcEntity*)GetRlcEntity();
    if (amRlc->GetSentAMDs()->size() > 0) {
      HOL = now - amRlc->GetSentAMDs()->at(0)->m_packet->GetTimeStamp();
    } else {
      HOL = now - GetMacQueue()->Peek().GetTimeStamp();
    }
  } else {
    HOL = now - GetMacQueue()->Peek().GetTimeStamp();
  }
  return HOL;
}

void RadioBearer::CheckForDropPackets(void) {
  if (m_application->GetApplicationType() != Application::APPLICATION_TYPE_TRACE_BASED &&
      m_application->GetApplicationType() != Application::APPLICATION_TYPE_VOIP) {
    return;
  }
  GetMacQueue()->CheckForDropPackets(GetQoSParameters()->GetMaxDelay(), m_application->GetApplicationID());
}

bool RadioBearer::HasPackets(void) {
  if (m_application->GetApplicationType() == Application::APPLICATION_TYPE_INFINITE_BUFFER) {
    return true;
  }
  if (GetRlcEntity()->GetRlcModel() == RlcEntity::AM_RLC_MODE) {
    AmRlcEntity* amRlc = (AmRlcEntity*)GetRlcEntity();
    return amRlc->GetSentAMDs()->size() > 0 || !GetMacQueue()->IsEmpty();
  } else {
    return !GetMacQueue()->IsEmpty();
  }
}

int RadioBearer::GetHeadOfLinePacketSize(void) {
  int size = 0;
  if (GetRlcEntity()->GetRlcModel() == RlcEntity::AM_RLC_MODE) {
    AmRlcEntity* amRlc = (AmRlcEntity*)GetRlcEntity();
    if (amRlc->GetSentAMDs()->size() > 0) {
      size = amRlc->GetSizeOfUnaknowledgedAmd();
    } else {
      size = GetMacQueue()->Peek().GetSize() - GetMacQueue()->Peek().GetFragmentOffset();
    }
  } else {
    size = GetMacQueue()->Peek().GetSize() - GetMacQueue()->Peek().GetFragmentOffset();
  }
  return size;
}

int RadioBearer::GetByte(int byte) {
  int maxData = 0;
  if (GetRlcEntity()->GetRlcModel() == RlcEntity::AM_RLC_MODE) {
    AmRlcEntity* amRlc = (AmRlcEntity*)GetRlcEntity();
    auto* am_segments = amRlc->GetSentAMDs();
    for (int i = 0; i < (int)am_segments->size(); i++) {
      maxData += am_segments->at(i)->m_packet->GetSize() + 6;
      if (maxData >= byte) continue;
    }
  }
  if (maxData < byte) {
    maxData += GetMacQueue()->GetByte(byte - maxData);
  }
  return maxData;
}
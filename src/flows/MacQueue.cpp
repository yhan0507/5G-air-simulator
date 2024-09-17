#include "MacQueue.h"
#include "../device/NetworkNode.h"
#include "../componentManagers/NetworkManager.h"
#include "../core/eventScheduler/simulator.h"
#include "../load-parameters.h"
#include <iostream>

MacQueue::MacQueue()
{
  m_maxSize = 0; // XXX NOT IMPLEMENTED
  m_queueSize = 0;
  m_nbDataPackets = 0;
  m_queue = new PacketQueue();
  m_agent = new ApplicationAgent();  // 初始化 ApplicationAgent
}

MacQueue::~MacQueue()
{
  m_queue->clear();
  delete m_queue;
  delete m_agent;  // 釋放 ApplicationAgent
}

void
MacQueue::SetMaxSize(int maxSize)
{
  m_maxSize = maxSize;
}

int
MacQueue::GetMaxSize(void) const
{
  return m_maxSize;
}

void
MacQueue::SetQueueSize(int size)
{
  m_queueSize = size;
}

void
MacQueue::UpdateQueueSize(int packetSize)
{
  m_queueSize += packetSize;
}

void
MacQueue::UpdateNbDataPackets(void)
{
  m_nbDataPackets += 1;
}

int
MacQueue::GetNbDataPackets(void) const
{
  return m_nbDataPackets;
}

int
MacQueue::GetQueueSize(void) const
{
  return m_queueSize;
}

int
MacQueue::GetQueueSizeWithMACHoverhead(void) const
{
  // we should consider the overhead due to RLC (2 bytes) and MAC (3 bytes) headers and CRC (3 bytes)
  return (GetQueueSize() + (GetNbDataPackets() * 8));
}

MacQueue::PacketQueue*
MacQueue::GetPacketQueue(void) const
{
  return m_queue;
}

bool
MacQueue::Enqueue(Packet *packet)
{
  QueueElement element(packet);
  GetPacketQueue()->push_back(element);

  UpdateQueueSize(element.GetSize());
  UpdateNbDataPackets();

  // 將封包依據應用類型傳遞給 Agent
  ApplicationAgent::ApplicationType appType = m_agent->DetermineApplicationType(packet);
  m_agent->EnqueuePacket(packet, appType);  // 傳給 agent 進行分類

  return true;
}

Packet*
MacQueue::GetPacketToTramsit(int availableBytes)
{
  DEBUG_LOG_START_1(SIM_ENV_MAC_QUEUE_DEBUG)
  std::cout << " MAC_DEBUG: Dequeue(), availableBytes = " <<
            availableBytes << std::endl;
  DEBUG_LOG_END

  int overhead = 8; // RLC + MAC + CRC overhead
  if (IsEmpty() || overhead >= availableBytes)
  {
    return nullptr;
  }

  QueueElement element = Peek();

  RLCHeader *rlcHeader = new RLCHeader();

  int dataToSend;
  int fragmentSize = 0;

  if (element.GetFragmentation())
  {
    DEBUG_LOG_START_1(SIM_ENV_MAC_QUEUE_DEBUG)
    std::cout << " MAC_DEBUG: the packet is a fragment" << std::endl;
    DEBUG_LOG_END
    dataToSend = element.GetSize() - element.GetFragmentOffset();
    rlcHeader->SetAFragment(true);
    rlcHeader->SetTheLatestFragment(true);
    rlcHeader->SetStartByte(element.GetFragmentOffset());
  }
  else
  {
    DEBUG_LOG_START_1(SIM_ENV_MAC_QUEUE_DEBUG)
    std::cout << " MAC_DEBUG: the packet is NOT a fragment" << std::endl;
    DEBUG_LOG_END
    dataToSend = element.GetSize();
    rlcHeader->SetStartByte(0);
  }

  // Packet to transmit
  Packet *packet = GetPacketQueue()->begin()->GetPacket()->Copy();

  // CASE 1 --> PACKET FRAGMENTATION
  if (dataToSend + overhead > availableBytes)
  {
    fragmentSize = availableBytes - overhead;
    packet->SetSize(fragmentSize);

    GetPacketQueue()->begin()->SetFragmentOffset(fragmentSize);
    GetPacketQueue()->begin()->SetFragmentation(true);
    GetPacketQueue()->begin()->SetFragmentNumber(element.GetFragmentNumber() + 1);

    rlcHeader->SetAFragment(true);
    rlcHeader->SetTheLatestFragment(false);
    rlcHeader->SetFragmentNumber(element.GetFragmentNumber());
    rlcHeader->SetEndByte(rlcHeader->GetStartByte() + fragmentSize - 1);

    UpdateQueueSize(-fragmentSize);
  }
  // CASE 2 -> NO other PACKET FRAGMENTATION
  else
  {
    rlcHeader->SetFragmentNumber(element.GetFragmentNumber());
    rlcHeader->SetEndByte(element.GetSize() - 1);
    Dequeue();
    UpdateQueueSize(-dataToSend);
    packet->SetSize(dataToSend);
  }

  DEBUG_LOG_START_1(SIM_ENV_MAC_QUEUE_DEBUG)
  std::cout << "\t packetSize = " << element.GetSize() <<
            "\n\t dataToSend = " << dataToSend <<
            "\n\t fragmentSize = " << fragmentSize <<
            "\n\t fragmentOffset = " << GetPacketQueue()->front().GetFragmentOffset()
            << std::endl;
  DEBUG_LOG_END

  packet->AddRLCHeader(rlcHeader);
  return packet;
}

void
MacQueue::Dequeue()
{
  GetPacketQueue()->pop_front();
  m_nbDataPackets -= 1;
}

MacQueue::QueueElement
MacQueue::Peek(void) const
{
  MacQueue::QueueElement element = *GetPacketQueue()->begin();
  return element;
}

bool
MacQueue::IsEmpty(void) const
{
  return GetPacketQueue()->empty();
}

MacQueue::QueueElement::QueueElement(void)
{
  m_packet = nullptr;
  m_fragmentation = false;
  m_fragmentNumber = 0;
  m_fragmentOffset = 0;
  m_tempFragmentNumber = 0;
  m_tempFragmentOffset = 0;
}

MacQueue::QueueElement::QueueElement(Packet *packet)
    : MacQueue::QueueElement::QueueElement()
{
  m_packet = packet;
}

MacQueue::QueueElement
MacQueue::QueueElement::operator= (const MacQueue::QueueElement &obj)
{
  m_packet = obj.m_packet->Copy();
  m_fragmentation = obj.m_fragmentation;
  m_fragmentNumber = obj.m_fragmentNumber;
  m_fragmentOffset = obj.m_fragmentOffset;
  m_tempFragmentNumber = obj.m_tempFragmentNumber;
  m_tempFragmentOffset = obj.m_tempFragmentOffset;
  return *this;
}

MacQueue::QueueElement::QueueElement(const MacQueue::QueueElement &obj)
{
  m_packet = obj.m_packet->Copy();
  m_fragmentation = obj.m_fragmentation;
  m_fragmentNumber = obj.m_fragmentNumber;
  m_fragmentOffset = obj.m_fragmentOffset;
  m_tempFragmentNumber = obj.m_tempFragmentNumber;
  m_tempFragmentOffset = obj.m_tempFragmentOffset;
}

MacQueue::QueueElement::~QueueElement()
{
  delete m_packet;
}

Packet*
MacQueue::QueueElement::GetPacket(void)
{
  return m_packet;
}

int
MacQueue::QueueElement::GetSize(void) const
{
  return m_packet->GetSize();
}

double
MacQueue::QueueElement::GetTimeStamp(void) const
{
  return m_packet->GetTimeStamp();
}

void
MacQueue::QueueElement::SetFragmentation(bool flag)
{
  m_fragmentation = flag;
}

bool
MacQueue::QueueElement::GetFragmentation(void) const
{
  return m_fragmentation;
}

void
MacQueue::QueueElement::SetFragmentNumber(int fragmentNumber)
{
  m_fragmentNumber = fragmentNumber;
}

int
MacQueue::QueueElement::GetFragmentNumber(void) const
{
  return m_fragmentNumber;
}

void
MacQueue::QueueElement::SetFragmentOffset(int fragmentOffset)
{
  m_fragmentOffset += fragmentOffset;
}

int
MacQueue::QueueElement::GetFragmentOffset(void) const
{
  return m_fragmentOffset;
}

void
MacQueue::QueueElement::SetTempFragmentNumber(int fragmentNumber)
{
  m_tempFragmentNumber = fragmentNumber;
}

int
MacQueue::QueueElement::GetTempFragmentNumber(void) const
{
  return m_tempFragmentNumber;
}

void
MacQueue::QueueElement::SetTempFragmentOffset(int fragmentOffset)
{
  m_tempFragmentOffset += fragmentOffset;
}

int
MacQueue::QueueElement::GetTempFragmentOffset(void) const
{
  return m_tempFragmentOffset;
}

void
MacQueue::ModifyPacketSourceID(int id)
{
  for (auto queueElement : *GetPacketQueue())
  {
    queueElement.GetPacket()->GetIPHeader()->SetSourceID(id);
  }
}

void
MacQueue::CheckForDropPackets(double maxDelay, int bearerID)
{
  double now = Simulator::Init()->Now();

  while (true && GetPacketQueue()->size() > 0)
  {
    double headOfLineDelay = now - GetPacketQueue()->begin()->GetTimeStamp();

    if (headOfLineDelay > maxDelay)
    {
      if (_MAC_TRACING_)
      {
        std::cout << "DROP_QUEUE";
        if (GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_VOIP)
          std::cout << " VOIP";
        else if (GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
          std::cout << " VIDEO";
        else if (GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_CBR)
          std::cout << " CBR";
        else if (GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER)
          std::cout << " INF_BUF";
        else
          std::cout << " UNKNOW";

        std::cout << " ID " << GetPacketQueue()->begin()->GetPacket()->GetID()
                  << " B " << bearerID;

        if (GetPacketQueue()->begin()->GetPacket()->GetPacketTags() != nullptr
            && GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetApplicationType() ==
            PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
        {
          std::cout << " FRAME " << GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetFrameNuber()
                    << " START " << GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetStartByte()
                    << " END " << GetPacketQueue()->begin()->GetPacket()->GetPacketTags()->GetEndByte();
        }
        std::cout << std::endl;
      }

      int size = GetPacketQueue()->begin()->GetSize() - GetPacketQueue()->begin()->GetFragmentOffset();
      UpdateQueueSize(-size);
      Dequeue();
    }
    else
    {
      break;
    }
  }
}

void
MacQueue::PrintQueueInfo(void)
{
  std::cout << "\t ** queue info: "
            "\n\t ** total size = " << GetQueueSize()
            << " packets = " << GetNbDataPackets() << std::endl;

  for (auto queueElement : *GetPacketQueue())
  {
    std::cout << "\t ** pkt --> "
              " size  " << queueElement.GetSize()
              << " offset " << queueElement.GetFragmentOffset()
              << std::endl;
  }
}

int
MacQueue::GetByte(int byte)
{
  int maxData = 0;

  for (auto queueElement : *GetPacketQueue())
  {
    maxData += queueElement.GetPacket()->GetSize() - queueElement.GetFragmentOffset() + 8;
    if (maxData >= byte) return maxData;
  }

  return maxData;
}

int
MacQueue::CalculateTotalRBs()
{
  // 假設 ApplicationAgent 提供的方法來計算資源塊
  return m_agent->CalculateTotalRBs(GetPacketQueue());
}

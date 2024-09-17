#include "ApplicationAgent.h"
#include <iostream>

// 構造函數
ApplicationAgent::ApplicationAgent()
{
  // 初始化不同應用類型的隊列映射
  m_typeToQueueMap[VOIP] = std::deque<Packet*>();
  m_typeToQueueMap[VIDEO] = std::deque<Packet*>();
  m_typeToQueueMap[CBR] = std::deque<Packet*>();
  m_typeToQueueMap[INF_BUF] = std::deque<Packet*>();
  m_typeToQueueMap[UNKNOWN] = std::deque<Packet*>();
}

// 析構函數
ApplicationAgent::~ApplicationAgent()
{
  // 清理資源
  for (auto& pair : m_typeToQueueMap)
  {
    for (Packet* packet : pair.second)
    {
      delete packet;
    }
  }
}

// 判斷應用類型
ApplicationAgent::ApplicationType
ApplicationAgent::DetermineApplicationType(Packet *packet)
{
  // 根據封包內容來確定應用類型
  // 這裡假設 PacketTAGs 提供了方法來獲取應用類型
  switch (packet->GetPacketTags()->GetApplicationType())
  {
    case PacketTAGs::APPLICATION_TYPE_VOIP:
      return VOIP;
    case PacketTAGs::APPLICATION_TYPE_TRACE_BASED:
      return VIDEO;
    case PacketTAGs::APPLICATION_TYPE_CBR:
      return CBR;
    case PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER:
      return INF_BUF;
    default:
      return UNKNOWN;
  }
}

// 根據應用類型進行分類
void
ApplicationAgent::EnqueuePacket(Packet *packet, ApplicationType type)
{
  // 將封包加入對應的隊列
  m_typeToQueueMap[type].push_back(packet);
}

// 計算總資源塊數量
int
ApplicationAgent::CalculateTotalRBs(const std::deque<MacQueue::QueueElement> *queue)
{
  int totalRBs = 0;
  // 假設每個 Packet 具有一個 GetRBs() 方法來獲取其占用的資源塊數量
  for (const auto& element : *queue)
  {
    totalRBs += element.GetPacket()->GetRBs();
  }
  return totalRBs;
}

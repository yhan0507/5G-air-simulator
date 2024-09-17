#ifndef APPLICATIONAGENT_H_
#define APPLICATIONAGENT_H_

#include "../protocolStack/packet/Packet.h"
#include <map>
#include <deque>

class ApplicationAgent
{
public:
  enum ApplicationType
  {
    VOIP,
    VIDEO,
    CBR,
    INF_BUF,
    UNKNOWN
  };

  ApplicationAgent();
  ~ApplicationAgent();

  // 判斷應用類型
  ApplicationType DetermineApplicationType(Packet *packet);

  // 根據應用類型進行分類
  void EnqueuePacket(Packet *packet, ApplicationType type);

  // 計算總資源塊數量
  int CalculateTotalRBs(const std::deque<MacQueue::QueueElement> *queue);

private:
  std::map<ApplicationType, std::deque<Packet*>> m_typeToQueueMap;  // 管理不同應用類型的隊列
};

#endif /* APPLICATIONAGENT_H_ */

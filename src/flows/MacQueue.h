#ifndef MACQUEUE_H_
#define MACQUEUE_H_

#include "../protocolStack/packet/Packet.h"
#include "ApplicationAgent.h"  // 引入 ApplicationAgent 的頭文件

#include <deque>
#include <stdint.h>

class MacQueue
{
private:
  struct QueueElement
  {
    QueueElement(void);
    QueueElement(Packet *packet);
    virtual ~QueueElement();
    QueueElement operator= (const QueueElement &obj);
    QueueElement(const QueueElement &obj);
    Packet *m_packet;
    bool m_fragmentation;
    int m_fragmentNumber;
    int m_fragmentOffset;

    // Can be used for HARQ process
    bool m_lastFragment;
    int m_tempFragmentNumber;
    int m_tempFragmentOffset;

    Packet* GetPacket(void);
    int GetSize(void) const;
    double GetTimeStamp(void) const;

    void SetFragmentation(bool flag);
    bool GetFragmentation(void) const;

    void SetFragmentNumber(int fragmentNumber);
    int GetFragmentNumber(void) const;
    void SetFragmentOffset(int fragmentOffset);
    int GetFragmentOffset(void) const;

    void SetTempFragmentNumber(int fragmentNumber);
    int GetTempFragmentNumber(void) const;
    void SetTempFragmentOffset(int fragmentOffset);
    int GetTempFragmentOffset(void) const;
  };

  typedef std::deque<QueueElement> PacketQueue;
  PacketQueue *m_queue;
  int m_maxSize;
  int m_queueSize;
  int m_nbDataPackets;
  ApplicationAgent *m_agent;  // 新增 ApplicationAgent 成員

public:
  MacQueue();
  virtual ~MacQueue();

  void SetMaxSize(int maxSize);
  int GetMaxSize(void) const;
  void SetQueueSize(int size);
  void UpdateQueueSize(int packetSize);
  int GetQueueSize(void) const;
  int GetQueueSizeWithMACHoverhead(void) const;
  void UpdateNbDataPackets(void);
  int GetNbDataPackets(void) const;

  PacketQueue* GetPacketQueue(void) const;

  bool Enqueue(Packet *packet);
  QueueElement Peek(void) const;
  bool IsEmpty(void) const;

  Packet* GetPacketToTramsit(int availableBytes);
  void Dequeue();

  void ModifyPacketSourceID(int id);

  void CheckForDropPackets(double maxDelay, int bearerID);
  int GetByte(int byte); // for FLS scheduler

  void PrintQueueInfo(void);

  // 新增方法來計算總 RB 數量
  int CalculateTotalRBs();
};

#endif /* MACQUEUE_H_ */

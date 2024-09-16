// QueueAgent.h

#ifndef QUEUE_AGENT_H
#define QUEUE_AGENT_H

#include <map>
#include "MacQueue.h"
#include "application/Application.h"
#include "Packet.h"

class QueueAgent {
public:
    enum QueueType {
        QUEUE_VOIP,
        QUEUE_VIDEO,
        QUEUE_CBR,
        QUEUE_BE  // Best Effort
    };

    QueueAgent();
    ~QueueAgent();

    void enqueuePacket(Packet* packet);
    int calculateRequiredRBs();
    void setAvailableRBs(int rbs);

private:
    std::map<QueueType, MacQueue*> m_queues;
    int m_availableRBs;

    QueueType getQueueTypeForApplication(Application::ApplicationType appType);
    int calculateRBsForQueue(MacQueue* queue);
};

#endif // QUEUE_AGENT_H
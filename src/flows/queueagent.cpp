// QueueAgent.cpp

#include "QueueAgent.h"

QueueAgent::QueueAgent() : m_availableRBs(0) {
    m_queues[QUEUE_VOIP] = new MacQueue();
    m_queues[QUEUE_VIDEO] = new MacQueue();
    m_queues[QUEUE_CBR] = new MacQueue();
    m_queues[QUEUE_BE] = new MacQueue();
}

QueueAgent::~QueueAgent() {
    for (auto& queue : m_queues) {
        delete queue.second;
    }
}

void QueueAgent::enqueuePacket(Packet* packet) {
    Application::ApplicationType appType = packet->GetPacketTags()->GetApplicationType();
    QueueType queueType = getQueueTypeForApplication(appType);
    m_queues[queueType]->Enqueue(packet);
}

QueueAgent::QueueType QueueAgent::getQueueTypeForApplication(Application::ApplicationType appType) {
    switch (appType) {
        case Application::APPLICATION_TYPE_VOIP:
            return QUEUE_VOIP;
        case Application::APPLICATION_TYPE_TRACE_BASED:
            return QUEUE_VIDEO;
        case Application::APPLICATION_TYPE_CBR:
            return QUEUE_CBR;
        default:
            return QUEUE_BE;
    }
}

int QueueAgent::calculateRequiredRBs() {
    int totalRequiredRBs = 0;
    for (const auto& queue : m_queues) {
        totalRequiredRBs += calculateRBsForQueue(queue.second);
    }
    return totalRequiredRBs;
}

int QueueAgent::calculateRBsForQueue(MacQueue* queue) {
    // 這裡的實現取決於您的具體需求和系統參數
    // 這只是一個簡單的示例
    int queueSizeInBytes = queue->GetQueueSize();
    int bytesPerRB = 100; // 假設每個RB可以傳輸100字節
    return (queueSizeInBytes + bytesPerRB - 1) / bytesPerRB; // 向上取整
}

void QueueAgent::setAvailableRBs(int rbs) {
    m_availableRBs = rbs;
}
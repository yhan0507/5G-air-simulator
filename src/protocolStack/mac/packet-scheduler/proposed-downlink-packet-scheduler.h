#ifndef PROPOSED_DOWNLINK_PACKET_SCHEDULER_H_
#define PROPOSED_DOWNLINK_PACKET_SCHEDULER_H_

#include "downlink-packet-scheduler.h"

class PROPOSEDDownlinkPacketScheduler : public DownlinkPacketScheduler
{
public:
	PROPOSEDDownlinkPacketScheduler();
	virtual ~PROPOSEDDownlinkPacketScheduler();

	virtual void DoSchedule(void);
	virtual void DoStopSchedule(void);

	virtual void RBsAllocation();
	virtual double ComputeSchedulingMetric(RadioBearer *bearer, double spectralEfficiency, int subChannel);
	void Checkfairness();

private:
	float metric;
	std::map<int, int> TransmittedThroughputs;
	double fairness_index = 0.0;
};

#endif
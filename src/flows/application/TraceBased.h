/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 TELEMATICS LAB, Politecnico di Bari
 *
 * This file is part of 5G-air-simulator
 *
 * 5G-air-simulator is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation;
 *
 * 5G-air-simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 5G-air-simulator; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Telematics Lab <telematics-dev@poliba.it>
 */

#ifndef TRACEBASED_H_
#define TRACEBASED_H_

#include "Application.h"

#include <cstdio>
#include <iostream>
#include <fstream>

/*
 * This application sends udp packets based on a trace file could be downloaded form :
 * http://www.tkn.tu-berlin.de/research/trace/ltvt.html
 * A valid trace file is a file with 4 columns:
 * - the first one represents the frame index
 * - the second one indicates the type of the frame: I, P or B
 * - the third one indicates the time on which the frame was generated by the encoder
 * - the fourth one indicates the frame size in byte
 * if no valid trace trace file is provided to the application the trace from
 * default-trace.h will be loaded.
 */

class TraceBased : public Application
{
public:
  TraceBased();
  virtual ~TraceBased();

  typedef struct
  {
    uint32_t TimeToSend;
    uint32_t PacketSize;
    uint32_t FrameIndex;
    char FrameType;
  } TraceEntry;

  void SetTraceFile(string traceFile);
  void LoadTrace(string traceFile);
  void LoadInternalTrace(vector<TraceEntry> *trace);
  void LoadDefaultTrace(void);

  virtual void DoStart(void);
  virtual void DoStop(void);

  void ScheduleTransmit(double time);
  void Send(void);

  void UpdateFrameCounter(void);
  int GetFrameCounter(void);

  void SetBurstSize(int bytes);
  int GetBurstSize(void);

  void PrintTrace(void);

private:
  uint32_t m_TraceSize; //in packet
  double m_interval;
  uint32_t m_size;
  uint32_t m_sent;

  vector<TraceEntry> *m_entries;
  bool m_is_internal_trace;
  vector<TraceEntry>::iterator iter;

  int m_frameCounter;
  int m_burstSize;
};

#include "Trace/foreman_H264_128k.h"
#include "Trace/foreman_H264_242k.h"
#include "Trace/foreman_H264_440k.h"
#include "Trace/mobile_H264_128k.h"
#include "Trace/highway_H264_128k.h"
#include "Trace/trailer_H264_17000k.h"
#include "Trace/trailer_H264_8000k.h"
#include "Trace/trailer_H264_7000k.h"

#include "Trace/trailer_H264_1000k.h"
#include "Trace/trailer_H264_3000k.h"
#include "Trace/trailer_H264_5000k.h"
#endif /* TRACEBASED_H_ */

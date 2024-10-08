/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */

#ifndef UDP_TRACE_CLIENT_H
#define UDP_TRACE_CLIENT_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include <vector>

namespace ns3 {

class Socket;
class Packet;

/**
 * \ingroup udpclientserver
 * \class UdpTraceClient
 * \brief A trace based streamer
 *
 * sends udp packets based on a trace file of an MPEG4 stream
 * trace files could be downloaded form :
 * http://www.tkn.tu-berlin.de/research/trace/ltvt.html (the 2 first lines of
 * the file should be removed)
 * A valid trace file is a file with 4 columns:
 * -1- the first one represents the frame index
 * -2- the second one indicates the type of the frame: I, P or B
 * -3- the third one indicates the time on which the frame was generated by the encoder
 * -4- the fourth one indicates the frame size in byte
 * if no valid MPEG4 trace file is provided to the application the trace from
 * g_defaultEntries array will be loaded.
 */
class UdpTraceClient : public Application
{
public:
  static TypeId
  GetTypeId (void);
  /**
   * \brief creates a traceBasedStreamer application
   */
  UdpTraceClient ();

  /**
   * \brief creates a traceBasedStreamer application
   * \param ip the destination ip address to which the stream will be sent
   * \param port the destination udp port to which the stream will be sent
   * \param traceFile a path to an MPEG4 trace file formatted as follows:
   *  FrameNo Frametype   Time[ms]    Length [byte]
   *  FrameNo Frametype   Time[ms]    Length [byte]
   *  ...
   *
   *
   */

  UdpTraceClient (Ipv4Address ip, uint16_t port, char *traceFile);
  ~UdpTraceClient ();

  /**
   * \brief set the destination IP address and port
   * \param ip the destination ip address to which the stream will be sent
   * \param port the destination udp port to which the stream will be sent
   */
  void SetRemote (Address ip, uint16_t port);
  void SetRemote (Ipv4Address ip, uint16_t port);
  void SetRemote (Ipv6Address ip, uint16_t port);

  /**
   * \brief set the trace file to be used by the application
   * \param filename a path to an MPEG4 trace file formatted as follows:
   *  Frame No Frametype   Time[ms]    Length [byte]
   *  Frame No Frametype   Time[ms]    Length [byte]
   *  ...
   */
  void SetTraceFile (std::string filename);

  /**
   * \return the maximum packet size
   */
  uint16_t GetMaxPacketSize (void);

  /**
   * \param maxPacketSize The maximum packet size
   */
  void SetMaxPacketSize (uint16_t maxPacketSize);

protected:
  virtual void DoDispose (void);

private:
  void LoadTrace (std::string filename);
  void LoadDefaultTrace (void);
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  void ScheduleTransmit (Time dt);
  void Send (void);
  void SendPacket (uint32_t size);


  struct TraceEntry
  {
    uint32_t timeToSend;
    uint16_t packetSize;
    char frameType;
  };
  uint32_t m_sent;
  Ptr<Socket> m_socket;
  Address m_peerAddress;
  uint16_t m_peerPort;
  EventId m_sendEvent;
  std::vector<struct TraceEntry> m_entries;
  uint32_t m_currentEntry;
  static struct TraceEntry g_defaultEntries[];
  uint16_t m_maxPacketSize;
};

} // namespace ns3

#endif /* UDP_TRACE_CLIENT_H */

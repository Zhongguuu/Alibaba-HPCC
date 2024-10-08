/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ipv4-end-point.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4EndPoint");

namespace ns3 {

Ipv4EndPoint::Ipv4EndPoint (Ipv4Address address, uint16_t port)
  : m_localAddr (address), 
    m_localPort (port),
    m_peerAddr (Ipv4Address::GetAny ()),
    m_peerPort (0)
{
}
Ipv4EndPoint::~Ipv4EndPoint ()
{
  if (!m_destroyCallback.IsNull ())
    {
      m_destroyCallback ();
    }
  m_rxCallback.Nullify ();
  m_icmpCallback.Nullify ();
  m_destroyCallback.Nullify ();
}

Ipv4Address 
Ipv4EndPoint::GetLocalAddress (void)
{
  return m_localAddr;
}

void 
Ipv4EndPoint::SetLocalAddress (Ipv4Address address)
{
  m_localAddr = address;
}

uint16_t 
Ipv4EndPoint::GetLocalPort (void)
{
  return m_localPort;
}
Ipv4Address 
Ipv4EndPoint::GetPeerAddress (void)
{
  return m_peerAddr;
}
uint16_t 
Ipv4EndPoint::GetPeerPort (void)
{
  return m_peerPort;
}
void 
Ipv4EndPoint::SetPeer (Ipv4Address address, uint16_t port)
{
  m_peerAddr = address;
  m_peerPort = port;
}

void
Ipv4EndPoint::BindToNetDevice (Ptr<NetDevice> netdevice)
{
  m_boundnetdevice = netdevice;
  return;
}

Ptr<NetDevice> 
Ipv4EndPoint::GetBoundNetDevice (void)
{
  return m_boundnetdevice;
}

void 
Ipv4EndPoint::SetRxCallback (Callback<void,Ptr<Packet>, Ipv4Header, uint16_t, Ptr<Ipv4Interface> > callback)
{
  m_rxCallback = callback;
}
void 
Ipv4EndPoint::SetIcmpCallback (Callback<void,Ipv4Address,uint8_t,uint8_t,uint8_t,uint32_t> callback)
{
  m_icmpCallback = callback;
}

void 
Ipv4EndPoint::SetDestroyCallback (Callback<void> callback)
{
  m_destroyCallback = callback;
}

void 
Ipv4EndPoint::ForwardUp (Ptr<Packet> p, const Ipv4Header& header, uint16_t sport,
                         Ptr<Ipv4Interface> incomingInterface)
{
  if (!m_rxCallback.IsNull ())
    {
      Simulator::ScheduleNow (&Ipv4EndPoint::DoForwardUp, this, p, header, sport, 
                              incomingInterface);
    }
}
void 
Ipv4EndPoint::DoForwardUp (Ptr<Packet> p, const Ipv4Header& header, uint16_t sport,
                           Ptr<Ipv4Interface> incomingInterface)
{
  if (!m_rxCallback.IsNull ())
    {
      m_rxCallback (p, header, sport, incomingInterface);
    }
}

void 
Ipv4EndPoint::ForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, 
                           uint8_t icmpType, uint8_t icmpCode,
                           uint32_t icmpInfo)
{
  NS_LOG_FUNCTION (this << icmpSource << (uint32_t)icmpTtl << (uint32_t)icmpType <<
                   (uint32_t)icmpCode << icmpInfo);
  if (!m_icmpCallback.IsNull ())
    {
      Simulator::ScheduleNow (&Ipv4EndPoint::DoForwardIcmp, this, 
                              icmpSource, icmpTtl, icmpType, icmpCode, icmpInfo);
    }
}
void 
Ipv4EndPoint::DoForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, 
                             uint8_t icmpType, uint8_t icmpCode,
                             uint32_t icmpInfo)
{
  if (!m_icmpCallback.IsNull ())
    {
      m_icmpCallback (icmpSource,icmpTtl,icmpType,icmpCode,icmpInfo);
    }
}

} // namespace ns3

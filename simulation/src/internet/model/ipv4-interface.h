/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Authors: 
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>,
 *  Tom Henderson <tomh@tomh.org>
 */
#ifndef IPV4_INTERFACE_H
#define IPV4_INTERFACE_H

#include <list>
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-interface-address.h"
#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3 {

class NetDevice;
class Packet;
class Node;
class ArpCache;

/**
 * \brief The IPv4 representation of a network interface
 *
 * This class roughly corresponds to the struct in_device
 * of Linux; the main purpose is to provide address-family
 * specific information (addresses) about an interface.
 *
 * By default, Ipv4 interface are created in the "down" state
 * no IP addresses.  Before becoming useable, the user must 
 * add an address of some type and invoke Setup on them.
 */
class Ipv4Interface  : public Object
{
public:
  static TypeId GetTypeId (void);

  Ipv4Interface ();
  virtual ~Ipv4Interface();

  void SetNode (Ptr<Node> node); 
  void SetDevice (Ptr<NetDevice> device);
  void SetArpCache (Ptr<ArpCache>);

  /**
   * \returns the underlying NetDevice. This method cannot return zero.
   */
  Ptr<NetDevice> GetDevice (void) const;
  /**
   * \return ARP cache used by this interface
   */
  Ptr<ArpCache> GetArpCache () const;

  /**
   * \param metric configured routing metric (cost) of this interface
   *
   * Note:  This is synonymous to the Metric value that ifconfig prints
   * out.  It is used by ns-3 global routing, but other routing daemons
   * choose to ignore it. 
   */
  void SetMetric (uint16_t metric);

  /**
   * \returns configured routing metric (cost) of this interface
   *
   * Note:  This is synonymous to the Metric value that ifconfig prints
   * out.  It is used by ns-3 global routing, but other routing daemons 
   * may choose to ignore it. 
   */
  uint16_t GetMetric (void) const;

  /**
   * These are IP interface states and may be distinct from 
   * NetDevice states, such as found in real implementations
   * (where the device may be down but IP interface state is still up).
   */
  /**
   * \returns true if this interface is enabled, false otherwise.
   */
  bool IsUp (void) const;

  /**
   * \returns true if this interface is disabled, false otherwise.
   */
  bool IsDown (void) const;

  /**
   * Enable this interface
   */
  void SetUp (void);

  /**
   * Disable this interface
   */
  void SetDown (void);

  /**
   * \returns true if this interface is enabled for IP forwarding of input datagrams
   */
  bool IsForwarding (void) const;

  /**
   * \param val Whether to enable or disable IP forwarding for input datagrams
   */
  void SetForwarding (bool val);

  /**
   * \param p packet to send
   * \param dest next hop address of packet.
   *
   * This method will eventually call the private
   * SendTo method which must be implemented by subclasses.
   */ 
  void Send (Ptr<Packet> p, Ipv4Address dest);

  /**
   * \param address The Ipv4InterfaceAddress to add to the interface
   * \returns true if succeeded
   */
  bool AddAddress (Ipv4InterfaceAddress address);

  /**
   * \param index Index of Ipv4InterfaceAddress to return
   * \returns The Ipv4InterfaceAddress address whose index is i
   */
  Ipv4InterfaceAddress GetAddress (uint32_t index) const;

  /**
   * \returns the number of Ipv4InterfaceAddresss stored on this interface
   */
  uint32_t GetNAddresses (void) const;

  /**
   * \param index Index of Ipv4InterfaceAddress to remove
   * \returns The Ipv4InterfaceAddress address whose index is index 
   */
  Ipv4InterfaceAddress RemoveAddress (uint32_t index);

protected:
  virtual void DoDispose (void);
private:
  void DoSetup (void);
  typedef std::list<Ipv4InterfaceAddress> Ipv4InterfaceAddressList;
  typedef std::list<Ipv4InterfaceAddress>::const_iterator Ipv4InterfaceAddressListCI;
  typedef std::list<Ipv4InterfaceAddress>::iterator Ipv4InterfaceAddressListI;

  bool m_ifup;
  bool m_forwarding;  // IN_DEV_FORWARD
  uint16_t m_metric;
  Ipv4InterfaceAddressList m_ifaddrs;
  Ptr<Node> m_node;
  Ptr<NetDevice> m_device;
  Ptr<ArpCache> m_cache; 
};

} // namespace ns3

#endif

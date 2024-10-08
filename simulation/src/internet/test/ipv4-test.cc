/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Faker Moatamri <faker.moatamri@sophia.inria.fr>
 *
 */
/**
 * This is the test code for ipv4-l3-protocol.cc
 */

#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"

#include "ns3/ipv4-l3-protocol.h"
#include "ns3/arp-l3-protocol.h"
#include "ns3/ipv4-interface.h"
#include "ns3/loopback-net-device.h"

namespace ns3 {

class Ipv4L3ProtocolTestCase : public TestCase
{
public:
  /**
   * \brief Constructor.
   */
  Ipv4L3ProtocolTestCase ();
  /**
   * \brief Destructor.
   */
  virtual
  ~Ipv4L3ProtocolTestCase ();
  /**
   * \brief Run unit tests for this class.
   * \return false if all tests have passed, false otherwise
   */
  virtual void
  DoRun (void);

};

Ipv4L3ProtocolTestCase::Ipv4L3ProtocolTestCase () :
  TestCase ("Verify the IPv4 layer 3 protocol")
{
}

Ipv4L3ProtocolTestCase::~Ipv4L3ProtocolTestCase ()
{
}
void
Ipv4L3ProtocolTestCase::DoRun (void)
{
  Ptr<Node> node = CreateObject<Node> ();
  Ptr<Ipv4L3Protocol> ipv4 = CreateObject<Ipv4L3Protocol> ();
  Ptr<Ipv4Interface> interface = CreateObject<Ipv4Interface> ();
  Ptr<LoopbackNetDevice> device = CreateObject<LoopbackNetDevice> ();
  node->AddDevice (device);
  interface->SetDevice (device);
  interface->SetNode (node);
  uint32_t index = ipv4->AddIpv4Interface (interface);
  NS_TEST_ASSERT_MSG_EQ (index, 0, "No interface should be found??");
  interface->SetUp ();
  Ipv4InterfaceAddress ifaceAddr1 = Ipv4InterfaceAddress ("192.168.0.1",
                                                          "255.255.255.0");
  interface->AddAddress (ifaceAddr1);
  Ipv4InterfaceAddress ifaceAddr2 = Ipv4InterfaceAddress ("192.168.0.2",
                                                          "255.255.255.0");
  interface->AddAddress (ifaceAddr2);
  Ipv4InterfaceAddress ifaceAddr3 = Ipv4InterfaceAddress ("10.30.0.1",
                                                          "255.255.255.0");
  interface->AddAddress (ifaceAddr3);
  Ipv4InterfaceAddress ifaceAddr4 = Ipv4InterfaceAddress ("250.0.0.1",
                                                          "255.255.255.0");
  interface->AddAddress (ifaceAddr4);
  uint32_t num = interface->GetNAddresses ();
  NS_TEST_ASSERT_MSG_EQ (num, 4, "Should find 4 interfaces??");
  interface->RemoveAddress (2);
  num = interface->GetNAddresses ();
  NS_TEST_ASSERT_MSG_EQ (num, 3, "Should find 3 interfaces??");
  Ipv4InterfaceAddress output = interface->GetAddress (2);
  NS_TEST_ASSERT_MSG_EQ (ifaceAddr4, output,
                         "The addresses should be identical");
  Simulator::Destroy ();
}

static class IPv4L3ProtocolTestSuite : public TestSuite
{
public:
  IPv4L3ProtocolTestSuite () :
    TestSuite ("ipv4-protocol", UNIT)
  {
    AddTestCase (new Ipv4L3ProtocolTestCase ());
  }
} g_ipv4protocolTestSuite;

} // namespace ns3

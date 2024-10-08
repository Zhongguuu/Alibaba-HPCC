/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */



#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"

#include "ns3/epc-tft-classifier.h"

#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("TestEpcTftClassifier");

namespace ns3 {

class EpcTftClassifierTestCase : public TestCase
{
public:
  EpcTftClassifierTestCase (Ptr<EpcTftClassifier> c,
                            EpcTft::Direction d,
                            Ipv4Address sa, 
                            Ipv4Address da, 
                            uint16_t sp,
                            uint16_t dp,
                            uint8_t tos,
                            uint32_t tftId);
  virtual ~EpcTftClassifierTestCase ();

private:
  
  Ptr<EpcTftClassifier> m_c;
  EpcTft::Direction m_d;
  uint8_t m_tftId;
  Ipv4Header m_ipHeader;
  UdpHeader m_udpHeader;
  TcpHeader m_tcpHeader;

  static std::string BuildNameString (Ptr<EpcTftClassifier> c,
                                      EpcTft::Direction d,
                                      Ipv4Address sa, 
                                      Ipv4Address da, 
                                      uint16_t sp,
                                      uint16_t dp,
                                      uint8_t tos,
                                      uint32_t tftId);
  virtual void DoRun (void);
};

EpcTftClassifierTestCase::EpcTftClassifierTestCase (Ptr<EpcTftClassifier> c,
                                                    EpcTft::Direction d,
                                                    Ipv4Address sa, 
                                                    Ipv4Address da, 
                                                    uint16_t sp,
                                                    uint16_t dp,
                                                    uint8_t tos,
                                                    uint32_t tftId)
  : TestCase (BuildNameString (c, d, sa, da, sp, dp, tos, tftId)),
    m_c (c),
    m_d (d),    
    m_tftId (tftId)
{
  NS_LOG_FUNCTION (this);

  m_ipHeader.SetSource (sa);
  m_ipHeader.SetDestination (da);
  m_ipHeader.SetTos (tos);  

  m_udpHeader.SetSourcePort (sp);
  m_udpHeader.SetDestinationPort (dp);  
}

EpcTftClassifierTestCase::~EpcTftClassifierTestCase ()
{
}

std::string
EpcTftClassifierTestCase::BuildNameString (Ptr<EpcTftClassifier> c,
                                           EpcTft::Direction d,
                                           Ipv4Address sa, 
                                           Ipv4Address da, 
                                           uint16_t sp,
                                           uint16_t dp,
                                           uint8_t tos,
                                           uint32_t tftId)
{
  std::ostringstream oss;
  oss << c
      << "  d = " << d
      << ", sa = " << sa
      << ", da = " << da
      << ", sp = " << sp
      << ", dp = " << dp
      << ", tos = 0x" << std::hex << (int) tos
      << " --> tftId = " << tftId;  
  return oss.str ();
}

void 
EpcTftClassifierTestCase::DoRun (void)
{
  ns3::PacketMetadata::Enable ();

  Ptr<Packet> udpPacket = Create<Packet> ();
  m_ipHeader.SetProtocol (UdpL4Protocol::PROT_NUMBER);
  udpPacket->AddHeader (m_udpHeader);
  udpPacket->AddHeader (m_ipHeader);
  NS_LOG_LOGIC (this << *udpPacket);
  uint32_t obtainedTftId = m_c ->Classify (udpPacket, m_d);
  NS_TEST_ASSERT_MSG_EQ (obtainedTftId, m_tftId, "bad classification of UDP packet");
}




class EpcTftClassifierTestSuite : public TestSuite
{
public:
  EpcTftClassifierTestSuite ();
};

static EpcTftClassifierTestSuite g_lteTftClassifierTestSuite;

EpcTftClassifierTestSuite::EpcTftClassifierTestSuite ()
  : TestSuite ("eps-tft-classifier", UNIT)
{
  NS_LOG_FUNCTION (this);


  ///////////////////////////
  // check some TFT matches
  ///////////////////////////

  
  Ptr<EpcTftClassifier> c1 = Create<EpcTftClassifier> ();
  

  Ptr<EpcTft> tft1_1 = Create<EpcTft> ();

  EpcTft::PacketFilter pf1_1_1;
  pf1_1_1.remoteAddress.Set ("1.0.0.0");
  pf1_1_1.localAddress.Set ("2.0.0.0");
  pf1_1_1.remoteMask.Set (0xFF000000);
  pf1_1_1.localMask.Set (0xFF000000);
  tft1_1->Add (pf1_1_1);

  EpcTft::PacketFilter pf1_1_2;
  pf1_1_2.remoteAddress.Set ("3.3.3.0");
  pf1_1_2.localAddress.Set ("4.4.4.0");
  pf1_1_2.remoteMask.Set (0xFFFFFF00);
  pf1_1_2.localMask.Set (0xFFFFFF00);
  tft1_1->Add (pf1_1_2);

  c1->Add (tft1_1, 1);



  Ptr<EpcTft> tft1_2 = Create<EpcTft> ();

  EpcTft::PacketFilter pf1_2_1;
  pf1_2_1.remotePortStart = 1024;
  pf1_2_1.remotePortEnd   = 1035;
  tft1_2->Add (pf1_2_1);

  EpcTft::PacketFilter pf1_2_2;
  pf1_2_2.localPortStart = 3456;
  pf1_2_2.localPortEnd   = 3489;
  tft1_2->Add (pf1_2_2);


  EpcTft::PacketFilter pf1_2_3;
  pf1_2_3.localPortStart = 7895;
  pf1_2_3.localPortEnd   = 7895;
  tft1_2->Add (pf1_2_3);


  EpcTft::PacketFilter pf1_2_4;
  pf1_2_4.remotePortStart = 5897;
  pf1_2_4.remotePortEnd   = 5897;
  tft1_2->Add (pf1_2_4);


  c1->Add (tft1_2, 2);

  // ------------------------------------classifier---direction--------------src address---------------dst address---src port--dst port--ToS--TFT id

  // test IP addresses
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("2.2.3.4"), Ipv4Address ("1.1.1.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("2.2.3.4"), Ipv4Address ("1.0.0.0"),     2,      123,     5,    1));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("6.2.3.4"), Ipv4Address ("1.1.1.1"),     4,     1234,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("3.3.3.4"), Ipv4Address ("4.4.4.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("3.3.4.4"), Ipv4Address ("4.4.4.1"),     4,     1234,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("3.3.3.4"), Ipv4Address ("4.4.2.1"),     4,     1234,     0,    0));

  // test remote port
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1024,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1025,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1035,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1234,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1024,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1025,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1035,     0,    0));

  // test local port
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3456,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3457,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3489,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  3456,        6,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  3461,     3461,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     3489,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     7895,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  7895,       10,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     5897,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c1, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  5897,       10,     0,    2));


  ///////////////////////////
  // check default TFT 
  ///////////////////////////

  Ptr<EpcTftClassifier> c2 = Create<EpcTftClassifier> ();
  c2->Add (EpcTft::Default (), 1);

  // ------------------------------------classifier---direction--------------src address---------------dst address---src port--dst port--ToS--TFT id

  // test IP addresses
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("2.2.3.4"), Ipv4Address ("1.1.1.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("2.2.3.4"), Ipv4Address ("1.0.0.0"),     2,      123,     5,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("6.2.3.4"), Ipv4Address ("1.1.1.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::DOWNLINK, Ipv4Address ("3.3.3.4"), Ipv4Address ("4.4.4.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::DOWNLINK, Ipv4Address ("3.3.4.4"), Ipv4Address ("4.4.4.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("3.3.3.4"), Ipv4Address ("4.4.2.1"),     4,     1234,     0,    1));

  // test remote port
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1024,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1025,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1035,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1024,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1025,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1035,     0,    1));

  // test local port
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3456,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3457,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3489,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  3456,        6,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  3461,     3461,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c2, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     3489,     0,    1));

  

  ///////////////////////////////////////////
  // check default TFT plus dedicated ones
  ///////////////////////////////////////////

  Ptr<EpcTftClassifier> c3 = Create<EpcTftClassifier> ();
  c3->Add (EpcTft::Default (), 1);
  c3->Add (tft1_1, 2);
  c3->Add (tft1_2, 3);

  // ------------------------------------classifier---direction--------------src address---------------dst address---src port--dst port--ToS--TFT id

  // test IP addresses
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("2.2.3.4"), Ipv4Address ("1.1.1.1"),     4,     1234,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("2.2.3.4"), Ipv4Address ("1.0.0.0"),     2,      123,     5,    2));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("6.2.3.4"), Ipv4Address ("1.1.1.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::DOWNLINK, Ipv4Address ("3.3.3.4"), Ipv4Address ("4.4.4.1"),     4,     1234,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::DOWNLINK, Ipv4Address ("3.3.4.4"), Ipv4Address ("4.4.4.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("3.3.3.4"), Ipv4Address ("4.4.2.1"),     4,     1234,     0,    1));

  // test remote port
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1024,     0,    3));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1025,     0,    3));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1035,     0,    3));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1234,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1024,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1025,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     1035,     0,    1));

  // test local port
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3456,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3457,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     4,     3489,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  3456,        6,     0,    3));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  3461,     3461,     0,    3));
  AddTestCase (new EpcTftClassifierTestCase (c3, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     3489,     0,    3));



  ///////////////////////////////////////////
  // check two TFTs with different ports
  ///////////////////////////////////////////

  Ptr<EpcTftClassifier> c4 = Create<EpcTftClassifier> ();
  Ptr<EpcTft> tft4_1 = Create<EpcTft> ();
  tft4_1->Add (pf1_2_3);  
  c4->Add (tft4_1, 1);
  Ptr<EpcTft> tft4_2 = Create<EpcTft> ();
  tft4_2->Add (pf1_2_4);
  c4->Add (tft4_2, 2);
  AddTestCase (new EpcTftClassifierTestCase (c4, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     3489,     0,    0));
  AddTestCase (new EpcTftClassifierTestCase (c4, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     7895,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c4, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  7895,       10,     0,    1));
  AddTestCase (new EpcTftClassifierTestCase (c4, EpcTft::UPLINK,   Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),     9,     5897,     0,    2));
  AddTestCase (new EpcTftClassifierTestCase (c4, EpcTft::DOWNLINK, Ipv4Address ("9.1.1.1"), Ipv4Address ("8.1.1.1"),  5897,       10,     0,    2));

}


} // namespace ns3

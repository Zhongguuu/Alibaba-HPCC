/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#ifndef MESH_CONFIGURATION_H
#define MESH_CONFIGURATION_H

#include "ns3/mesh-information-element-vector.h"

namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 * \brief See 7.3.2.86.1 in 802.11s draft 3.0
 */
enum dot11sPathSelectionProtocol
{
  PROTOCOL_HWMP = 0x000fac00,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.86.2 in 802.11s draft 3.0
 */
enum dot11sPathSelectionMetric
{
  METRIC_AIRTIME = 0x000fac00,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.86.3 in 802.11s draft 3.0
 */
enum dot11sCongestionControlMode
{
  CONGESTION_SIGNALING = 0x000fac00,
  CONGESTION_NULL      = 0x000facff,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.86.4 in 802.11s draft 3.0
 */
enum dot11sSynchronizationProtocolIdentifier
{
  SYNC_NEIGHBOUR_OFFSET = 0x000fac00,
  SYNC_NULL             = 0x000facff,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.86.5 in 802.11s draft 3.0
 */
enum dot11sAuthenticationProtocol
{
  AUTH_NULL = 0x000fac00,
  AUTH_SAE  = 0x000fac01,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.86.7 in 802.11s draft 3.0
 */
class Dot11sMeshCapability
{
public:
  Dot11sMeshCapability ();
  uint8_t  GetSerializedSize () const;
  Buffer::Iterator Serialize (Buffer::Iterator i) const;
  Buffer::Iterator Deserialize (Buffer::Iterator i);
  uint16_t GetUint16 () const;
  bool acceptPeerLinks;
  bool MCCASupported;
  bool MCCAEnabled;
  bool forwarding;
  bool beaconTimingReport;
  bool TBTTAdjustment;
  bool powerSaveLevel;
  bool Is (uint16_t cap,uint8_t n) const;
  friend bool operator== (const Dot11sMeshCapability & a, const Dot11sMeshCapability & b);
};

/**
 * \ingroup dot11s
 * \brief Describes Mesh Configuration Element 
 * see 7.3.2.86 of 802.11s draft 3.0
 */
class IeConfiguration : public WifiInformationElement
{
public:
  IeConfiguration ();
  void SetRouting (dot11sPathSelectionProtocol routingId);
  void SetMetric (dot11sPathSelectionMetric metricId);
  bool IsHWMP ();
  bool IsAirtime ();
  void SetNeighborCount (uint8_t neighbors);
  uint8_t GetNeighborCount ();
  Dot11sMeshCapability const& MeshCapability ();
  /**
   * \name Inherited from WifiInformationElement
   * \{
   */
  virtual WifiInformationElementId ElementId () const;
  virtual uint8_t GetInformationFieldSize () const;
  virtual void SerializeInformationField (Buffer::Iterator i) const;
  virtual uint8_t DeserializeInformationField (Buffer::Iterator i, uint8_t length);
  virtual void Print (std::ostream& os) const;
  ///\}
private:
  /** Active Path Selection Protocol ID */
  dot11sPathSelectionProtocol m_APSPId;
  /** Active Path Metric ID */
  dot11sPathSelectionMetric   m_APSMId;
  /** Congestion Control Mode ID */
  dot11sCongestionControlMode m_CCMId;
  /** Sync protocol ID */
  dot11sSynchronizationProtocolIdentifier m_SPId;
  /** Auth protocol ID */
  dot11sAuthenticationProtocol m_APId;
  Dot11sMeshCapability m_meshCap;
  uint8_t m_neighbors;
  friend bool operator== (const IeConfiguration & a, const IeConfiguration & b);
};
bool operator== (const IeConfiguration & a, const IeConfiguration & b);
bool operator== (const Dot11sMeshCapability & a, const Dot11sMeshCapability & b);
std::ostream &operator << (std::ostream &os, const IeConfiguration &config);
} // namespace dot11s
} // namespace ns3
#endif

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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef PERR_INFORMATION_ELEMENT_H
#define PERR_INFORMATION_ELEMENT_H

#include "ns3/mac48-address.h"
#include "ns3/mesh-information-element-vector.h"
#include "ns3/hwmp-protocol.h"

namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 * \brief See 7.3.2.98 of 802.11s draft 2.07
 */
class IePerr : public WifiInformationElement
{
public:
  IePerr ();
  ~IePerr ();
  uint8_t   GetNumOfDest () const;
  void AddAddressUnit (HwmpProtocol::FailedDestination unit);
  bool IsFull () const;
  std::vector<HwmpProtocol::FailedDestination> GetAddressUnitVector () const;
  void DeleteAddressUnit (Mac48Address address);
  void ResetPerr ();
  /**
   * \name Inherited from WifiInformationElement
   * \{
   */
  virtual WifiInformationElementId ElementId () const;
  virtual void SerializeInformationField (Buffer::Iterator i) const;
  virtual uint8_t DeserializeInformationField (Buffer::Iterator start, uint8_t length);
  virtual void Print (std::ostream& os) const;
  virtual uint8_t GetInformationFieldSize () const;
  ///\}
private:
  std::vector<HwmpProtocol::FailedDestination> m_addressUnits;
  friend bool operator== (const IePerr & a, const IePerr & b);
};
bool operator== (const IePerr & a, const IePerr & b);
std::ostream &operator << (std::ostream &os, const IePerr &perr);
} // namespace dot11s
} // namespace ns3
#endif

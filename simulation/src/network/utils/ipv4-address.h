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

#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <stdint.h>
#include <ostream>
#include "ns3/address.h"
#include "ns3/attribute-helper.h"

#ifdef WIN32
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <hash_map>
#endif

namespace ns3 {

class Ipv4Mask;

/** 
 * \ingroup address
 *
 * \brief Ipv4 addresses are stored in host order in this class.
 */
class Ipv4Address {
public:
  Ipv4Address ();
  /**
   * input address is in host order.
   * \param address The host order 32-bit address
   */
  explicit Ipv4Address (uint32_t address);
  /** 
    * \brief Constructs an Ipv4Address by parsing a the input C-string
    *
    * Input address is in format:
    * hhh.xxx.xxx.lll
    * where h is the high byte and l the
    * low byte
    * \param address C-string containing the address as described above
    */
  Ipv4Address (char const *address);
  /** 
   * Get the host-order 32-bit IP address
   * \return the host-order 32-bit IP address
   */
  uint32_t Get (void) const;
  /**
   * input address is in host order.
   * \param address The host order 32-bit address
   */
  void Set (uint32_t address);
  /** 
    * \brief Sets an Ipv4Address by parsing a the input C-string
    *
    * Input address is in format:
    * hhh.xxx.xxx.lll
    * where h is the high byte and l the
    * low byte
    * \param address C-string containing the address as described above
    */
  void Set (char const *address);
  /**
   * \brief Comparison operation between two Ipv4Addresses
   * \param other address to which to compare this address
   * \return True if the addresses are equal. False otherwise.
   */
  bool IsEqual (const Ipv4Address &other) const
  {
    return m_address == other.m_address;
  }
  /**
   * Serialize this address to a 4-byte buffer
   *
   * \param buf output buffer to which this address gets overwritten with this
   * Ipv4Address
   */
  void Serialize (uint8_t buf[4]) const;
  /**
   * \param buf buffer to read address from
   * \return an Ipv4Address
   * 
   * The input address is expected to be in network byte order format.
   */
  static Ipv4Address Deserialize (const uint8_t buf[4]);
  /**
   * \brief Print this address to the given output stream
   *
   * The print format is in the typical "192.168.1.1"
   * \param os The output stream to which this Ipv4Address is printed
   */
  void Print (std::ostream &os) const;
  /**
    * \return true if address is 255.255.255.255; false otherwise
    */
  bool IsBroadcast (void) const;
  /**
    * \return true only if address is in the range 224.0.0.0 - 239.255.255.255
    */
  bool IsMulticast (void) const;
  /**
    * \return true only if address is in local multicast address scope, 224.0.0.0/24
    */
  bool IsLocalMulticast (void) const;
  /**
   * \brief Combine this address with a network mask
   *
   * This method returns an IPv4 address that is this address combined
   * (bitwise and) with a network mask, yielding an IPv4 network
   * address.
   *
   * \param mask a network mask 
   */
  Ipv4Address CombineMask (Ipv4Mask const &mask) const;
  /**
   * \brief Generate subnet-directed broadcast address corresponding to mask
   *
   * The subnet-directed broadcast address has the host bits set to all
   * ones.  If this method is called with a mask of 255.255.255.255,
   * (i.e., the address is a /32 address), the program will assert, since
   * there is no subnet associated with a /32 address.
   *
   * \param mask a network mask 
   */
  Ipv4Address GetSubnetDirectedBroadcast (Ipv4Mask const &mask) const;
  /**
   * \brief Generate subnet-directed broadcast address corresponding to mask
   * 
   * The subnet-directed broadcast address has the host bits set to all
   * ones.  If this method is called with a mask of 255.255.255.255,
   * (i.e., the address is a /32 address), the program will assert, since
   * there is no subnet associated with a /32 address.
   *
   * \param mask a network mask 
   * \return true if the address, when combined with the input mask, has all
   * of its host bits set to one
   */
  bool IsSubnetDirectedBroadcast (Ipv4Mask const &mask) const;
  /**
   * \param address an address to compare type with
   *
   * \return true if the type of the address stored internally
   * is compatible with the type of the input address, false otherwise.
   */
  static bool IsMatchingType (const Address &address);
  /**
   * Convert an instance of this class to a polymorphic Address instance.
   *
   * \return a new Address instance
   */
  operator Address () const;
  /**
   * \param address a polymorphic address
   * \return a new Ipv4Address from the polymorphic address
   *
   * This function performs a type check and asserts if the
   * type of the input address is not compatible with an
   * Ipv4Address.
   */
  static Ipv4Address ConvertFrom (const Address &address);
  /**
   * \return the 0.0.0.0 address
   */
  static Ipv4Address GetZero (void);
  /**
   * \return the 0.0.0.0 address
   */
  static Ipv4Address GetAny (void);
  /**
   * \return the 255.255.255.255 address
   */
  static Ipv4Address GetBroadcast (void);
  /**
   * \return the 127.0.0.1 address
   */
  static Ipv4Address GetLoopback (void);

private:
  Address ConvertTo (void) const;
  static uint8_t GetType (void);
  uint32_t m_address;

  friend bool operator == (Ipv4Address const &a, Ipv4Address const &b);
  friend bool operator != (Ipv4Address const &a, Ipv4Address const &b);
  friend bool operator < (Ipv4Address const &addrA, Ipv4Address const &addrB);
};

/**
 * \ingroup address
 *
 * \brief a class to represent an Ipv4 address mask
 * 
 * The constructor takes arguments according to a few formats. 
 * Ipv4Mask ("255.255.255.255"), Ipv4Mask ("/32"), and Ipv4Mask (0xffffffff)
 * are all equivalent.
 */
class Ipv4Mask {
public:
  /**
   * Will initialize to a garbage value (0x66666666)
   */
  Ipv4Mask ();
  /**
   * \param mask bitwise integer representation of the mask
   * 
   * For example, the integer input 0xffffff00 yields a 24-bit mask
   */
  Ipv4Mask (uint32_t mask);
  /**
   * \param mask String constant either in "255.255.255.0" or "/24" format
   */
  Ipv4Mask (char const *mask);
  /**
   * \param a first address to compare
   * \param b second address to compare
   * \return true if both addresses are equal in their masked bits, 
   * corresponding to this mask
   */
  bool IsMatch (Ipv4Address a, Ipv4Address b) const;
  /**
   * \param other a mask to compare 
   * \return true if the mask equals the mask passed as input parameter
   */
  bool IsEqual (Ipv4Mask other) const;
  /** 
   * Get the host-order 32-bit IP mask
   * \return the host-order 32-bit IP mask
   */
  uint32_t Get (void) const;
  /**
   * input mask is in host order.
   * \param mask The host order 32-bit mask
   */
  void Set (uint32_t mask);
  /**
   * \brief Return the inverse mask in host order. 
   */
  uint32_t GetInverse (void) const;
  /**
   * \brief Print this mask to the given output stream
   *
   * The print format is in the typical "255.255.255.0"
   * \param os The output stream to which this Ipv4Address is printed
   */
  void Print (std::ostream &os) const;
  /**
   * \return the prefix length of mask (the yy in x.x.x.x/yy notation)
   */
  uint16_t GetPrefixLength (void) const;
  /**
   * \return the 255.0.0.0 mask corresponding to a typical loopback address
   */
  static Ipv4Mask GetLoopback (void);
  /**
   * \return the 0.0.0.0 mask
   */
  static Ipv4Mask GetZero (void);
  /**
   * \return the 255.255.255.255 mask
   */
  static Ipv4Mask GetOnes (void);

private:
  uint32_t m_mask;
};

/**
 * \class ns3::Ipv4AddressValue
 * \brief hold objects of type ns3::Ipv4Address
 */
/**
 * \class ns3::Ipv4MaskValue
 * \brief hold objects of type ns3::Ipv4Mask
 */

ATTRIBUTE_HELPER_HEADER (Ipv4Address);
ATTRIBUTE_HELPER_HEADER (Ipv4Mask);

std::ostream& operator<< (std::ostream& os, Ipv4Address const& address);
std::ostream& operator<< (std::ostream& os, Ipv4Mask const& mask);
std::istream & operator >> (std::istream &is, Ipv4Address &address);
std::istream & operator >> (std::istream &is, Ipv4Mask &mask);

inline bool operator == (const Ipv4Address &a, const Ipv4Address &b)
{
  return (a.m_address == b.m_address);
}
inline bool operator != (const Ipv4Address &a, const Ipv4Address &b)
{
  return (a.m_address != b.m_address);
}
inline bool operator < (const Ipv4Address &a, const Ipv4Address &b)
{
  return (a.m_address < b.m_address);
}


#ifndef WIN32
class Ipv4AddressHash : public std::unary_function<Ipv4Address, size_t> {
public:
	size_t operator() (Ipv4Address const &x) const;
};
#else
class Ipv4AddressHash : public stdext::hash_compare<ns3::Ipv4Address> {
public:
	size_t operator()(Ipv4Address const &x) const;
	bool operator() (const Ipv4Address& s1, const Ipv4Address& s2) const
	{
		return s1 < s2;
	}

};
#endif

bool operator == (Ipv4Mask const &a, Ipv4Mask const &b);
bool operator != (Ipv4Mask const &a, Ipv4Mask const &b);

} // namespace ns3

#endif /* IPV4_ADDRESS_H */

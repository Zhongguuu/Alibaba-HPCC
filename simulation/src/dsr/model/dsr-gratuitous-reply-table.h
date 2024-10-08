/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Yufei Cheng
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
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#ifndef DSR_GRATUITOUS_REPLY_TABLE_H
#define DSR_GRATUITOUS_REPLY_TABLE_H

#include "ns3/simulator.h"
#include "ns3/timer.h"
#include "ns3/ipv4-address.h"
#include "ns3/callback.h"
#include <vector>

namespace ns3 {
namespace dsr {
/*
 * The gratuitous table entries, it maintains the already sent gratuitous route reply entries.
 * When the node "promiscuously" received a packet destined for other nodes, and inferred a shorter
 * route for the data packet, it will construct a route reply and send back to the source
 */
struct GraReplyEntry
{
  Ipv4Address m_replyTo;
  Ipv4Address m_hearFrom;
  Time m_gratReplyHoldoff;

  GraReplyEntry (Ipv4Address t, Ipv4Address f, Time h)
    : m_replyTo (t),
      m_hearFrom (f),
      m_gratReplyHoldoff (h)
  {
  }
};
/**
 * \ingroup dsr
 * \brief maintain the gratuitous reply
 */
class GraReply  : public Object
{
public:
  // / c-tor
  /**
   * \brief Get the type identificator.
   * \return type identificator
   */
  static TypeId GetTypeId ();
  /**
   * \brief Constructor.
   */
  GraReply ();
  /**
   * \brief Destructor.
   */
  virtual ~GraReply ();
  // / Set the gratuitous reply table size
  void SetGraTableSize (uint32_t g)
  {
    GraReplyTableSize = g;
  }
  // / Get the gratuitous reply table size
  uint32_t GetGraTableSize () const
  {
    return GraReplyTableSize;
  }
  // / Add a new gratuitous reply entry
  bool AddEntry (GraReplyEntry & graTableEntry);
  // / Update the route entry if found, create a new one if not
  bool FindAndUpdate (Ipv4Address replyTo, Ipv4Address replyFrom, Time gratReplyHoldoff);
  // / Remove all expired entries
  void Purge ();
  // / Remove all entries
  void Clear ()
  {
    m_graReply.clear ();
  }

private:
  // / Vector of entries
  std::vector<GraReplyEntry> m_graReply;
  // / The max # of gratuitous reply entries to hold
  uint32_t GraReplyTableSize;

  // / Check if the entry is expired or not
  struct IsExpired
  {
    bool operator() (const struct GraReplyEntry & b) const
    {
      return (b.m_gratReplyHoldoff < Simulator::Now ());
    }
  };
};
}  // namespace dsr
}  // namespace ns3

#endif /* DSR_GRATUITOUS_REPLY_TABLE_H */

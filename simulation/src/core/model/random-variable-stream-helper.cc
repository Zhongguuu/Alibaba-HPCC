/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of Washington
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
 */
#include "random-variable-stream.h"
#include "random-variable-stream-helper.h"
#include "config.h"
#include "pointer.h"
#include "log.h"
#include "assert.h"

NS_LOG_COMPONENT_DEFINE ("RandomVariableStreamHelper");

namespace ns3 {

int64_t RandomVariableStreamHelper::AssignStreams (std::string path, int64_t stream)
{
  NS_ASSERT (stream >= 0);
  Config::MatchContainer mc = Config::LookupMatches (path);
  int64_t i = 0;
  for (Config::MatchContainer::Iterator mci = mc.Begin (); mci != mc.End (); ++mci, ++i)
    {
      PointerValue ptr = mc.Get (i);
      Ptr<RandomVariableStream> rvs = ptr.Get<RandomVariableStream> ();
      NS_LOG_DEBUG ("RandomVariableStream found: " << rvs << "; setting stream to " << (stream + i));
      rvs->SetStream (stream + i);
    }
  return i;
}

} // namespace ns3

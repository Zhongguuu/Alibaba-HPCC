/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Drexel University
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
 * Author: Joe Kopena (tjkopena@cs.drexel.edu)
 */

#include "ns3/log.h"
#include "ns3/simulator.h"

#include "data-calculator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DataCalculator");

static double zero = 0;
const double ns3::NaN = zero / zero;

//--------------------------------------------------------------
//----------------------------------------------
DataCalculator::DataCalculator() :
  m_enabled (true)
{
  NS_LOG_FUNCTION_NOARGS ();
}

DataCalculator::~DataCalculator()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
DataCalculator::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Simulator::Cancel (m_startEvent);
  Simulator::Cancel (m_stopEvent);

  Object::DoDispose ();
  // DataCalculator::DoDispose
}

//----------------------------------------------
void
DataCalculator::SetKey (const std::string key)
{
  m_key = key;
  // end DataCalculator::SetKey
}

std::string
DataCalculator::GetKey () const
{
  return m_key;
  // end DataCalculator::GetKey
}

//----------------------------------------------
void
DataCalculator::SetContext (const std::string context)
{
  m_context = context;
  // end DataCalculator::SetContext
}

std::string
DataCalculator::GetContext () const
{
  return m_context;
  // end DataCalculator::GetContext
}
//----------------------------------------------
void
DataCalculator::Enable ()
{
  m_enabled = true;
  // end DataCalculator::Enable
}

void
DataCalculator::Disable ()
{
  m_enabled = false;
  // end DataCalculator::Disable
}

bool
DataCalculator::GetEnabled () const
{
  return m_enabled;
  // end DataCalculator::GetEnabled
}

//----------------------------------------------
void
DataCalculator::Start (const Time& startTime)
{

  m_startEvent = Simulator::Schedule (startTime,
                                      &DataCalculator::Enable, this);

  // end DataCalculator::Start
}

void
DataCalculator::Stop (const Time& stopTime)
{
  m_stopEvent = Simulator::Schedule (stopTime,
                                     &DataCalculator::Disable, this);
  // end DataCalculator::Stop
}

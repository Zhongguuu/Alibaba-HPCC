/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 CTTC
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

#ifndef REM_SPECTRUM_PHY_H
#define REM_SPECTRUM_PHY_H


#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/net-device.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <string>
#include <fstream>

namespace ns3 {


/**
 *
 * This minimal SpectrumPhy implemetation calculates the SINR with
 * respect to the strongest signal for a given point. The original
 * purpose of this class is to be used to generate a
 * Radio Environment Map (REM) by locating several instances in a grid
 * fashion, and connecting them to the channel only for a very short
 * amount of time. 
 *
 * The assumption on which this class works is that the system
 * being considered is an infrastructured radio access network using
 * FDD, hence all signals will be transmitted simultaneously. 
 */
class RemSpectrumPhy : public SpectrumPhy
{

public:
  RemSpectrumPhy ();
  virtual ~RemSpectrumPhy ();

  // inherited from Object
  void DoDispose ();
  static TypeId GetTypeId (void);

  // inherited from SpectrumPhy
  void SetChannel (Ptr<SpectrumChannel> c);
  void SetMobility (Ptr<MobilityModel> m);
  void SetDevice (Ptr<NetDevice> d);
  Ptr<MobilityModel> GetMobility ();
  Ptr<NetDevice> GetDevice ();
  Ptr<const SpectrumModel> GetRxSpectrumModel () const;
  Ptr<AntennaModel> GetRxAntenna ();
  void StartRx (Ptr<SpectrumSignalParameters> params);

  /** 
   * set the RX spectrum model to be used
   * 
   * \param m 
   */
  void SetRxSpectrumModel (Ptr<const SpectrumModel> m);

  /** 
   * 
   * \return the Signal to Noise Ratio calculated 
   */
  double GetSinr (double noisePower);

  /** 
   * make StartRx a no-op from now on, and mark instance as inactive
   * 
   */
  void Deactivate ();

  /** 
   * 
   * \return true if active
   */
  bool IsActive ();

  /** 
   * Reset the SINR calculator
   * 
   */
  void Reset ();

private:
  Ptr<MobilityModel> m_mobility;
  Ptr<const SpectrumModel> m_rxSpectrumModel;

  double m_referenceSignalPower;
  double m_sumPower;

  bool m_active;

};






}





#endif /* REM_SPECTRUM_PHY_H */

/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * NIST-developed software is provided by NIST as a public
 * service. You may use, copy and distribute copies of the software in
 * any medium, provided that you keep intact this entire notice. You
 * may improve, modify and create derivative works of the software or
 * any portion of the software, and you may copy and distribute such
 * modifications or works. Modified works should carry a notice
 * stating that you changed the software and should note the date and
 * nature of any such change. Please explicitly acknowledge the
 * National Institute of Standards and Technology as the source of the
 * software.
 *
 * NIST-developed software is expressly provided "AS IS." NIST MAKES
 * NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT OR ARISING BY
 * OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * NON-INFRINGEMENT AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR
 * WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED
 * OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT
 * WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE
 * SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE
 * CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
 *
 * You are solely responsible for determining the appropriateness of
 * using and distributing the software and you assume all risks
 * associated with its use, including but not limited to the risks and
 * costs of program errors, compliance with applicable laws, damage to
 * or loss of data, programs or equipment, and the unavailability or
 * interruption of operation. This software is not intended to be used
 * in any situation where a failure could cause risk of injury or
 * damage to property. The software developed by NIST employees is not
 * subject to copyright protection within the United States.
 */

#include "lte-sl-ue-rrc.h"

#include <ns3/fatal-error.h>
#include <ns3/log.h>
#include <ns3/abort.h>

#include <ns3/pointer.h>
#include <ns3/object-map.h>
#include <ns3/object-factory.h>
#include <ns3/simulator.h>

#include <ns3/lte-rrc-sap.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LteSlUeRrc");
NS_OBJECT_ENSURE_REGISTERED (LteSlUeRrc);

TypeId LteSlUeRrc::GetTypeId (void)
{
  static TypeId  tid = TypeId ("ns3::LteSlUeRrc")
    .SetParent<Object> ()
    .AddConstructor<LteSlUeRrc> ()

  ;
  return tid;
}
LteSlUeRrc::LteSlUeRrc ()
  : m_slEnabled (false), m_discEnabled (false)
{
  NS_LOG_FUNCTION (this);
  m_rand = CreateObject<UniformRandomVariable> ();
}

void
LteSlUeRrc::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
}

LteSlUeRrc::~LteSlUeRrc (void)
{
  NS_LOG_FUNCTION (this);
}

void
LteSlUeRrc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
}
/**
 * \brief makes a copy of the sidelink configuration
 * \return a copy of the sidelink configuration
 */
Ptr<LteSlUeRrc>
LteSlUeRrc::Copy ()
{
  NS_LOG_FUNCTION (this);
  Ptr<LteSlUeRrc> copy = CreateObject<LteSlUeRrc> ();
  //copy internal values
  copy->m_slEnabled = m_slEnabled;
  copy->m_slMap = m_slMap;
  return copy;
}

void
LteSlUeRrc::SetSlEnabled (bool status)
{
  NS_LOG_FUNCTION (this);
  m_slEnabled = status;
}

bool
LteSlUeRrc::IsSlEnabled ()
{
  NS_LOG_FUNCTION (this);
  return m_slEnabled;
}

void
LteSlUeRrc::SetDiscEnabled (bool status)
{
  NS_LOG_FUNCTION (this);
  m_discEnabled = status;
}

bool
LteSlUeRrc::IsDiscEnabled ()
{
  NS_LOG_FUNCTION (this);
  return m_discEnabled;
}

void
LteSlUeRrc::SetSlPreconfiguration (LteRrcSap::SlPreconfiguration preconfiguration)
{
  NS_LOG_FUNCTION (this);
  m_preconfiguration = preconfiguration;
}

LteRrcSap::SlPreconfiguration
LteSlUeRrc::GetSlPreconfiguration ()
{
  NS_LOG_FUNCTION (this);
  return m_preconfiguration;
}

void
LteSlUeRrc::SetSourceL2Id (uint32_t src)
{
  NS_LOG_FUNCTION (this);
  m_sourceL2Id = src;
}

bool
LteSlUeRrc::IsTxInterested ()
{
  NS_LOG_FUNCTION (this);

  //Loop through each bearer to see if one is interested to transmit
  std::map <uint32_t, std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> > >::iterator srcIt = m_slrbMap.find (m_sourceL2Id);
  if (srcIt == m_slrbMap.end ())
    {
      return false;
    }
  return m_slrbMap[m_sourceL2Id].size () > 0;
}

bool
LteSlUeRrc::IsRxInterested ()
{
  NS_LOG_FUNCTION (this);

  return m_rxGroup.size () > 0;
}

bool
LteSlUeRrc::IsMonitoringInterested ()
{
  NS_LOG_FUNCTION (this);
  return m_monitorApps.size () > 0;
}

bool
LteSlUeRrc::IsAnnouncingInterested ()
{
  NS_LOG_FUNCTION (this);
  return m_announceApps.size () > 0;
}

std::list<uint32_t>
LteSlUeRrc::GetTxDestinations ()
{
  std::list<uint32_t> destinations;

  //Loop through each bearer to see if one is interested to transmit
  std::map <uint32_t, std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> > >::iterator srcIt = m_slrbMap.find (m_sourceL2Id);
  if (srcIt != m_slrbMap.end ())
    {
      //Loop through each bearer to see if one is interested to transmit
      std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> >::iterator it;
      for (it = m_slrbMap[m_sourceL2Id].begin (); it != m_slrbMap[m_sourceL2Id].end (); it++)
        {
          destinations.push_back (it->second->m_destinationL2Id);
        }
    }
  return destinations;
}

uint8_t
LteSlUeRrc::GetDiscTxResources ()
{
  return m_discTxResources;
}

void
LteSlUeRrc::SetDiscTxResources (uint8_t numDiscTxRes)
{
  m_discTxResources = numDiscTxRes;
}

uint16_t
LteSlUeRrc::GetDiscInterFreq ()
{
  return m_discInterFreq;
}
void
LteSlUeRrc::SetDiscInterFreq (uint16_t ulEarfcn)
{
  m_discInterFreq = ulEarfcn;
}

bool
LteSlUeRrc::AddSidelinkRadioBearer (Ptr<LteSidelinkRadioBearerInfo> slb)
{
  std::map <uint32_t, std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> > >::iterator srcIt = m_slrbMap.find (slb->m_sourceL2Id);
  if (srcIt == m_slrbMap.end ())
    {
      //must insert map
      std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> > empty;

      m_slrbMap.insert (std::pair <uint32_t, std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> > > (slb->m_sourceL2Id, empty));
      NS_LOG_LOGIC ("First SLRB for source " << slb->m_sourceL2Id);
    }

  std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> >::iterator groupIt = m_slrbMap[slb->m_sourceL2Id].find (slb->m_destinationL2Id);
  NS_ASSERT (groupIt == m_slrbMap[slb->m_sourceL2Id].end ());
  NS_LOG_LOGIC ("Adding SLRB " << slb->m_sourceL2Id << "->" << slb->m_destinationL2Id);
  m_slrbMap[slb->m_sourceL2Id].insert (std::pair<uint32_t,Ptr<LteSidelinkRadioBearerInfo> > (slb->m_destinationL2Id, slb));

  return true;
}

bool
LteSlUeRrc::DeleteSidelinkRadioBearer (uint32_t src, uint32_t group)
{
  bool deleted = m_slrbMap[src].erase (group) > 0;
  if (m_slrbMap[src].size () == 0)
    {
      //can delete the source as well
      m_slrbMap.erase (src);
    }
  //remove from receiving group if present
  m_rxGroup.remove (group);

  return deleted;
}

Ptr<LteSidelinkRadioBearerInfo>
LteSlUeRrc::GetSidelinkRadioBearer (uint32_t src, uint32_t group)
{
  Ptr<LteSidelinkRadioBearerInfo> slrb = NULL;

  NS_LOG_LOGIC ("Searching SLRB " << src << "->" << group);

  std::map <uint32_t, std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> > >::iterator srcIt = m_slrbMap.find (src);
  if (srcIt != m_slrbMap.end ())
    {
      std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> >::iterator srcIt2 = (*srcIt).second.find (group);
      if (srcIt2 != (*srcIt).second.end ())
        {
          slrb = m_slrbMap[src][group];
        }
    }
  return slrb;
}

Ptr<LteSidelinkRadioBearerInfo>
LteSlUeRrc::GetSidelinkRadioBearer (uint32_t group)
{
  return GetSidelinkRadioBearer (m_sourceL2Id, group);
}

void
LteSlUeRrc::AddDiscoveryApps (std::list<uint32_t> apps, bool rxtx)
{
  for (std::list<uint32_t>::iterator it = apps.begin (); it != apps.end (); ++it)
    {
      if (rxtx)
        {
          m_announceApps.push_back (*it);
        }
      else
        {
          m_monitorApps.push_back (*it);
        }
    }
}


void
LteSlUeRrc::RemoveDiscoveryApps (std::list<uint32_t> apps, bool rxtx)
{
  for (std::list<uint32_t>::iterator it = apps.begin (); it != apps.end (); ++it)
    {
      if (rxtx)
        {
          m_announceApps.remove (*it);
        }
      else
        {
          m_monitorApps.remove (*it);
        }
    }
}

void
LteSlUeRrc::RecordTransmissionOfSidelinkUeInformation ()
{
  m_lastSidelinkUeInformationTime = Simulator::Now ();
}

double
LteSlUeRrc::GetTimeSinceLastTransmissionOfSidelinkUeInformation ()
{
  return (Simulator::Now () - m_lastSidelinkUeInformationTime).GetSeconds ();
}

uint8_t
LteSlUeRrc::GetNextLcid ()
{
  //find unused the LCID
  bool found = true;
  uint8_t lcid;

  for (lcid = 1; lcid < 11; lcid++)
    {
      found = false;
      std::map <uint32_t, Ptr<LteSidelinkRadioBearerInfo> >::iterator it;
      for (it = m_slrbMap[m_sourceL2Id].begin (); it != m_slrbMap[m_sourceL2Id].end (); it++)
        {
          if (it->second->m_logicalChannelIdentity == lcid)
            {
              found = true;
              break;
            }
        }
      if (!found)
        {
          break; //avoid increasing lcid
        }
    }
  NS_ASSERT (!found);
  return lcid;
}

bool
LteSlUeRrc::IsCellBroadcastingSIB18 (uint16_t cellId)
{
  std::map <uint16_t, LteSlCellConfiguration>::iterator it = m_slMap.find (cellId);
  return (it != m_slMap.end () && it->second.haveSib18);
}

bool
LteSlUeRrc::IsCellBroadcastingSIB19 (uint16_t cellId)
{
  std::map <uint16_t, LteSlCellConfiguration>::iterator it = m_slMap.find (cellId);
  return (it != m_slMap.end () && it->second.haveSib19);
}

int64_t
LteSlUeRrc::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_rand->SetStream (stream);
  return 1;
}

} // namespace ns3

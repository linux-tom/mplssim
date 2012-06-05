#include "burstyApp.h"
#include <ctime>


BurstyApp::BurstyApp ()

  : m_socket (0), 
    m_peer (), 
    m_packetSize (0), 
    m_nPackets (0), 
    m_dataRate (0), 
    m_sendEvent (), 
    m_running (false), 
    m_packetsSent (0)
{
	this->counter=0;
}


BurstyApp::~BurstyApp()
{
  m_socket = 0;
}

void BurstyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
 // m_dataRate = dataRate;
}

void BurstyApp::StartApplication (void)
{
  m_running = true;
  m_packetsSent = 0;
  m_socket->Bind ();
  m_socket->Connect (m_peer);
  SendPacket ();
}

void BurstyApp::StopApplication (void)
{
  m_running = false;

  if (m_sendEvent.IsRunning ())
    {
	for ( size_t i=0; i<sendEventVector.size(); i++ )
	{
		Simulator::Cancel (sendEventVector[i]);
	}
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

void BurstyApp::SendPacket (void)
{
  Ptr<Packet> packet = Create<Packet> (m_packetSize);
  m_socket->Send (packet);

	//std::cout <<counter<<" sending stuff " << std::endl;
	counter ++;

  if (++m_packetsSent < m_nPackets && counter >= sendEventVector.size() )
    {
      ScheduleTx ();
	counter =0;
    }
}

void BurstyApp::ScheduleTx (void)
{
  if (m_running)
    {
	std::srand((unsigned)std::time(0));

	sendEventVector.clear();

	//get random count of numbers

	unsigned int random_integer; 
	for(int index=0; index<6; index++)
	{ 
		random_integer = (std::rand()%5000)+1; 
	} 

	//int baseTime=((m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ()))*1000);
	int baseTime=(Simulator::Now().GetSeconds () + (std::rand()%30)+1)*1000;
	//std::cout <<" sending stuff " <<baseTime<<" "<<random_integer<< std::endl;

	for ( size_t j=0; j<random_integer; j++ )
	{
		Time tNext (MilliSeconds( baseTime + j));
		sendEventVector.push_back(Simulator::Schedule (tNext, &BurstyApp::SendPacket, this));
	}
	      //m_sendEvent = Simulator::Schedule (tNext, &BurstyApp::SendPacket, this);
    }
}

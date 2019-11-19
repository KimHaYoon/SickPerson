#include "PacketMgr.h"

DEFINITION_SINGLE(PacketMgr)

PacketMgr::PacketMgr()
{
}


PacketMgr::~PacketMgr()
{
}

bool PacketMgr::Init()
{
	return true;
}

void PacketMgr::Send_Packet(const unsigned int & id, void * buf)
{
	char* packet = reinterpret_cast<char*>(buf);
	int packet_size = packet[0];
	SOCKET	client_s = m_clients[id]->socket;

	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->event = EV_SEND;
	memcpy(send_over->net_buf, packet, packet_size);
	send_over->wsabuf[0].buf = send_over->net_buf;
	send_over->wsabuf[0].len = packet_size;
	if (WSASend(client_s, send_over->wsabuf, 1, 0, 0, &send_over->overlapped, 0) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			cout << "Error - Fail WSASend(error_code : " << WSAGetLastError << " ) " << endl;
		}
	}
}

void PacketMgr::Send_Login_Ok_Packet(const int & id)
{
	sc_packet_login_ok packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	Send_Packet(id, &packet);
}

#pragma once

#include "stdafx.h"

class PacketMgr
{
private:
	map<int, PLAYER_INFO*>	m_clients;
public:
	bool Init();
public:
	void Send_Packet(const unsigned int& id, void* buf);

public:
	void Send_Login_Ok_Packet(const int& id);
private:
	DECLARE_SINGLE(PacketMgr)
};


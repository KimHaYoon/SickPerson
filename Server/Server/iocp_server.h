#pragma once
#include "stdafx.h"

class iocp_server
{
public:
	iocp_server();
	~iocp_server();

private:
	HANDLE	m_Iocp;
	int			m_CpuCore;

	vector<thread*>	m_WorkerThread;
	thread					m_TimerThread;
	thread					m_DBThread;
	bool						m_ServerShutdown{ false };

public:
	void GetServerIpAddress();
	void Initialize();
	void	CheckThisCpuCount();

	void MakeThreads();

	void Do_WokerThread();
	void Do_AcceptThread();
	void Do_TimerThread();

	int ConnectDB(int id);
	int CreateDB(int id);
	int SavePosDB(int id);

	void ProcessPacket(const unsigned int& id, const char* buf);
	void SendPacket(const unsigned int& id, const char* packet);
	
	// 오류 출력
	void error_display(const char* msg, int err_no);
	void error_quit(const char* msg, int err_no);

private:
	map<int, PLAYER_INFO*>	m_clients;
	unsigned int							new_user_id;
};


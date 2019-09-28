#include "iocp_server.h"



iocp_server::iocp_server()
{
	GetServerIpAddress();
	CheckThisCpuCount();

	Initialize();

	MakeThreads();
}


iocp_server::~iocp_server()
{
	WSACleanup();
}

void iocp_server::GetServerIpAddress()
{
	// Winsock Start - winsock.dll 로드
	WSADATA	WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		cout << "Error - Can not load 'winsock.dll file\n'";
		return;
	}

	PHOSTENT	hostinfo;
	char				hostname[50];
	char				ipaddr[50];
	memset(hostname, 0, sizeof(hostname));
	memset(ipaddr, 0, sizeof(ipaddr));

	int err_no = gethostname(hostname, sizeof(hostname));
	if (err_no == 0) {
		hostinfo = gethostbyname(hostname);
		strcpy(ipaddr, inet_ntoa(*reinterpret_cast<struct in_addr*>(hostinfo->h_addr_list[0])));
	}
	WSACleanup();
	cout << "Server IP Address" << ipaddr << endl;
}

void iocp_server::Initialize()
{
	WSADATA	WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		cout << "Error - Can not load 'winsock.dll' file\n";
		return;
	}
	m_Iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	if (m_Iocp == NULL) {
		int err_no = WSAGetLastError();
		error_quit("IOCP_Server_Initialize - ", err_no);
	}
}

void iocp_server::CheckThisCpuCount()
{
	SYSTEM_INFO	si;
	GetSystemInfo(&si);
	m_CpuCore = static_cast<int>(si.dwNumberOfProcessors) * 2;
	cout << "CPU Core Count = " << m_CpuCore / 2 << "Thread = " << m_CpuCore << endl;
}

void iocp_server::MakeThreads()
{
	m_WorkerThread.reserve(m_CpuCore);

	// worker_thread create
	for (int i = 0; i < m_CpuCore; ++i)
		m_WorkerThread.emplace_back(new thread{ &iocp_server::m_WorkerThread, this });
	
	// accept_thread create
	thread Accept_Thread{ &iocp_server::Do_AcceptThread, this };
	while (m_ServerShutdown) Sleep(1000);

	// timer_thread create
	thread Timer_Thread{ &iocp_server::Do_TimerThread, this };

	// database_thread create
	thread DB_Thread{ &iocp_server::Do_DBThread, this };

	// thread_delete
	for (auto thread : m_WorkerThread) {
		thread->join();
		delete thread;
	}
	Accept_Thread.join();
	Timer_Thread.join();
	DB_Thread.join();
}

void iocp_server::Do_WokerThread()
{
	while (true == (!m_ServerShutdown)) {
		DWORD	io_byte;
		unsigned long long	key;
		OVER_EX	*lpover_ex;

		BOOL	is_error = GetQueuedCompletionStatus(m_Iocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED *>(&lpover_ex), INFINITE);

		// client가 접속을 끊었을 경우
		if (FALSE == is_error || 0 == io_byte) {
			if (FALSE == is_error) {
				int err_no = WSAGetLastError();
				error_display("Woker_Thread Start - GetQueuedCompletionStatus", err_no);
			}
			closesocket(m_clients[key]->socket);
			m_clients[key]->connected = false;
			cout << "[ Clients" << key << " ] Disconnected" << endl;
		}
	}
}

void iocp_server::Do_AcceptThread()
{
}

void iocp_server::Do_TimerThread()
{
}

void iocp_server::Do_DBThread()
{
}

void iocp_server::ProcessPacket(const unsigned int & id, const char * buf)
{
	// packet[0] = packet size --> 0번째 자리에는 무조건 패킷의 크기가 들어가야만 한다.
	// packet[1] = type --> 1번째 자리에는 현재 패킷이 무슨 패킷인지 속성을 정해주는 값이다.
	// packet[...] = data --> 2번째 부터는 속성에 맞는 순서대로 처리 해준다.
	
	// buf[1] 번째
	switch (buf[1]) {
	case CS_LOGIN:
		break;
	case CS_MOVE:
		break;
	case CS_ATTACK:
		break;
	case CS_CHAT:
		break;
	case CS_LOGOUT:
		break;
	default:
	{
		cout << "Error, Unknown signal -> [ " << id << " ] protocol num = " << buf[1] << endl;
		exit(-1);
	}
	break;
	}
}

void iocp_server::SendPacket(const unsigned int & id, const char * packet)
{
	SOCKET	client_s = m_clients[id]->socket;

	OVER_EX*	over = reinterpret_cast<OVER_EX*>(malloc(sizeof(OVER_EX)));

	over->dataBuffer.len = packet[0];
	over->dataBuffer.buf = over->messageBuffer;
	memcpy(over->messageBuffer, packet, packet[0]);
	ZeroMemory(&(over->overlapped), sizeof(WSAOVERLAPPED));
	over->event = EV_SEND;

	if (WSASend(client_s, &over->dataBuffer, 1, NULL, 0, &(over->overlapped), NULL) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			cout << "Error - Fail WSASend(error_code : " << WSAGetLastError << " ) " << endl;
		}
	}
}

void iocp_server::error_display(const char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[ %s ]", msg);
	wprintf(L"에러 %s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void iocp_server::error_quit(const char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

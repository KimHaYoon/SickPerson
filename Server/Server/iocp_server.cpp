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
	// Winsock Start - winsock.dll �ε�
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
	m_CpuCore = static_cast<int>(static_cast<int>(si.dwNumberOfProcessors) * 1.5f);
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

		// client�� ������ ������ ���
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
	// Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return;
	}

	// 1. ���ϻ���  
	SOCKET listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Error - Invalid socket\n";
		return;
	}

	// �������� ��ü����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. ���ϼ���
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "Error - Fail bind\n";
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return;
	}

	// 3. ���Ŵ�⿭����
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		cout << "Error - Fail listen\n";
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return;
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;

	while (true)
	{
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			cout << "Error - Accept Failure\n";
			return;
		}

		// �α���
		//char ID[10];
		//char ID_Buf[10];
		//// recv �ް�
		//int retval = recv(clientSocket, ID_Buf, sizeof(ID_Buf), 0);
		//if (retval == SOCKET_ERROR) {
		//	cout << "Recv Error\n";
		//}
		//ID_Buf[retval - 1] = '\0';
		//strcpy(ID, ID_Buf);

		//int new_id = atoi(ID);
		////int new_id = get_new_id();

		//if (true == clients[new_id].connected) {
		//	cout << "�̹� �����Ͽ��ִ� �÷��̾� �Դϴ�." << endl;
		//	closesocket(clientSocket);
		//	continue;
		//}

		//new_id = ConnectDB(new_id);

		//if (-1 == new_id) {
		//	cout << "�������� �ʴ� �÷��̾� �Դϴ�.\n";
		//	cout << "���̵� �����մϴ�.\n";
		//	new_id = CreateDB(atoi(ID));
		//	new_id = ConnectDB(new_id);
		//	//closesocket(clientSocket);
		//	//continue;
		//}

		//clients[new_id].socket = clientSocket;
		//clients[new_id].over.dataBuffer.len = MAX_BUFFER;
		//clients[new_id].over.dataBuffer.buf =
		//	clients[clientSocket].over.messageBuffer;
		//clients[new_id].over.event = EV_RECV;//���� �� �κ� Ȯ��
		////clients[new_id].x = START_X;
		////clients[new_id].y = START_Y;
		//clients[new_id].viewlist.clear();
		//clients[new_id].prev_size = 0;
		//ZeroMemory(&clients[new_id].over.overlapped, sizeof(WSAOVERLAPPED));
		//flags = 0;

		//clients[new_id].attack = false;
		//clients[new_id].damage = clients[new_id].level * 15;

		//CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, new_id, 0);
		//clients[new_id].connected = true;

		//send_login_ok_packet(new_id);

		// �þ�ó��

		/*for (int i = 0; i < MAX_USER; ++i) {
			if (false == clients[i].connected) continue;
			if (i == new_id) continue;
			clients[i].v_lock.lock();
			if (true == is_near_object(i, new_id, VIEW_RADIUS)) {
				clients[i].viewlist.insert(new_id);
				clients[i].v_lock.unlock();
				send_add_object_pacekt(i, new_id);
			}
			else
				clients[i].v_lock.unlock();
		}

		for (int i = 0; i < MAX_USER; ++i) {
			if (false == clients[i].connected) continue;
			if (i == new_id) continue;
			if (true == is_near_object(i, new_id, VIEW_RADIUS)) {
				clients[new_id].viewlist.insert(i);
				send_add_object_pacekt(new_id, i);
			}
		}

		for (int i = 0; i < NUM_NPC; ++i) {
			int npc_id = i + NPC_ID_START;
			if (true == is_near_object(npc_id, new_id, VIEW_RADIUS)) {
				if (true == clients[npc_id].connected) {
					wake_up_NPC(npc_id);
					clients[new_id].viewlist.insert(npc_id);
					send_add_object_pacekt(new_id, npc_id);
				}
			}
		}*/
		/*for (int i = 0; i < NUM_ITEM; ++i) {
			int item_id = i + ITEM_ID_START;
			if (true == is_near_object(item_id, new_id, VIEW_RADIUS)) {
				if (true == clients[item_id].connected) {
					clients[new_id].viewlist.insert(item_id);
					send_add_object_pacekt(new_id, item_id);
				}
			}
		}*/
		//send_add_object_pacekt(new_id, new_id);

		//do_recv(new_id);
	}
}

void iocp_server::Do_TimerThread()
{
	
}

int iocp_server::ConnectDB(int id)
{
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;

	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
				//Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"2015180031", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

				//Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					//printf("DB Access OK!!\n");
					cout << "DB Access OK!!\n";

					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						SQLCancel(hstmt);
						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}
					SQLDisconnect(hdbc);
				}
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
	return 0;
}

int iocp_server::CreateDB(int id)
{
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;
	SQLINTEGER sUserID, sUserMaxHp, sUserHp, sUserLevel, sUserMaxExp, sUserExp, sUserPosX, sUserPosY;
	SQLLEN cbUserID = 0, cbMaxHp = 0, cbHp = 0, cbLevel = 0, cbMaxExp = 0, cbExp = 0, cbPosX = 0, cbPosY = 0; // callback

	int client_id = id;
	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
				//Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"2015180031", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

				//Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					//printf("DB Access OK!!\n");
					cout << "DB Access OK!!\n";
					
					
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

					}

					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						SQLCancel(hstmt);
						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}
					SQLDisconnect(hdbc);
				}
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
	return 0;
}

int iocp_server::SavePosDB(int id)
{
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;
	SQLINTEGER sUserID, sUserMaxHp, sUserHp, sUserLevel, sUserMaxExp, sUserExp, sUserPosX, sUserPosY;
	SQLLEN cbUserID = 0, cbMaxHp = 0, cbHp = 0, cbLevel = 0, cbMaxExp = 0, cbExp = 0, cbPosX = 0, cbPosY = 0; // callback
	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
				//Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"2015180031", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

				//Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					//printf("DB Access OK!!\n");
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

						//cout << "<SAVE> ID: " << id << "POS ( " << clients[id].x << ", " << clients[id].y << ")" << endl;
						//// Bind columns 1, 2, and 3  
						//retcode = SQLBindCol(hstmt, 1, SQL_C_LONG, &sUserID, 100, &cbUserID);
						//retcode = SQLBindCol(hstmt, 2, SQL_C_LONG, &sUserPosX, 100, &cbPosX);
						//retcode = SQLBindCol(hstmt, 3, SQL_C_LONG, &sUserPosY, 100, &cbPosY);

						//retcode = SQLFetch(hstmt);
						//if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
						//	show_error();
						//}

						////Fetch and print each row of data. On an error, display a message and exit.  
						//if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
						//{
						//	cout << "<SAVE> ID: " << sUserID << "POS ( " << sUserPosX << ", " << sUserPosY << ")";
						//}
					}

					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						SQLCancel(hstmt);
						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}
					SQLDisconnect(hdbc);
				}
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
}

void iocp_server::ProcessPacket(const unsigned int & id, const char * buf)
{
	// packet[0] = packet size --> 0��° �ڸ����� ������ ��Ŷ�� ũ�Ⱑ ���߸� �Ѵ�.
	// packet[1] = type --> 1��° �ڸ����� ���� ��Ŷ�� ���� ��Ŷ���� �Ӽ��� �����ִ� ���̴�.
	// packet[...] = data --> 2��° ���ʹ� �Ӽ��� �´� ������� ó�� ���ش�.
	
	// buf[1] ��°
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
	wprintf(L"���� %s\n", lpMsgBuf);
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

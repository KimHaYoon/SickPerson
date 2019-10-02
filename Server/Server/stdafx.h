#pragma once
#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <Windows.h>

#include <sqlext.h>  

#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace chrono;

// enum ��
#include "types.h"

// Ŭ���̾�Ʈ�� ������ ����ϱ� ���� �⺻���� ��� ��Ģ(���� �� Ŭ���̾�Ʈ ������, Ŭ���̾�Ʈ�� ����ȭ�� ������ ��ģ��.)
#include "protocol.h"

// struct ����ü ��
#include "struct.h"

// iocp ������ ������ �Ǵ� ���� �ٽ����� Ŭ������ ������ ��� �ִ�. (iocp��  boost �� �ϳ��� ���� �ȴ�.)
#include "iocp_server.h"

// boost asio �������� ���̺귯�� (iocp�� boost �� �ϳ��� ���� �ȴ�.)
// #include "boost_asio.h"

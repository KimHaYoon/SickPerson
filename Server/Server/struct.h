#pragma once

struct OVER_EX {
	WSAOVERLAPPED	overlapped;
	WSABUF					dataBuffer;
	char							messageBuffer[MAX_BUF_SIZE];
	EVENT_TYPE			event;
};

struct PLAYER_INFO {
	SOCKET			socket;
	unsigned int		id;
	bool					connected;
	OVER_EX			over;
	char					packet_buf[MAX_BUF_SIZE];
	int					prev_size;
};
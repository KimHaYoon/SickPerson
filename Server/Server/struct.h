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

typedef struct Position2d{
	short x;
	short y;

	Position2d(short _x, short _y) : x(_x), y(_y) {}
}POS2D, PPOS2D;

typedef struct Position3d {
	short x;
	short y;
	short z;

	Position3d(short _x, short _y, short _z) : x(_x), y(_y), z(_z) {}
}POS3D, PPOS3D;

struct T_EVENT {
	int do_object;
	EVENT_TYPE event_type;
	high_resolution_clock::time_point	start_time;
	int from_object;

	constexpr bool operator <(const T_EVENT& _Left) const {
		return (start_time > _Left.start_time);
	}
};
#pragma once
#include "pch.h"
#include <winsock2.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
using namespace std;

class client {
private:
	WSAData wsa_data;
	WORD wsa_version;
	SOCKET active_socket;
	sockaddr_in socket_hint;
	bool alive = false;
public:
	string hostname2ip(string host);
	void data_parser(string type, int size, string command);
	bool startup_socket(string ip_address, int port, WSADATA &wsa_data, WORD &version, SOCKET &active, sockaddr_in &socket_hint);
	bool connect_socket(SOCKET &active, sockaddr_in &hint);
	bool connect_socket();
	bool check_error(int status);
	bool get_alive();
	bool send_data(string data = "", string header = "RESP");
	bool recv_data(string &output);
	void endme();
	client(string ip, int port);
};

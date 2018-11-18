#include "pch.h"
#include "client-interface.h"

int main()
{
	client default_client("472jens.zapto.org", 1337, "CDLXXII");
	default_client.connect_socket();
	while (default_client.get_alive()) { // recv_data recurv calls send_data that recurv calls recv data lmao
		string output;
		default_client.recv_data(output);
		//default_client.endme();
	}
}
#include "pch.h"
#include "client-interface.h"

int main()
{
	client default_client("472jens.zapto.org", 1337, "CDLXXII");
	default_client.connect_socket();
	while (default_client.get_alive_client()) {
		string otp;
		default_client.recv_data(otp);
	}
}
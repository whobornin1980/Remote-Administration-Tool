#include "pch.h"
#include "client-interface.h"

int main()
{
	client default_client("472jens.zapto.org", 1337);
	default_client.connect_socket();
	while (default_client.get_alive()) {
		cout << default_client.get_alive() << endl;
		string output = "commit die plz";
		default_client.recv_data(output);
		default_client.endme();
	}
	cout << "commit die" << endl;
	cin.get();
}
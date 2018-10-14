#include <signal.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "gateway.h"

int main_cpp(std::vector<std::string>& args)
{

    std::string username, key;
    {

		std::ifstream ifstream("/secrets/gateway");
		std::string gateway_key((std::istreambuf_iterator<char>(ifstream)),
                                 std::istreambuf_iterator<char>());
		std::cout << "factory user of the gateway\n"
                  << "  username: Client_identity\n"
                  << "  key:      " << gateway_key << "\n";
        
		auto connection(connect(gateway_key));
        username = connection.first;
        key      = connection.second;
        
		std::cout << "registered new user with the gateway\n"
                  << "  username: " << username  << "\n"
                  << "  key:      " << key << "\n";

    }

	return EXIT_SUCCESS;

}

int main(int argc, char *argv[])
{
	
	auto signal_handler(
		[](int signum)
		{
			std::cout << "Received " << strsignal(signum) << std::endl;
		}
	);

	struct sigaction signal_action = {};
	signal_action.sa_handler = signal_handler;
	sigaction(SIGINT, &signal_action, NULL);
	sigaction(SIGTERM, &signal_action, NULL);

	std::vector<std::string> args;
	for (int i = 1; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	auto return_code(main_cpp(args));

	std::cout.flush();
	
	return return_code;

}
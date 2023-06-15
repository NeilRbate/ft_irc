#include "../include/Server.hpp"

int	main(int argc, char **argv) {


	if (argc != 3) {
		std::cerr << "Error: invalid number of arguments !" << std::endl;
		return (1);
	}

	std::string	port(argv[1]), password(argv[2]);
	if (port.empty() || password.empty() || \
		port.find_first_not_of("0123456789") != std::string::npos) {
		std::cerr << "Error: invalid arguments !" << std::endl;
		return (1);
	}

	Server	server(stoi(port), password);

	server.getChannels().insert(std::pair<std::string, Channel>("#general", Channel("#general")));

	server.createSocket();
	server.bindSocket();
	server.selectSockets();

	std::cout << server.getServerSocketFd() << std::endl;

	return (0);
}

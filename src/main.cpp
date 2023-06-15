#include "../include/irc.hpp"

int	main(int argc, char **argv) {


	if (argc != 3) {
		std::cerr << "Error: invalid number of arguments !" << std::endl;
		return (1);
	}
	std::string	port(argv[1]), password(argv[2]);
	if (port.empty() || password.empty() || \
		port.find_first_not_of("0123456789") != std::string::npos) {
		std::cerr << "Error: invalid arguments !" << std::endl;
	}

	Server	server(stoi(port), password);

	server.createSocket();
	server.bindSocket();

	std::cout << server.getSocketFd() << std::endl;

	return (0);
}

#include "Server.hpp"

int Server::port;
int Server::exited = 0;
std::string Server::password;
std::string Server::name = "Barba-Chat";
std::vector<int> Server::fds;
std::vector<Channel> Server::channels;
std::vector<User> Server::users;
sockaddr_in Server::addr;
fd_set Server::read_fd_set;

std::string lower(std::string const & s) {
	std::string output;
	for (size_t i = 0; i < s.length(); i++) {
		output += tolower(s[i]);
	}
	return output;
}

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

	Server::port = stoi(port);
    Server::password = password;


	Server::createSocket();
	Server::bindSocket();
	std::cout << MAGENTA << "Listening with fd " <<Server::getServerSocketFd() << RESET << std::endl;
	Server::selectSocket();
	close(Server::fds[0]);
	std::cout << MAGENTA << "Closing connection " << RESET << std::endl;
	return (0);
}

#include "../include/irc.hpp"

//###Constructor###//
Server::Server( int port, std::string password ) {
	
	this->_port = port;
	this->_password = password;

	return ;
}

//###Destructor###//
Server::~Server() {

	return ;
}

void	Server::createSocket( void ) {

	
	try {
		this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	}
	catch (std::exception & e) {
		std::cout << "Error: " << e.what() << std::endl;
		return ;
	}
}

void	Server::bindSocket( void ) {

	return ;
}

int	Server::getSocketFd( void ) const {

	return (this->_socketFd);
}

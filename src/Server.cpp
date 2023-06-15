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

	bzero((char*) &this->_address, sizeof(this->_address));
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);

	try {
		bind(this->_socketFd, (struct sockaddr *)&this->_address, sizeof(this->_address));
	}
	catch (std::exception & e) {
		std::cout << "Error bind : " << e.what() << std::endl;
		return ;
	}
	if(listen(this->_socketFd, 10) < 0)
		std::cerr << "Listen error" << std::endl;
	socklen_t	addresslen = sizeof(this->_address);
	int sockfd = accept(this->_socketFd, (struct sockaddr *) &this->_address, &addresslen);

	send(sockfd, "Hello world !\n", 14, 0);

	std::cout << sockfd << std::endl;

	return ;
}

int	Server::getSocketFd( void ) const {

	return (this->_socketFd);
}

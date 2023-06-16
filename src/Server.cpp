#include "../include/Server.hpp"

//###Constructor###//
Server::Server( int port, std::string password ) : _name("Barba-chat") {
	
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
		this->_connections.push_back(socket(AF_INET, SOCK_STREAM, 0));
	}
	catch (std::exception & e) {
		std::cout << "Error: " << e.what() << std::endl;
		return ;
	}
}

void	Server::bindSocket( void ) {
	bzero((char*) &this->_server_addr, sizeof(this->_server_addr));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(this->_port);

	try {
		bind(this->getServerSocketFd(), (struct sockaddr *)&this->_server_addr, sizeof(this->_server_addr));
	}
	catch (std::exception & e) {
		std::cout << "Error bind : " << e.what() << std::endl;
		return ;
	}
	if (listen(this->getServerSocketFd(), FD_SETSIZE) < 0)
		std::cerr << "Listen error" << std::endl;
}

void	Server::selectSockets( void ) {
	int select_output;
	
	while (true) {
		FD_ZERO(&this->_read_fd_set);
		for (size_t i = 0; i < this->_connections.size(); i++) {
			FD_SET(this->_connections[i], &this->_read_fd_set);
		}

		select_output = select(FD_SETSIZE, &this->_read_fd_set, NULL, NULL, NULL);

		if (select_output < 0) return ;
		if (FD_ISSET(this->getServerSocketFd(), &this->_read_fd_set))
			newConnection();
		else {
			for (size_t i = 0; i < this->_connections.size(); i++) {
				if (FD_ISSET(this->_connections[i], &this->_read_fd_set))
					readInput(this->_connections[i]);
			}
		}
	}
}

void	Server::newConnection( void ) {
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);

	bzero((char*) &client_addr, addr_len);
	int client_fd = accept(this->getServerSocketFd(), (struct sockaddr*)&client_addr, &addr_len);
	if (client_fd >= 0) {
		this->_connections.push_back(client_fd);
		this->_users.insert(std::pair<int, User>(client_fd, User(client_fd, this)));
		std::cout << "New connection with client fd: " << client_fd << std::endl;
	} else {
		std::cout << "Failed to connect new client" << std::endl;
	}
}

void	Server::readInput( int client_fd ) {
	int output = 0;
	char buffer[256];
	std::string msg;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	bzero(buffer, 256);
	output = recv(client_fd, buffer, 255, 0);
	if (output <= 0) {
		std::cout << "Connection closed with client fd:" << client_fd << std::endl;
		close(client_fd);
		std::vector<int>::iterator index = std::find(this->_connections.begin(), this->_connections.end(), client_fd);
		this->_connections.erase(index);
		this->_users.erase(client_fd);
		return ;
	}
	while (output > 0) {
		msg += buffer;
		bzero(buffer, 256);
		output = recv(client_fd, buffer, 255, 0);
	}
	if (msg[0] == '/') {
		std::cout << "Command: " << msg << std::endl;
	} else {
		User user = this->_users.at(client_fd);
		if (user.getChannel() == "") {
			std::cout << "User not in channel" << std::endl;
			return ;
		}
		Channel &channel = this->_channels.at(user.getChannel());
		channel.sendMsgFromUser(msg, user);
	}
}

std::string	Server::getName( void ) const {
	return (this->_name);
}

int	Server::getServerSocketFd( void ) const {
	return (this->_connections[0]);
}

std::map<int, User> Server::getUsers( void ) const {
	return (this->_users);
}

std::map<std::string, Channel> Server::getChannels( void ) const {
	return (this->_channels);
}

void	Server::setChannel(std::string const & name, Channel *channel) {
	
	this->_channels.insert(std::pair<std::string, Channel>(name, channel));
}

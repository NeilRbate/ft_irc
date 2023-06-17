#include "Server.hpp"

void	Server::createSocket( void ) {
	try {
		Server::fds.push_back(socket(AF_INET, SOCK_STREAM, 0));
	}
	catch (std::exception & e) {
		std::cout << "Error: " << e.what() << std::endl;
		return ;
	}
}

void	Server::bindSocket( void ) {
	bzero((char*) &Server::addr, sizeof(Server::addr));
	Server::addr.sin_family = AF_INET;
	Server::addr.sin_addr.s_addr = INADDR_ANY;
	Server::addr.sin_port = htons(Server::port);

	if (bind(Server::getServerSocketFd(), (struct sockaddr *)&Server::addr, sizeof(Server::addr))) {
		std::cout << "Error bind port: " << strerror(errno) << std::endl;
		exit(1);
	}
	if (listen(Server::getServerSocketFd(), FD_SETSIZE)) {
		std::cout << "Error bind port: " << strerror(errno) << std::endl;
		exit(1);
	}
}

void signal_callback_handler(int) {
	Server::exited = 1;
	std::cout << std::endl << "=== Good Bye ! ===" << std::endl;
}

void	Server::selectSocket( void ) {
	int select_output;
	
	signal(SIGINT, signal_callback_handler);

	while (!Server::exited) {
		FD_ZERO(&Server::read_fd_set);
		for (size_t i = 0; i < Server::fds.size(); i++) {
			FD_SET(Server::fds[i], &Server::read_fd_set);
		}

		select_output = select(FD_SETSIZE, &Server::read_fd_set, NULL, NULL, NULL);

		if (Server::exited)
			return ;

		if (select_output < 0) {
			std::cerr << "Select error" << std::endl;
			return ;
		}
		if (FD_ISSET(Server::getServerSocketFd(), &Server::read_fd_set)) {
			newConnection();
			continue ;
		}
		
		for (size_t i = 0; i < Server::fds.size(); i++) {
			if (!FD_ISSET(Server::fds[i], &Server::read_fd_set)) 
				continue ;
			
			for (std::vector<User>::iterator it = Server::users.begin(); it != Server::users.end(); it++) {
				if (it->getFd() == Server::fds[i]) {
					readInput(*it);
					break ;
				}
			}
		}
	}
}

void	Server::newConnection( void ) {
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);

	bzero((char*) &client_addr, addr_len);
	int client_fd = accept(Server::getServerSocketFd(), (struct sockaddr*)&client_addr, &addr_len);
	if (client_fd >= 0) {
		Server::fds.push_back(client_fd);
		Server::users.push_back(User(client_fd));
		std::cout << "New connection with client fd: " << client_fd << std::endl;
	} else {
		std::cout << "Failed to connect new client" << std::endl;
	}
}

void	Server::readInput( User & user ) {
	int output = 0;
	char buffer[512];

	fcntl(user.getFd(), F_SETFL, O_NONBLOCK);
	bzero(buffer, 512);
	output = recv(user.getFd(), buffer, 512, 0);
	if (output <= 0) {
		std::cout << "Connection closed with client fd:" << user.getFd() << std::endl;
		close(user.getFd());
		std::vector<int>::iterator index = std::find(Server::fds.begin(), Server::fds.end(), user.getFd());
		Server::fds.erase(index);
		std::vector<User>::iterator it = std::find(Server::users.begin(), Server::users.end(), user);
		Server::users.erase(it);
		return ;
	}
	user.input += buffer;

	while (user.input.find("\r\n") != std::string::npos) {
		std::string cmd = user.input.substr(0, user.input.find("\r\n"));
		user.input = user.input.substr(user.input.find("\r\n") + 2);
		executeCommand(user, cmd);
	}
}


//###Cmd functions###//

void	nick(User & user, std::string & cmd) {
	std::string	temp = cmd.substr(5, cmd.size() - 5);
	if (temp.find_first_of("#$:") != std::string::npos || isdigit(temp.at(0)) == true) {
		user.sendMsg(":" + Server::name + " 432 " + user.nickName + " :Invalid nickname\r\n");
		return ;
	}
	for (std::vector<User>::iterator it = Server::users.begin(); it != Server::users.end(); it++) 
		if (it->getNickName() == temp) {
			user.sendMsg(":" + Server::name + " 433 " + user.nickName + ": " + temp + "\r\n");
			return ;
		}
	if (user.nickName.empty())
		user.nickName = temp;
	else {
		user.sendMsg(":" + user.nickName + " NICK " + temp + "\r\n");
		user.nickName = temp;
	}
}

void	sendPrivMsg( User & user, std::string & cmd ) {

	size_t	point = cmd.find(":");
	size_t	space = cmd.find(" ");
	if (point <= space)
		return;
	std::string	chan = cmd.substr(space + 1, ((point - 1) - (space + 1)));
	std::string	msg = cmd.substr(point + 1, cmd.size() - (point + 1));
	std::cout << "channel ->" << chan << "|"  <<std::endl;
	std::cout << "message ->" << msg << "|"  <<std::endl;
	if (msg.at(0) == '#') {
		for (std::vector<Channel>::iterator it = Server::channels.begin(); it != Server::channels.end(); it++) 
			if (it->getName() == chan) {
				std::vector<std::string> chanUsers = it->getAuthorizedUsers();
				if (std::find(chanUsers.begin(), chanUsers.end(), user.nickName) != chanUsers.end()) {
					it->sendMsgFromUser(":" + user.nickName + " PRIVMSG " + chan + " : " + msg + "\r\n", user);
					return ;
				}
				user.sendMsg(":" + Server::name + " 404 " + chan + ": Cannot send to CHANNEL\r\n");
				return ;
		}
		user.sendMsg(":" + Server::name + " 403 " + chan + ": No such CHANNEL\r\n");
	} else {
		for (std::vector<User>::iterator it = Server::users.begin(); it != Server::users.end(); it++) 
			if (it->getNickName() == chan) {
				it->sendMsg(":" + user.nickName + " PRIVMSG " + it->nickName + " : " + msg + "\r\n");
				return ;
		}
		user.sendMsg(":" + Server::name + " 401 " + user.nickName + ": No such NICK\r\n");
	}
}

void	Server::executeCommand( User & user, std::string & cmd ) {

	std::cout << "cmd -> " << cmd << std::endl;

	if (cmd.find("CAP") == 0)
		return ;
	else if (cmd.find("PING") == 0 && cmd.size() > 5 && cmd.find(Server::name) == 5)
			user.sendMsg("PONG\r\n");
	else if (cmd.find("NICK") == 0 && cmd.size() > 5 && cmd.at(4) == ' ')
		return nick(user, cmd);
	else if (cmd.find("USER") == 0 && cmd.size() > 5 && user.userName.empty()) {
		user.userName = cmd.substr(5, cmd.size());
		std::ifstream file("asset/motd.txt");
		std::string text;
		std::string line;
		while (std::getline(file, line))
			text += line + "\n";
		user.sendMsg(":" + Server::name + " 001 " + user.nickName + " :" + text + "\r\n");
	}
	else if (cmd.find("PASS") == 0 && cmd.size() > 5) {
		if (user.getIsAuth() == true)
			user.sendMsg(Server::name +  " : You're already auth !\r\n");
		else if (cmd.substr(5, cmd.size()) == Server::password)
			user.setIsAuth(true);
		else
			user.sendMsg(":" + Server::name + " 464 " + user.getNickName() +  " :Password Incorrect\r\n");
	} 
	else if (user.getIsAuth() == false)
		user.sendMsg(":" + Server::name + " 464 " + user.getNickName() +  " :You're no authentify !\r\n");
	else if (cmd.find("PRIVMSG") == 0 && user.getIsAuth() == true)
		sendPrivMsg(user, cmd);
}

int		Server::getServerSocketFd( void ) {
	return (Server::fds[0]);
}

void	Server::addChannel(std::string const & name) {
    
    for (std::vector<Channel>::iterator it = Server::channels.begin(); it != Server::channels.end(); it++) {
        if (it->getName() == name) {
            std::cout << "Channel already exist" << std::endl;
            return ;
        }
    }

    Server::channels.push_back(Channel(name));
}

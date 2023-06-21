#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

User::User(int fd) : fd(fd), isAuth(false) { }
User::~User() { }

int User::getFd() const { return this->fd; }
bool User::getIsAuth() const { return this->isAuth; }
bool User::getIsOperator() const { return this->isOperator; }
std::string User::getNickName() const { return this->nickName; }
std::string User::getUserName() const { return this->userName; }
std::string User::getRealName() const { return this->realName; }

void User::setIsAuth(bool isAuth) { this->isAuth = isAuth; }

void User::sendMsg(std::string msg) const {
	if (send(this->fd, msg.c_str(), msg.length(), 0) < 0)
		std::cout << RED << "ERROR: impossible to send" << RESET << std::endl;
  else
    std::cout << YELLOW << "[fd: " << this->getFd() << " (" << this->getNickName() << ")] " << RESET << msg;
}

void User::closeConnection() {
  std::cout << MAGENTA << "Connection closed with client fd: " << this->getFd() << RESET << std::endl;
  close(this->getFd());
  std::vector<int>::iterator index = std::find(Server::fds.begin(), Server::fds.end(), this->getFd());
  Server::fds.erase(index);
  std::vector<User>::iterator it = std::find(Server::users.begin(), Server::users.end(), *this);
  Server::users.erase(it);
}

void	User::quitAllChannel( void ) {

	std::vector<Channel>::iterator it;
	for (it = Server::channels.begin(); it != Server::channels.end(); it++) {
		std::vector<User *>::iterator user = std::find(it->users.begin(), it->users.end(), this);
		if (user != it->users.end() && (*user)->getFd() == this->getFd()){
			it->sendMsg(":" + this->getNickName() + " PART " + it->getName() + "\r\n");
			it->users.erase(user);
		}
	}

}

void	User::leaveChannel(std::vector<std::string> const & cmd) {
	if (cmd.size() != 2) {
    this->sendMsg(":" + this->getNickName() + " 461 :Not Enough Parameters\r\n");
    return ;
  }
	if (cmd[1][0] != '#') {
    this->sendMsg(":" + Server::name + " 403 " + this->nickName + " " + lower(cmd.at(1)) + " :No such channel\r\n");
    return ;
  }
	std::vector<Channel>::iterator it;
	for (it = Server::channels.begin(); it != Server::channels.end(); it++) {
		if (it->getName() == lower(cmd.at(1))) {
			std::vector<User *>::iterator user = std::find(it->users.begin(), it->users.end(), this);
      if (user == it->users.end()) {
        this->sendMsg("442 " + it->getName() + " :You're not on that channel\r\n");
        return ;
      }
      it->sendMsg(":" + this->getNickName() + " PART " + it->getName() + "\r\n");
      it->users.erase(user);
    }
	}
}

void User::joinChannel(std::vector<std::string> const & cmd) {

  if (cmd.size() < 2) {
    this->sendMsg(":" + this->getNickName() + " 461 :Not Enough Parameters\r\n");
	  return ;
  }

	if (cmd.size() > 2 || cmd[1][0] != '#'){
    this->sendMsg(":" + lower(cmd.at(1)) + " 476 :Bad Channel Mask\r\n");
	  return ;
	}
	std::string	name(lower(cmd.at(1)));

	std::vector<Channel>::iterator it;
	for (it = Server::channels.begin(); it != Server::channels.end(); it++) {
    if (it->getName() == name) {
      it->users.push_back(this);
      
      // JOIN message
      it->sendMsg(":" + this->getNickName() + " JOIN " + name + "\r\n");

      // RPL_TOPIC
      if (it->topic != "")
        this->sendMsg(":" + Server::name + " 332 " + this->getNickName() + " " + name + " :" + it->topic + "\r\n");

      // RPL_NAMREPLY
	  std::string userList;
      std::vector<User *>::iterator it2;
      for (it2 = it->users.begin(); it2 != it->users.end(); it2++) {
        if (it2 != it->users.end() && it2 != it->users.begin())
          userList += " ";
        if (it->isOperator((*it2)->getNickName())) {
          userList += "@";
        }
        userList += (*it2)->getNickName();
      }
      this->sendMsg("353 " + this->getNickName() + " = " + name + " :" + userList + "\r\n");
      this->sendMsg("366 " + this->getNickName() + " " + name + " :End of /NAMES list.\r\n");
      return ;
    }
  }
  Server::addChannel(name, this->getNickName());

  this->joinChannel(cmd);
}

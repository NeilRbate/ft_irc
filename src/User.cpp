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
  send(this->fd, msg.c_str(), msg.length(), 0);
}

/*void	User::leaveChannel(std::vector<std::string> const & cmd) {

	if (cmd.size() != 2)
      it->sendMsg(":" + this->getNickName() + " 461 :Not Enough Parameters\r\n");
	if (cmd[1][0] != #)
      it->sendMsg(":" + this->getNickName() + " " + cmd.at(1) + " 403 :No such channel\r\n");
	std::vector<Channel>::iterator it;
	for (it = Server::channels.begin(); it != Server::channels.end(); it++) {
		if (it->getName() == cmd.at(1))

			
			
	}

	
}*/

void User::joinChannel(std::vector<std::string> const & cmd) {

	if (cmd.size() > 2 || cmd[1][0] != '#'){
      this->sendMsg(":" + cmd.at(1) + " 476 :Bad Channel Mask\r\n");
	  return ;
	}
	std::string	name(cmd.at(1));

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
      std::string users;
      std::vector<User *>::iterator it2;
      for (it2 = it->users.begin(); it2 != it->users.end(); it2++) {
        if (it2 != it->users.begin())
          users += " ";
        users += (*it2)->getNickName();
      }
      this->sendMsg(":" + Server::name + " 353 " + this->getNickName() + " = " + name + " :" + users + "\r\n");
      this->sendMsg(":" + Server::name + " 366 " + this->getNickName() + " " + name + " :End of /NAMES list.\r\n");
      return ;
    }
  }
  Server::addChannel(name);
  this->joinChannel(cmd);
}

#include "Channel.hpp"

Channel::Channel(std::string name, std::string operatorName) : isInviteOnly(false), name(lower(name)), topic("Welcome to channel " + name) {
  this->operatorUsers.push_back(operatorName);
}
Channel::~Channel() { }

std::string Channel::getName() const { return this->name; }

void Channel::sendMsg(std::string msg) const {
  std::vector<User *>::const_iterator it;
  if (users.size() > 0) {
	  for (it = users.begin(); it != users.end(); it++)
		  (*it)->sendMsg(msg);
  }
}

void Channel::sendMsgFromUser(std::string msg, User &user) const {
  std::vector<User *>::const_iterator it;
  for (it = users.begin(); it != users.end(); it++) {
    if ((*it)->getFd() == user.getFd())
      continue ;
    (*it)->sendMsg(msg);
  }
}

bool Channel::isOperator(std::string nickName) {
  return (std::find(this->operatorUsers.begin(), this->operatorUsers.end(), nickName) != this->operatorUsers.end());
}
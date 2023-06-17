#include "Channel.hpp"

Channel::Channel(std::string name) : isInviteOnly(false), name(name) { }
Channel::~Channel() { }

std::string Channel::getName() const { return this->name; }
bool Channel::getIsInviteOnly() const { return this->isInviteOnly; }
std::vector<User *> Channel::getUsers() { return this->users; }
std::vector<std::string> Channel::getAuthorizedUsers() { return this->authorizedUsers; }

void Channel::setIsInviteOnly(bool isInviteOnly) { this->isInviteOnly = isInviteOnly; }

void Channel::sendMsg(std::string msg) const {
    std::vector<User *>::const_iterator it;
    for (it = users.begin(); it != users.end(); it++)
        (*it)->sendMsg(msg);
}

void Channel::sendMsgFromUser(std::string msg, User &user) const {
    std::vector<User *>::const_iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if ((*it)->getFd() == user.getFd())
            continue ;
        (*it)->sendMsg(msg);
    }
}

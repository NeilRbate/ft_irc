#include "../include/Channel.hpp"
#include "../include/User.hpp"
#include "../include/Server.hpp"

Channel::Channel(std::string name) : isInviteOnly(false), name(name){ }
Channel::~Channel() { }

std::string Channel::getName() const { return this->name; }
bool Channel::getIsInviteOnly() const { return this->isInviteOnly; }
std::vector<std::string> Channel::getAuthorizedUsers() const { return this->authorizedUsers; }

void Channel::setIsInviteOnly(bool isInviteOnly) { this->isInviteOnly = isInviteOnly; }

void Channel::sendMsg(std::string msg) const {
    for (size_t i = 0; i < Server->getUsers().size(); i++) {
        if (Server->getUsers().at(i).getChannel() == this->name)
            Server->getUsers().at(i).sendMsg(msg);
    }
}

void Channel::sendMsgFromUser(std::string msg, User &user) const {
    for (size_t i = 0; i < Server->getUsers().size(); i++) {
        if (Server->getUsers().at(i).getChannel() == this->name && Server->getUsers().at(i).getNickName() != user.getNickName())
            Server->getUsers().at(i).sendMsg(msg);
    }
}
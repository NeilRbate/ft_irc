#include "../include/Channel.hpp"
#include "../include/User.hpp"
#include "../include/Server.hpp"

Channel::Channel(std::string name, Server *server) : isInviteOnly(false), name(name), server(server){ }
Channel::~Channel() { }

std::string Channel::getName() const { return this->name; }
bool Channel::getIsInviteOnly() const { return this->isInviteOnly; }
std::vector<std::string> Channel::getAuthorizedUsers() const { return this->authorizedUsers; }

void Channel::setIsInviteOnly(bool isInviteOnly) { this->isInviteOnly = isInviteOnly; }

void Channel::sendMsg(std::string msg) const {
    for (size_t i = 0; i < server->getUsers().size(); i++) {
        if (server->getUsers().at(i).getChannel() == this->name)
            server->getUsers().at(i).sendMsg(msg);
    }
}

void Channel::sendMsgFromUser(std::string msg, User &user) const {
    for (size_t i = 0; i < server->getUsers().size(); i++) {
        if (server->getUsers().at(i).getChannel() == this->name && server->getUsers().at(i).getNickName() != user.getNickName())
            server->getUsers().at(i).sendMsg(msg);
    }
}

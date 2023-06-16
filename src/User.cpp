#include "../include/User.hpp"
#include "../include/Channel.hpp"
#include "../include/Server.hpp"

User::User(int fd, Server *server) : fd(fd), isAuth(false), channel("#general"), server(server) { }
User::~User() { }

int User::getFd() const { return this->fd; }
bool User::getIsAuth() const { return this->isAuth; }
bool User::getIsOperator() const { return this->isOperator; }
std::string User::getChannel() const { return this->channel; }
std::string User::getNickName() const { return this->nickName; }
std::string User::getUserName() const { return this->userName; }
std::string User::getRealName() const { return this->realName; }

void User::setIsAuth(bool isAuth) { this->isAuth = isAuth; }

void User::sendMsg(std::string msg) const {
    send(this->fd, msg.c_str(), msg.length(), 0);
}

void User::changeChannel(std::string channel) {
    if (server->getChannels().count(this->channel)) {
        sendMsg(":" + server->getName() + " 403 " + this->nickName + " " + this->channel + " :No such channel\r\n");
        return;
    }

    this->channel = channel;

    Channel &chan = server->getChannels().at(channel);

   if (chan.getIsInviteOnly() && std::find(chan.getAuthorizedUsers().begin(), chan.getAuthorizedUsers().end(), this->nickName) == chan.getAuthorizedUsers().end()) {
        sendMsg(":" + server->getName() + " 473 " + this->nickName + " " + this->channel + " :Cannot join channel (+i)\r\n");
        return;
    }

    sendMsg(":" + this->nickName + " JOIN " + this->channel + "\r\n");
}

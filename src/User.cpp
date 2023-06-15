#include "../include/User.hpp"
#include "../include/Channel.hpp"
#include "../include/Server.hpp"

User::User(int fd) : fd(fd), isAuth(false), channel("#general") { }
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
    if (!Server::channels.count(this->channel)) {
        sendMsg(":" + Server::serverName + " 403 " + this->nickName + " " + this->channel + " :No such channel\r\n");
        return;
    }

    this->channel = channel;

    Channel &chan = Server::channels.at(channel);

    if (chan.getIsInviteOnly() && std::find(chan.getAuthorizedUsers().begin(), chan.getAuthorizedUsers().end(), this->nickName) == chan.getAuthorizedUsers().end()) {
        sendMsg(":" + Server::serverName + " 473 " + this->nickName + " " + this->channel + " :Cannot join channel (+i)\r\n");
        return;
    }

    sendMsg(":" + this->nickName + " JOIN " + this->channel + "\r\n");
}

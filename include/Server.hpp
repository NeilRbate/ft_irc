#pragma once

#include "irc.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Server {
public:
    static void createSocket();
    static void bindSocket();
    static void selectSocket();
    static void newConnection();
    static void readInput(int clientFd);

    static void addChannel(std::string const & name);

    static int getServerSocketFd();

    static int port;
    static std::string password;
    static std::string name;

    static std::vector<int> fds;
    static std::vector<Channel> channels;
    static std::vector<User> users;

    static sockaddr_in addr;
    static fd_set read_fd_set;
};

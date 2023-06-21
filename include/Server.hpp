#pragma once

#include <string>
#include <sstream>

#include "irc.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Server {
public:
  static void createSocket();
  static void bindSocket();
  static void selectSocket();
  static void newConnection();
  static void readInput(User & user);
  static bool executeCommand(User & user, std::string & cmd);
  static void addChannel(std::string const & name, std::string const & operatorName);

  static int getServerSocketFd();

  static int port;
  static int exited;
  static std::string password;
  static std::string name;

  static std::vector<int> fds;
  static std::vector<Channel> channels;
  static std::vector<User> users;

  static sockaddr_in addr;
  static fd_set read_fd_set;
};

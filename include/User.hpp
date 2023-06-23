#pragma once

#include "irc.hpp"

class User {
public:
  User(int fd);
  ~User();

  int getFd() const;
  bool getIsAuth() const;
  bool getIsOperator() const;
  std::string getNickName() const;
  std::string getUserName() const;
  std::string getRealName() const;

  void setIsAuth(bool isAuth);

  void sendMsg(std::string msg) const;
  void joinChannel(std::string name);
  void leaveChannel(std::vector<std::string> const & cmd);
  void kickChannel(std::vector<std::string> const & cmd, std::string const & rawcmd);
  void closeConnection();
  void topic(std::vector<std::string> cmd, std::string rawcmd);
  void invite(std::vector<std::string> cmd, std::string rawcmd);
  void quitAllChannels( void );
  void mode(std::vector<std::string> const & cmd, std::string const & rawcmd);

  std::string input;

  int fd;
  bool isAuth;
  bool isOperator;
  std::string nickName;
  std::string userName;
  std::string realName;

  bool operator==(const User & other) const {
    return (this->fd == other.fd);
  }
};

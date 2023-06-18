#pragma once

#include "irc.hpp"
#include "User.hpp"

class   Channel {
public:
  Channel(std::string name);
  ~Channel();

  std::string getName() const;

  void sendMsg(std::string msg) const;
  void sendMsgFromUser(std::string msg, User &user) const;

  bool isInviteOnly;
  const std::string name;
  std::string topic;
  std::vector<User*> users;
  std::vector<std::string> operatorUsers;
};
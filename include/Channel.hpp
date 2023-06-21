#pragma once

#include "irc.hpp"
#include "User.hpp"

class   Channel {
public:
  Channel(std::string name, std::string operatorName);
  ~Channel();

  std::string getName() const;

  void sendMsg(std::string msg) const;
  void sendMsgFromUser(std::string msg, User &user) const;

  bool isOperator(std::string nickName);

  bool isInviteOnly;
  const std::string name;
  std::string topic;
  std::vector<User*> users;
  std::vector<std::string> operatorUsers;
};
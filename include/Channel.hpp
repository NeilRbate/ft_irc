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
  void changeTopic(std::string cmd);

  bool isOperator(std::string nickName);

  void	setInviteOnly();
  void	setTopicAccess();

  bool isInviteOnly;
  bool isTopicFree;
  const std::string name;
  std::string topic;
  std::vector<User*> users;
  std::vector<std::string> operatorUsers;
};

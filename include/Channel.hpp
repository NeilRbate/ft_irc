#pragma once

#include "irc.hpp"
#include "User.hpp"

class   Channel {
    public:
        Channel(std::string name);
        ~Channel();
    
        std::string getName() const;
        bool getIsInviteOnly() const;

        std::vector<User*> getUsers();
        std::vector<std::string> getAuthorizedUsers();
        
        void setIsInviteOnly(bool isInviteOnly);

        void sendMsg(std::string msg) const;
        void sendMsgFromUser(std::string msg, User &user) const;

    private:
        bool isInviteOnly;
        const std::string name;
        std::vector<User*> users;
        std::vector<std::string> authorizedUsers;
};
#ifndef __CHANNEL_HPP
# define __CHANNEL_HPP
#include "irc.hpp"
#include "User.hpp"
#include "Server.hpp"

class	User;
class	Server;

class   Channel {
    public:
        Channel(std::string name, Server *server);
        ~Channel();
    
        std::string getName() const;
        bool getIsInviteOnly() const;
        std::vector<std::string> getAuthorizedUsers() const;
        
        void setIsInviteOnly(bool isInviteOnly);

        void sendMsg(std::string msg) const;
        void sendMsgFromUser(std::string msg, User &user) const;

    private:

        bool isInviteOnly;
        const std::string name;
        std::vector<std::string> authorizedUsers;
		Server	*server;
};

#endif

#ifndef __USER_HPP
# define __USER_HPP
#include "irc.hpp"
#include "Server.hpp"
class	Server;

class   User {
    public:
        User(int fd, Server *server);
        ~User();

        int getFd() const;
        bool getIsAuth() const;
        bool getIsOperator() const;
        std::string getChannel() const;
        std::string getNickName() const;
        std::string getUserName() const;
        std::string getRealName() const;

        void setIsAuth(bool isAuth);
        void changeChannel(std::string channel);

        void sendMsg(std::string msg) const;

    private:
        const int fd;
        bool isAuth;
        bool isOperator;
        std::string channel;   
        std::string nickName;
        std::string userName;
        std::string realName;
		Server	*server;
};

#endif

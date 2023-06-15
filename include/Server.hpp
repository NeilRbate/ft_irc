#ifndef __SERVER_HPP
# define __SERVER_HPP
#include "User.hpp"
#include "Channel.hpp"

class	User;

class	Server {

	public:

		Server( int	port, std::string password );
		~Server( void );


		//###Member functions###//

		//Create a socket
		void	createSocket( void );
		void	bindSocket( void );
		void	selectSockets( void );
		void	newConnection( void );
		void	readInput( int client_fd );


		//###Getter###//
		int	getServerSocketFd( void ) const;
		std::map<int, User> getUsers( void ) const;
		std::map<std::string, Channel> getChannels( void ) const;


		//###Exception###//
		class	SocketException : public std::exception {
			virtual const char* what() const throw() {
				return ("Socket fail Exception");
			}
		};

	private:

		std::string	_password;
		int	_port;
		fd_set _read_fd_set;
		std::vector<int> _connections;
		std::map<int, User> _users;
		std::map<std::string, Channel> _channels;

		struct sockaddr_in _server_addr;

};

#endif

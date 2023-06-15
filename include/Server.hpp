#ifndef __SERVER_HPP
# define __SERVER_HPP
#include "irc.hpp"

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

		struct sockaddr_in _server_addr;
};

#endif

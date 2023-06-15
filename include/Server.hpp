#ifndef __SERVER_HPP
# define __SERVER_HPP

class	Server {

	public:

		Server( int	port, std::string password );
		~Server( void );


		//###Member functions###//

		//Create a socket
		void	createSocket( void );
		void	bindSocket( void );


		//###Getter###//
		int	getSocketFd( void ) const;

		//###Exception###//
		class	SocketException : public std::exception {
			virtual const char* what() const throw() {
				return ("Socket fail Exception");
			}
		};

	private:

		std::string	_password;

		int	_port;
		int	_socketFd;

		struct sockaddr_in _address;


};

#endif

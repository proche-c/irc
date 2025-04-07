#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
	private:
		int _socket;
		std::string	_address;
		std::vector<std::string>_args;
		std::string _rawArgs;
		std::string	_lastArg;
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		std::string _servername;
		std::string _realname;
		bool 		_isRegistered;
		std::string	_ServerCreationTime;
		bool		_isAuthenticated;
		std::string _buffer;
		bool		_nickIsSet;
		bool		_userIsSet;


	public:
		Client(void);
		Client(int sock);
		~Client();

		Client & operator=(Client const & src);

		int 		getSocket() const;
		void		setSocket(int fd);
		std::string	getAddress(void) const;
		void		setAddress(std::string address);

		std::string getNickName() const;
		void 		setNickName(const std::string &newNick);
		
		std::vector<std::string> getArgs() const;
		void 		setArgs(const std::vector <std::string> &arguments);

		std::string	getRawArg() const;
		void	setRawArg(const std::string &arguments);

		std::string	getLastArg() const;
		void		setLastArg(const std::string &arguments);
		
		void		setUsername(const std::string& username);
    	std::string const &getUsername() const;

    	void	setHostname(const std::string& hostname);
    	std::string const &getHostname() const;

    	void	setServername(const std::string& servername);
    	std::string const &getServername() const;

    	void	setRealname(const std::string& realname);
    	std::string const &getRealname() const;

        void	setServerCreationTime(std::string  time);
        std::string	getServerCreationTime(void) const;


		void setRegistered(bool isRegistered);
    	bool isRegistered() const;


		bool	isAuthenticated() const;
		void	setAuthenticated(bool isAuthenticated);

		std::string const &getBuffer() const;
		void	setBuffer(std::string const &buffer);
		void	cleanBuffer(void);

		bool	isNickSet() const;
		void	setNickSet(bool isSet);

		bool	isUserSet() const;
		void	setUserSet(bool isSet);
};

#endif

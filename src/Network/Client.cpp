#include "../../includes/Network/Client.hpp"


Client::Client(void): _socket(-1), _isRegistered(false), _isAuthenticated(false), _buffer(""),
	_nickIsSet(false), _userIsSet(false)
{
	std::cout << " Default Client object created" <<std::endl;
}

Client::Client(int sock): _socket(sock), _isRegistered(false), _isAuthenticated(false), _buffer(""),
	_nickIsSet(false), _userIsSet(false)
{
	std::cout << "Client object created" << std::endl;
}

Client::~Client()
{
	std::cout << "Client object destroyed" << std::endl;
}

Client & Client::operator=(Client const & src)
{
	if (this != &src)
	{
		_socket = src._socket;
		_address = src._address;
		_args = src._args;
		_nickname = src._nickname;
		_username = src._username;
		_hostname = src._hostname;
		_servername = src._servername;
		_realname = src._realname;
		_isRegistered = src._isRegistered;
	}
	return *this;
}

int Client::getSocket() const
{
	return(_socket);
}

std::string Client::getNickName() const
{
	return(_nickname);
}

void	Client::setSocket(int fd)
{
	_socket = fd;
}

std::string	Client::getAddress(void) const
{
	return (_address);
}

void	Client::setAddress(std::string address)
{
	_address = address;
}

void Client::setNickName(const std::string &newNick)
{
	_nickname = newNick;
}

//---------------------------Args

std::vector<std::string> Client::getArgs() const
{
	return _args;
}

void Client::setArgs(const std::vector<std::string> &arguments)
{
	_args = arguments;
}

std::string	Client::getRawArg() const
{
	return _rawArgs;
}

void	Client::setRawArg(const std::string &arguments)
{
	_rawArgs = arguments;
}

std::string	Client::getLastArg() const
{
	return _lastArg;
}

void	Client::setLastArg(const std::string &arguments)
{
    size_t  start = arguments.find(':');
    if (start == std::string::npos)
        _lastArg = "";
    else
        _lastArg = arguments.substr(start + 1);
}

//---------------------------Username

void Client::setUsername(std::string const &username)
{
	_username = username;
}

std::string const &Client::getUsername() const
{
	return _username;
}

//-------------------------Hostname

void Client::setHostname(std::string const &hostname) 
{ 
	_hostname = hostname; 
}

std::string const &Client::getHostname() const 
{
	return _hostname;
}

//-------------------------Servername

void Client::setServername(std::string const  &servername)
{
	_servername = servername;
}

std::string const &Client::getServername() const
{
	return _servername;
}

//-------------------------Realname

void Client::setRealname(std::string const &realname) 
{
	_realname = realname;
}

std::string  const &Client::getRealname() const 
{
	return _realname;
}

void        Client::setServerCreationTime(std::string time)
{
    _ServerCreationTime = time;
}

std::string Client::getServerCreationTime(void) const
{
    return _ServerCreationTime;
}

//------------------------------Register

void Client::setRegistered(bool isRegistered)
{ 
	_isRegistered = isRegistered;
}

bool Client::isRegistered() const 
{
	return _isRegistered;
}

bool Client::isAuthenticated() const
{
	return _isAuthenticated;
}

void Client::setAuthenticated(bool isAuthenticated)
{
	_isAuthenticated = isAuthenticated;
}

std::string const &Client::getBuffer() const
{
	return _buffer;
}

void	Client::setBuffer(std::string const &buffer)
{
	_buffer = _buffer + buffer;
}

void	Client::cleanBuffer(void)
{
	_buffer = "";
}

bool	Client::isNickSet() const
{
	return _nickIsSet;
}

void	Client::setNickSet(bool isSet)
{
	_nickIsSet = isSet;
}

bool	Client::isUserSet() const
{
	return _userIsSet;
}

void	Client::setUserSet(bool isSet)
{
	_userIsSet = isSet;
}
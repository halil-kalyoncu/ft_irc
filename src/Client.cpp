
#include "../include/Client.hpp"

Client::Client(void) : _nickName(""), _userName(""), _hostName("ircClient42.com"),
	_serverName(""), _realName(""), _socketFd(-1), _enteredPassword(false),
	_enteredNickname(false), _enteredUsername(false), _enteredServer(false)
{
	return ;
}

Client::Client(int client_fd) : _nickName(""), _userName(""), _hostName("ircClient42.com"),
	_serverName(""), _realName(""), _socketFd(client_fd), _enteredPassword(false),
	_enteredNickname(false), _enteredUsername(false), _enteredServer(false)
{
	return ;
}

Client::Client(const Client& copy) : _nickName(copy._nickName), _userName(copy._userName),
	_hostName(copy._hostName), _serverName(copy._serverName), _realName(copy._realName),
	_socketFd(copy._socketFd), _enteredPassword(copy._enteredPassword),
	_enteredNickname(copy._enteredNickname), _enteredUsername(copy._enteredUsername),
	_enteredServer(copy._enteredServer)
{
	return ;
}

Client::~Client(void)
{
	return ;
}

Client&	Client::operator=(const Client& src)
{
	if (this != &src) {
		this->_nickName = src._nickName;
		this->_userName = src._userName;
		this->_hostName = src._userName;
		this->_serverName = src._serverName;
		this->_realName = src._realName;
		this->_socketFd = src._socketFd;
		this->_enteredPassword = src._enteredPassword;
		this->_enteredNickname = src._enteredNickname;
		this->_enteredUsername = src._enteredUsername;
		this->_enteredServer = src._enteredServer;
	}
	return (*this);
}

//getter
const std::string&	Client::getNickName(void) const
{
	return (this->_nickName);
}

const std::string&	Client::getUserName(void) const
{
	return (this->_userName);
}

const std::string&	Client::getHostName(void) const
{
	return (this->_hostName);
}

int	Client::getSocketFd(void) const
{
	return (this->_socketFd);
}

bool	Client::getIsRegistered(void) const
{
	return (this->_enteredServer);
}

bool	Client::getEnteredNickname(void) const
{
	return (this->_enteredNickname);
}

bool	Client::getEnteredUsername(void) const
{
	return (this->_enteredUsername);
}

bool	Client::getEnteredPassword(void) const
{
	return (this->_enteredPassword);
}

void	Client::setNickName(std::string nickname)
{
	_nickName = nickname;
	this->_enteredNickname = true;
}

void 	Client::setUserName(std::string username)
{
	_userName = username;
	this->_enteredUsername = true;
}

void 	Client::setEnteredPassword(void)
{
	this->_enteredPassword = true;
}

void 	Client::setIsRegistered(void)
{
	if (this->getEnteredNickname() && this->getEnteredUsername() 
		&& this->getEnteredPassword())
		this->_enteredServer = true;
	else
		this->_enteredServer = false;
}

void 	Client::setSocketFd(int fd)
{
	this->_socketFd = fd;
}
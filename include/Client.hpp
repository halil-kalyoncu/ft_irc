
#pragma once

//string
#include <string>
//vector
#include <vector>

class Client
{
private:
	std::string				_nickName;
	std::string				_userName;
	std::string				_hostName;
	std::string				_serverName;
	std::string				_realName;
	int						_socketFd;
	bool					_enteredPassword;
	bool					_enteredNickname;
	bool					_enteredUsername;
	bool					_enteredServer;

	Client(void);
	Client(const Client& copy);

	Client&					operator=(const Client& src);
public:
	Client(int socketFd);
	~Client(void);

	//getter
	const std::string&		getNickName(void) const;
	const std::string&		getUserName(void) const;
	const std::string&		getHostName(void) const;
	int						getSocketFd(void) const;
	bool					getIsRegistered(void) const;
	bool					getEnteredNickname(void) const;
	bool					getEnteredUsername(void) const;
	bool					getEnteredPassword(void) const;

	//setter
	void 					setNickName(std::string nickname); 
	void 					setUserName(std::string username);
	void 					setEnteredPassword(void);
	void 					setIsRegistered(void);
	void 					setSocketFd(int fd);

};

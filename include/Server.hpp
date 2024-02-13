
#pragma once

// std::cout
# include <iostream>
// std::stoi()
# include <string>
// socket(), setsockopt(), bind(), accept(), recv(), send()
# include <sys/socket.h>
// fcntl()
# include <fcntl.h>
// htons()
#include <arpa/inet.h>
// poll()
#include <poll.h>
// errno()
#include <cerrno>
// std::memset()
#include <cstring>
// close()
#include <unistd.h>
//vector
#include <vector>
//exit
#include <cstdlib>
//transform, find
#include <algorithm>


#include "Client.hpp"
#include "Channel.hpp"
#include "ACommand.hpp"
#include "CommandPass.hpp"
#include "CommandNick.hpp"
#include "CommandUser.hpp"
#include "CommandJoin.hpp"
#include "CommandPart.hpp"
#include "CommandPrivmsg.hpp"
#include "CommandKick.hpp"
#include "CommandInvite.hpp"
#include "CommandTopic.hpp"
#include "CommandMode.hpp"
#include "CommandQuit.hpp"
#include "Reply.hpp"

class Server
{
private:
	const int							_port;
	std::string							_password;
	const std::string					_serverName;
	struct pollfd						_fds[1024];
	int									_listenFd;
	int									_nfds;
	int									_timeout;
	std::string							_request;

	std::time_t 						_startupTime;

	std::vector<Client* >				_clients;
	std::vector<Channel* >				_channels;

	static const std::string			_commandNames[11];

	static std::vector<std::string>		_splitRequest(std::string client_request);

	Client*								_getClient(int client_fd);
	ACommand*							_identifyCommand(const std::string& request_name,
											Client* user);
	bool								_parseRequest(int client_fd, const std::string& client_request);
	void								_printLog(const std::string &message);
	void								_exit(const std::string &error_message);
	void								_compressFds(void);
	bool 								_handleNewConnection(void);
	void								_receiveRequest(int fds_id);
	void								_closeConnection(int fds_id);
	void								_closeOpenSockets(void);

	void								_startServer(int port);
	void								_runServer(void);

	//Channel functions
	std::vector<Channel* >::iterator	_getChannelPos(const std::string& channel_name);
	void								_deleteChannel(const std::string& channel_name);

	std::vector<Client* >::iterator		_getClientPos(int client_fd);
	std::vector<Client* >::iterator		_getClientPos(const std::string& nick_name);

	Server(const Server& copy);
	Server&								operator=(const Server& src);
public:
	explicit Server(const int port, const std::string password);
	~Server(void);

	static std::string					normalizeInput(const std::string& input);
	bool								checkPassword(const std::string& client_input) const;

	//Channel functions
	Channel*							getChannel(const std::string& channel_name);
	Channel*							createChannel(const std::string& channel_name);
	t_ChannelERRNO						joinChannel(Client* client, Channel* channel,
											const std::string& password);
	bool								exitChannel(Client* client, Channel* channel);
	void								deleteChannels(void);

	Client*								getClient(const std::string& nick_name);
	Client*								eraseClientEntry(int client_fd);

	//getter
	const std::string&					getServerName(void) const;
	const std::vector<Client*>&			getClients(void) const;
	const std::vector<Channel*>&		getChannels(void) const;
	std::time_t							getTime(void) const;
};

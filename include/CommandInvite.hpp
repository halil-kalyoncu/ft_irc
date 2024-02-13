
#pragma once

//string
#include <string>
//vector
#include <vector>
//send
#include <sys/socket.h>

#include "ACommand.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Reply.hpp"

class CommandInvite : public ACommand
{
private:
	CommandInvite(void);
	CommandInvite(const CommandInvite& copy);

	CommandInvite&	operator=(const CommandInvite& src);
public:
	CommandInvite(Server* server, Client* client);
	~CommandInvite(void);


	bool			execute(const std::vector<std::string>& request_args);
	bool			checkChannelInList(const std::vector<Channel*>& channels,
											const Channel* channel);
};
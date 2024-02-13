
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
#include "Reply.hpp"

class CommandTopic : public ACommand
{
private:
	CommandTopic(void);
	CommandTopic(const CommandTopic& copy);
	CommandTopic&	operator=(const CommandTopic& src);
public:
	CommandTopic(Server* server, Client* client);
	~CommandTopic(void);

	bool			execute(const std::vector<std::string>& request_args);
};
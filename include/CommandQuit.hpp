
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
#include "Channel.hpp"

class CommandQuit : public ACommand
{
private:
	CommandQuit(void);
	CommandQuit(const CommandQuit& copy);
	CommandQuit&	operator=(const CommandQuit& src);
public:
	CommandQuit(Server* server, Client* client);
	~CommandQuit(void);

	bool			execute(const std::vector<std::string>& request_args);
};
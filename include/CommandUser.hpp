
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

class CommandUser : public ACommand
{
private:
	CommandUser(void);
	CommandUser(const CommandUser& copy);
	CommandUser&	operator=(const CommandUser& src);
public:

	CommandUser(Server* server, Client* client);
	~CommandUser(void);

	bool			execute(const std::vector<std::string>& request_args);
};

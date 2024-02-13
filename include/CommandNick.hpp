
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

class CommandNick : public ACommand
{
private:
	CommandNick(void);
	CommandNick(const CommandNick& copy);

	CommandNick&	operator=(const CommandNick& src);
public:
	CommandNick(Server* server, Client* client);
	~CommandNick(void);

	bool			execute(const std::vector<std::string>& request_args);
};


#pragma once

#define MAXTARGETS 50

//string
#include <string>
//vector
#include <vector>
//send
#include <sys/socket.h>
//ostringstream
#include <sstream>

#include "ACommand.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Reply.hpp"

class CommandPrivmsg : public ACommand
{
private:
	CommandPrivmsg(void);
	CommandPrivmsg(const CommandPrivmsg& copy);
	CommandPrivmsg&	operator=(const CommandPrivmsg& src);
	static size_t	_maxTargets;

	bool			_sendChannel(const std::string& channel_name,
						const std::string& message);
	bool			_executePrivmsg(const std::string& target,
						const std::string& message);
public:
	CommandPrivmsg(Server* server, Client* client);
	~CommandPrivmsg(void);

	bool			execute(const std::vector<std::string>& request_args);
};
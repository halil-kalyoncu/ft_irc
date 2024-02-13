
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

class CommandPart : public ACommand
{
	bool			_executePart(const std::string& channel_name);
	bool			_sendReply(const Channel* channel, const std::string& channel_name,
								size_t channel_size) const;
	
	CommandPart(void);
	CommandPart(const CommandPart& copy);

	CommandPart&	operator=(const CommandPart& src);
public:
	CommandPart(Server* server, Client* client);
	~CommandPart(void);

	bool			execute(const std::vector<std::string>& request_args);
};

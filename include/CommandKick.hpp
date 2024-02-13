
#pragma once

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

class CommandKick : public ACommand
{
private:
	std::string		_createReplyCommandName(const std::string& channel_name,
												const std::string& nick_name) const;
	bool			_executeKick(Channel* channel, const std::string& nick_name,
									const std::string& kick_message);

	CommandKick(void);
	CommandKick(const CommandKick& copy);

	CommandKick&	operator=(const CommandKick& src);
public:
	CommandKick(Server* server, Client* client);
	~CommandKick(void);

	bool			execute(const std::vector<std::string>& request_args);
};


#pragma once

//string
#include <string>
//vector
#include <vector>
//send
#include <sys/socket.h>
//stringstream, ostream
#include <sstream>

#include "ACommand.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Reply.hpp"

class CommandMode : public ACommand
{
private:
	static std::string	_supportedFlags[10];

	bool				_executeMODE(const std::string& mode_flag, Channel* channel,
										const std::vector<std::string>& request_args);
	bool				_sendSuccessReply(const Channel* channel,
											const std::string& reply_commandname,
											const std::string& reply_additional);

	bool				_setInviteOnly(Channel* channel);
	bool				_removeInviteOnly(Channel* channel);
	bool				_setDisallowTopicChange(Channel* channel);
	bool				_removeDisallowTopicChange(Channel* channel);
	bool				_setKey(Channel* channel, const std::vector<std::string>& request_args);
	bool				_removeKey(Channel* channel, const std::vector<std::string>& request_args);
	bool				_giveOperator(Channel* channel, const std::vector<std::string>& request_args);
	bool				_removeOperator(Channel* channel, const std::vector<std::string>& request_args);
	bool				_setUserLimit(Channel* channel, const std::vector<std::string>& request_args);
	bool				_removeUserLimit(Channel* channel);

	CommandMode(void);
	CommandMode(const CommandMode& copy);

	CommandMode&		operator=(const CommandMode& src);
public:
	CommandMode(Server* server, Client* client);
	~CommandMode(void);

	bool				execute(const std::vector<std::string>& request_args);
};

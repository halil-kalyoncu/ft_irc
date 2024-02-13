
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

class CommandJoin : public ACommand
{
private:
	static void	_matchVectorSizes(size_t channelnames_count,
										std::vector<std::string>& passwords);

	bool		_executeJoin(const std::string& channel_name, const std::string& password);
	bool		_sendSuccessReply(const Channel* channel) const;
	bool		_sendReply(t_ChannelERRNO errno_join, const Channel* channel) const;

	CommandJoin(void);
	CommandJoin(const CommandJoin& copy);

	CommandJoin&	operator=(const CommandJoin& src);
public:
	CommandJoin(Server* server, Client* client);
	~CommandJoin(void);

	bool			execute(const std::vector<std::string>& request_args);
};

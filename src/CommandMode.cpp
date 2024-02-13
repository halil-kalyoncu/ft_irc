
#include "../include/CommandMode.hpp"

std::string	CommandMode::_supportedFlags[10] =	{"+i", "-i",
												"+t", "-t",
												"+k", "-k",
												"+o", "-o",
												"+l", "-l"};

bool	CommandMode::_executeMODE(const std::string& mode_flag, Channel* channel,
									const std::vector<std::string>& request_args)
{
	size_t	mode_id = 0;

	while (mode_id < 10) {
		if (mode_flag == CommandMode::_supportedFlags[mode_id]) {
			break ;
		}
		mode_id++;
	}

	switch(mode_id) {
		case 0:
			return (this->_setInviteOnly(channel));
		case 1:
			return (this->_removeInviteOnly(channel));
		case 2:
			return (this->_setDisallowTopicChange(channel));
		case 3:
			return (this->_removeDisallowTopicChange(channel));
		case 4:
			return (this->_setKey(channel, request_args));
		case 5:
			return (this->_removeKey(channel, request_args));
		case 6:
			return (this->_giveOperator(channel, request_args));
		case 7:
			return (this->_removeOperator(channel, request_args));
		case 8:
			return (this->_setUserLimit(channel, request_args));
		case 9:
			return (this->_removeUserLimit(channel));
		default:
			return (this->_reply->errUnknownmode(request_args[2],
						channel->getChannelName()));
	}
}

bool	CommandMode::_sendSuccessReply(const Channel* channel,
											const std::string& reply_commandname,
											const std::string& reply_additional)
{
	if (this->_reply->sendMultiReply(channel->getRegularClients(),
			this->_client, reply_commandname, reply_additional) == false) {
		return (false);
	}
	if (this->_reply->sendMultiReply(channel->getOperatorClients(),
			this->_client, reply_commandname, reply_additional) == false) {
		return (false);
	}
	return (this->_reply->rplCustom(this->_client, this->_client,
		reply_commandname, reply_additional));	
}

bool	CommandMode::_setInviteOnly(Channel* channel)
{
	std::ostringstream	oss;

	if (channel->getInviteOnly() == true) {
		return (true);
	}

	channel->setInviteOnly(true);
	oss << this->_commandName << " " << channel->getChannelName();
	return (this->_sendSuccessReply(channel, oss.str(), "+i"));
}

bool	CommandMode::_removeInviteOnly(Channel* channel)
{
	std::ostringstream	oss;

	if (channel->getInviteOnly() == false) {
		return (true);
	}

	channel->setInviteOnly(false);
	oss << this->_commandName << " " << channel->getChannelName();
	return (this->_sendSuccessReply(channel, oss.str(), "-i"));
}

bool	CommandMode::_setDisallowTopicChange(Channel* channel)
{
	std::ostringstream	oss;

	if (channel->getDisallowTopicChange() == true) {
		return (true);
	}

	channel->setDisallowTopicChange(true);
	oss << this->_commandName << " " << channel->getChannelName();
	return (this->_sendSuccessReply(channel, oss.str(), "+t"));
}

bool	CommandMode::_removeDisallowTopicChange(Channel* channel)
{
	std::ostringstream	oss;

	if (channel->getDisallowTopicChange() == false) {
		return (true);
	}

	channel->setDisallowTopicChange(false);
	oss << this->_commandName << " " << channel->getChannelName();
	return (this->_sendSuccessReply(channel, oss.str(), "-t"));
}

bool	CommandMode::_setKey(Channel* channel, const std::vector<std::string>& request_args)
{
	const std::string&	current_password = channel->getChannelKey();
	std::ostringstream	oss;

	if (request_args.size() < 4) {
		return (this->_reply->errKeyset(channel->getChannelName()));
	}

	if (Channel::checkValidChannelKey(request_args[3]) == false) {
		return (this->_reply->errWrongChannelKey(channel->getChannelName()));
	}

	if (current_password.empty() == false) {
		return (true);
	}

	channel->setChannelKey(request_args[3]);
	oss << this->_commandName << " " << channel->getChannelName();
	return (this->_sendSuccessReply(channel, oss.str(), "+k"));
}

bool	CommandMode::_removeKey(Channel* channel, const std::vector<std::string>& request_args)
{
	const std::string&	current_password = channel->getChannelKey();
	std::ostringstream	oss;

	if (request_args.size() < 4) {
		return (this->_reply->errKeyset(channel->getChannelName()));
	}

	if (Channel::checkValidChannelKey(request_args[3]) == false) {
		return (this->_reply->errWrongChannelKey(channel->getChannelName()));
	}

	if (current_password.empty() == true) {
		return (true);
	}

	if (current_password != request_args[3]) {
		this->_reply->errKeyset(channel->getChannelName());
	}

	channel->setChannelKey("");
	oss << this->_commandName << " " << channel->getChannelName();
	return (this->_sendSuccessReply(channel, oss.str(), "-k"));
}

bool	CommandMode::_giveOperator(Channel* channel, const std::vector<std::string>& request_args)
{
	Client*				client;
	t_ChannelERRNO		errno_operator;
	std::ostringstream	oss;

	if (request_args.size() < 4) {
		return (this->_reply->errNonicknamegiven());
	}

	client = this->_server->getClient(request_args[3]);
	if (client == NULL) {
		return (this->_reply->errNosuchnick(request_args[3]));
	}

	errno_operator = channel->addOperatorStatus(client);
	switch (errno_operator) {
		case (ChannelERRNO_NOTJOINED):
			return (this->_reply->errUsernotonchannel(client->getNickName(),
												channel->getChannelName()));
		case (ChannelERRNO_NOSTATUSCHANGE):
			return (true);
		case (ChannelERRNO_SUCCESS):
			oss << this->_commandName << " " << channel->getChannelName();
			return (this->_sendSuccessReply(channel, oss.str(), "+o"));
		default:
			return (this->_reply->errSomethingwentwrong());
	}
}

bool	CommandMode::_removeOperator(Channel* channel, const std::vector<std::string>& request_args)
{
	Client*				client;
	t_ChannelERRNO		errno_operator;
	std::ostringstream	oss;

	if (request_args.size() < 4) {
		return (this->_reply->errNonicknamegiven());
	}

	client = this->_server->getClient(request_args[3]);
	if (client == NULL) {
		return (this->_reply->errNosuchnick(request_args[3]));
	}

	errno_operator = channel->removeOperatorStatus(client);
	switch (errno_operator) {
		case (ChannelERRNO_NOTJOINED):
			return (this->_reply->errUsernotonchannel(client->getNickName(),
												channel->getChannelName()));
		case (ChannelERRNO_NOSTATUSCHANGE):
			return (true);
		case (ChannelERRNO_SUCCESS):
			oss << this->_commandName << " " << channel->getChannelName();
			return (this->_sendSuccessReply(channel, oss.str(), "-o"));
		default:
			return (this->_reply->errSomethingwentwrong());
	}
}

bool	CommandMode::_setUserLimit(Channel* channel, const std::vector<std::string>& request_args)
{
	int					request_limit;
	std::stringstream	sStream;

	if (request_args.size() < 4) {
		return (this->_reply->errNolimitgiven(channel->getChannelName()));
	}

	sStream.str(request_args[3]);
	sStream >> request_limit;
	if (sStream.fail() || request_limit <= 0) {
		return (this->_reply->errWronglimit(channel->getChannelName()));
	}

	if ((size_t) request_limit < channel->getChannelSize()) {
		return (this->_reply->errChannelisfull(channel->getChannelName()));
	}

	channel->setLimit(request_limit);
	if (this->_reply->sendMultiReply(channel->getRegularClients(),
			this->_client, this->_commandName, "+l") == false) {
		return (false);
	}
	if (this->_reply->sendMultiReply(channel->getOperatorClients(),
			this->_client, this->_commandName, "+l") == false) {
		return (false);
	}
	return (this->_reply->rplCustom(this->_client, this->_client,
		this->_commandName, "+l"));
}

bool	CommandMode::_removeUserLimit(Channel* channel)
{
	if (channel->getLimit() == 0) {
		return (true);
	}

	channel->setLimit(0);
	if (this->_reply->sendMultiReply(channel->getRegularClients(),
			this->_client, this->_commandName, "-l") == false) {
		return (false);
	}
	if (this->_reply->sendMultiReply(channel->getOperatorClients(),
			this->_client, this->_commandName, "-l") == false) {
		return (false);
	}
	return (this->_reply->rplCustom(this->_client, this->_client,
		this->_commandName, "-l"));
	return (true);
}

CommandMode::CommandMode(Server* server, Client* client) : ACommand("MODE", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandMode::~CommandMode(void)
{
	delete this->_reply;
	return ;
}

bool	CommandMode::execute(const std::vector<std::string>& request_args)
{
	Channel*	channel;

	if (request_args.size() < 3) {
		return (this->_reply->errNeedmoreparams(this->_commandName));
	}
	if (!this->_client->getIsRegistered())
		return (this->_reply->errNotregistered(request_args[0]));
	// Check if the channel exists and the user is joined to the channel.
	if (Channel::checkValidChannelName(request_args[1]) == false) {
		return (this->_reply->errBadchanmask(request_args[1]));
	}
	channel = this->_server->getChannel(request_args[1]);
	if (channel == NULL) {
		return (this->_reply->errNosuchchannel(request_args[1]));
	}
	if (channel->checkJoined(this->_client) == false) {
		return (this->_reply->errNotonchannel(channel->getChannelName()));
	}

	// Check if the user has permission to modify the channel modes (i.e., is the channel operator or server operator).
	if (channel->checkIsOperatorClient(this->_client) == false) {
		return (this->_reply->errChanoprivsneeded(channel->getChannelName()));
	}
	return (this->_executeMODE(request_args[2], channel, request_args));
}

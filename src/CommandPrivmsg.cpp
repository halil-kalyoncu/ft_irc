
#include "../include/CommandPrivmsg.hpp"

size_t	CommandPrivmsg::_maxTargets = 50;

bool	CommandPrivmsg::_sendChannel(const std::string& channel_name,
	const std::string& message)
{
	Channel*			channel = this->_server->getChannel(channel_name);
	std::ostringstream	oss;
	std::string			reply_target;

	if (channel == NULL) {
		return (this->_reply->errNosuchchannel(channel_name));
	}

	oss << this->_commandName << " " << channel->getChannelName();
	reply_target = oss.str();
	if (this->_reply->sendMultiReply(channel->getRegularClients(), this->_client,
			reply_target, message) == false) {
		return (false);
	}
	return (this->_reply->sendMultiReply(channel->getOperatorClients(), this->_client,
											reply_target, message));
}

bool	CommandPrivmsg::_executePrivmsg(const std::string& target,
											const std::string& message)
{
	Client*				client;
	std::ostringstream	oss;
	std::string			reply_target;

	if (Channel::checkValidChannelName(target) == true) {
		return (this->_sendChannel(target, message));
	}

	client = this->_server->getClient(target);
	if (client == NULL) {
		return (this->_reply->errNosuchnick(target));
	}
	oss << this->_commandName << " " << client->getNickName();
	reply_target = oss.str();
	return (this->_reply->rplCustom(client, this->_client, reply_target,
										message));
}

CommandPrivmsg::CommandPrivmsg(Server* server, Client* client) : ACommand("PRIVMSG", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandPrivmsg::~CommandPrivmsg(void)
{
	delete this->_reply;
	return ;
}

bool	CommandPrivmsg::execute(const std::vector<std::string>& request_args)
{
	std::vector<std::string>	message_targets;
	size_t						i = 0;

	if (request_args.size() < 3) {
		return this->_reply->errNeedmoreparams(this->_commandName);
	}
	if (!this->_client->getIsRegistered())
		return (this->_reply->errNotregistered(request_args[0]));

	message_targets = this->_splitArgument(request_args[1]);
	if (message_targets.size() == 0) {
		return (true);
	}
	else if (message_targets.size() > this->_maxTargets) {
		return (this->_reply->errToomanytargets(this->_client->getNickName(),
			message_targets.size(), this->_maxTargets));
	}
	else if (request_args[2].empty() == true) {
		return (this->_reply->errNotexttosend());
	}

	while (i < message_targets.size()) {
		if (this->_executePrivmsg(message_targets[i], request_args[2]) == false) {
			return (false);
		}
		i++;
	}
	return (true);
}
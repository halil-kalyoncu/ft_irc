
#include "../include/CommandPart.hpp"

CommandPart::CommandPart(Server* server, Client* client) : ACommand("PART", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandPart::~CommandPart(void)
{
	delete this->_reply;
	return ;
}

bool	CommandPart::_executePart(const std::string& channel_name)
{
	Channel*	channel;
	size_t		channel_size;

	channel = this->_server->getChannel(channel_name);
	if (channel == NULL) {
		return (this->_reply->errNosuchchannel(channel_name));
	}

	channel_size = channel->getChannelSize();
	const std::string	entry_channelname(channel->getChannelName());
	if (this->_server->exitChannel(this->_client, channel) == false) {
		return (this->_reply->errNotonchannel(entry_channelname));
	}
	channel_size--;
	return (this->_sendReply(channel, entry_channelname, channel_size));
}

bool	CommandPart::_sendReply(const Channel* channel, const std::string& channel_name,
	size_t channel_size) const
{
	if (channel_size > 0) {
		if (Reply::sendMultiReply(channel->getRegularClients(), this->_client,
									this->_commandName, channel->getChannelName()) == false) {
			return (false);
		}
		if (Reply::sendMultiReply(channel->getOperatorClients(), this->_client,
									this->_commandName, channel->getChannelName()) == false) {
			return (false);
		}
	}
	return (Reply::rplCustom(this->_client, this->_client, this->_commandName,
				channel_name));
}

bool	CommandPart::execute(const std::vector<std::string>& request_args)
{
	std::vector<std::string>	channel_names;
	size_t						i = 0;

	if (request_args.size() == 1) {
		return (this->_reply->errNeedmoreparams(this->_commandName));
	}
	if (!this->_client->getIsRegistered())
		return (this->_reply->errNotregistered(request_args[0]));

	channel_names = this->_splitArgument(request_args[1]);
	if (channel_names.size() == 0) {
		return (true);
	}
	while (i < channel_names.size()) {
		if (this->_executePart(channel_names[i]) == false) {
			return (false);
		}
		i++;
	}
	return (true);
}

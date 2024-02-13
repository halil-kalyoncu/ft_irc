
#include "../include/CommandTopic.hpp"

CommandTopic::CommandTopic(Server* server, Client* client) : ACommand("TOPIC", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandTopic::~CommandTopic(void)
{
	delete this->_reply;
	return ;
}

bool CommandTopic::execute(const std::vector<std::string>& request_args) {
	// Check that there are enough arguments
	if (request_args.size() < 2) {
		return (this->_reply->errNeedmoreparams(this->_commandName));
	}
	if (!this->_client->getIsRegistered())
		return (this->_reply->errNotregistered(request_args[0]));
	std::string channel_name = request_args[1];
	std::string	topic;

	// Find the channel
	Channel* channel = this->_server->getChannel(channel_name);
	if (channel == NULL)
		return (this->_reply->errNosuchchannel(channel_name));

	if (request_args.size() == 2 && channel->getTopic().empty())
		return (this->_reply->rplNotopic(channel_name));
	if (request_args.size() == 2 && !channel->getTopic().empty())
		return (this->_reply->rplTopic(channel_name, channel->getTopic()));
	if (request_args.size() == 3 && request_args[2] != "''")
	{
		// Set the channel topic
		topic = request_args[2];
		if (channel->getDisallowTopicChange())
		{
			if (!channel->checkIsOperatorClient(this->_client))
				return (this->_reply->errChanoprivsneeded(channel_name));
		}
		channel->setTopic(topic);
		if (!this->_reply->sendMultiReplyExtra(channel->getOperatorClients(), this->_client,
				this->_commandName, channel->getChannelName(), channel->getTopic())) {
			return (false);
		}
		return (this->_reply->sendMultiReplyExtra(channel->getRegularClients(), this->_client,
							this->_commandName, channel->getChannelName(), channel->getTopic()));
	}
	return (true);
}

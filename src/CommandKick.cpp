
#include "../include/CommandKick.hpp"

std::string		CommandKick::_createReplyCommandName(const std::string& channel_name,
											const std::string& nick_name) const
{
	std::ostringstream	oss;

	oss << this->_commandName << " " << channel_name << " " << nick_name;
	return (oss.str());
}

bool	CommandKick::_executeKick(Channel* channel, const std::string& nick_name,
									const std::string& kick_message)
{
	Client*			client = this->_server->getClient(nick_name);
	std::string		reply_commandname;
	size_t			channel_size;

	//check if client is on server and channel
	if (client == NULL) {
		return (this->_reply->errNosuchnick(nick_name));
	}
	if (channel->checkJoined(client) == false) {
		return (this->_reply->errUsernotonchannel(client->getNickName(),
			channel->getChannelName()));
	}

	reply_commandname = this->_createReplyCommandName(channel->getChannelName(),
														client->getNickName());

	channel_size = channel->getChannelSize();
	if (channel->removeClient(client) == false) {
		return (this->_reply->errSomethingwentwrong());
	}

	//send replies, if the only on the channel kicks himself the channel doesn't
	//exist anymore
	if (channel_size > 1) {
		if (this->_reply->sendMultiReply(channel->getRegularClients(), client,
											reply_commandname, kick_message) == false) {
			return (false);
		}
		if (this->_reply->sendMultiReply(channel->getOperatorClients(), client,
											reply_commandname, kick_message) == false) {
			return (false);
		}
	}
	return (this->_reply->rplCustom(client, client, reply_commandname,
		kick_message));
}

CommandKick::CommandKick(Server* server, Client* client) : ACommand("KICK", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandKick::~CommandKick(void)
{
	delete this->_reply;
	return ;
}

bool	CommandKick::execute(const std::vector<std::string>& request_args)
{
	std::vector<std::string>	nick_names;
	Channel*					channel;
	std::string					kick_message;
	size_t						i = 0;

	if (request_args.size() < 3) {
		return this->_reply->errNeedmoreparams(this->_commandName);
	}
	if (!this->_client->getIsRegistered())
		return (this->_reply->errNotregistered(request_args[0]));

	nick_names = this->_splitArgument(request_args[2]);
	if (nick_names.size() == 0) {
		return (true);
	}
	if (Channel::checkValidChannelName(request_args[1]) == false) {
		return (this->_reply->errBadchanmask(request_args[1]));
	}

	//check if channel exists and afterwards if client is on that channel
	channel = this->_server->getChannel(request_args[1]);
	if (channel == NULL) {
		return (this->_reply->errNosuchchannel(request_args[1]));
	}
	if (channel->checkJoined(this->_client) == false) {
		return (this->_reply->errNotonchannel(channel->getChannelName()));
	}

	//only operator can kick
	if (channel->checkIsOperatorClient(this->_client) == false) {
		return (this->_reply->errChanoprivsneeded(channel->getChannelName()));
	}

	//kick message of request is only send if the request only one client
	if (nick_names.size() == 1 && request_args.size() > 3) {
		kick_message = request_args[3];
	}
	else {
		kick_message = this->_client->getNickName();
	}

	while (i < nick_names.size()) {
		if (this->_executeKick(channel, nick_names[i], kick_message) == false) {
			return (false);
		}
		i++;
	}
	return (true);
}

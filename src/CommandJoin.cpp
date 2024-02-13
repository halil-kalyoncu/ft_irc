
#include "../include/CommandJoin.hpp"

void	CommandJoin::_matchVectorSizes(size_t channelnames_count,
	std::vector<std::string>& passwords)
{
	while (passwords.size() != channelnames_count) {
		passwords.push_back("");
	}
	return ;
}

bool	CommandJoin::_executeJoin(const std::string& channel_name,
	const std::string& password)
{
	Channel*		channel;
	t_ChannelERRNO	errno_join;

	if (Channel::checkValidChannelName(channel_name) == false) {
		return (this->_reply->errBadchanmask(channel_name));
	}
	channel = this->_server->getChannel(channel_name);

	//if channel is not present create it
	if (channel == NULL) {
		channel = this->_server->createChannel(channel_name);
	}

	errno_join = this->_server->joinChannel(this->_client, channel, password);
	return (this->_sendReply(errno_join, channel));
}

bool	CommandJoin::_sendSuccessReply(const Channel* channel) const
{
	if (this->_reply->rplCustom(this->_client, this->_client, this->_commandName,
								channel->getChannelName()) == false) {
		return (false);
	}
	if (this->_reply->rplNamreply(channel) == false) {
		return (false);
	}
	if (this->_reply->rplEndofnames(channel->getChannelName()) == false) {
		return (false);
	}
	if (Reply::sendMultiReply(channel->getRegularClients(), this->_client,
								this->_commandName, channel->getChannelName()) == false) {
		return (false);
	}
	return (Reply::sendMultiReply(channel->getOperatorClients(), this->_client,
									this->_commandName, channel->getChannelName()));
}

bool	CommandJoin::_sendReply(t_ChannelERRNO errno_join, const Channel* channel) const
{
	switch (errno_join) {
		case ChannelERRNO_CHANNELFULL:
			return (this->_reply->errChannelisfull(channel->getChannelName()));
		case ChannelERRNO_BADCHANNELKEY:
			return (this->_reply->errBadchannelkey(channel->getChannelName()));
		case ChannelERRNO_INVITEONLY:
			return (this->_reply->errInviteonlychan(channel->getChannelName()));
		case ChannelERRNO_ALREADYJOINED:
			return (true);
		case ChannelERRNO_SUCCESS:
			return (this->_sendSuccessReply(channel));
		default:
			return (this->_reply->errSomethingwentwrong());
	}
}

CommandJoin::CommandJoin(Server* server, Client* client) : ACommand("JOIN", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandJoin::~CommandJoin(void)
{
	delete this->_reply;
	return ;
}

bool	CommandJoin::execute(const std::vector<std::string>& request_args)
{
	std::vector<std::string>	channel_names;
	std::vector<std::string>	passwords;
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
	if (request_args.size() >= 3) {
		passwords = this->_splitArgument(request_args[2]);
	}
	CommandJoin::_matchVectorSizes(channel_names.size(), passwords);

	while (i < channel_names.size()) {
		if (this->_executeJoin(channel_names[i], passwords[i]) == false) {
			return (false);
		}
		i++;
	}
	return (true);
}

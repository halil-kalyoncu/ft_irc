
#include "../include/CommandInvite.hpp"

CommandInvite::CommandInvite(Server* server, Client* client) : ACommand("INVITE", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandInvite::~CommandInvite(void)
{
	delete this->_reply;
	return ;
}

bool CommandInvite::checkChannelInList(const std::vector<Channel*>& channels, const Channel* channel) 
{
	for (std::vector<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (*it == channel)
			return (true);
	}
	return (false);
}

bool CommandInvite::execute(const std::vector<std::string>& request_args)
{
	if (request_args.size() < 3)
		return (this->_reply->errNeedmoreparams(this->_commandName));
	if (!this->_client->getIsRegistered())
		return (this->_reply->errNotregistered(request_args[0]));

	std::string nickname_to_invite = request_args[1];
	std::string channel_name = request_args[2];

	Client* client_to_invite = this->_server->getClient(nickname_to_invite);
	if (client_to_invite == NULL) 
		return (this->_reply->errNosuchnick(nickname_to_invite));
	if (!this->_server->getChannel(channel_name))
	{
		if (this->_reply->rplInviting(channel_name, nickname_to_invite) == false) {
			return (false);
		}
		return (this->_reply->rplInvited(client_to_invite, this->_client, "INVITE", nickname_to_invite, channel_name));
	}
	Channel* channel = this->_server->getChannel(channel_name);
	if (!channel->checkIsRegularClient(this->_client) && !channel->checkIsOperatorClient(this->_client))
		return (this->_reply->errNotonchannel(channel_name));

	if (channel->getInviteOnly() == true)
		if (!channel->checkIsOperatorClient(this->_client))
			return (this->_reply->errChanoprivsneeded(channel_name));

	if (!channel->checkIsRegularClient(client_to_invite) && !channel->checkIsOperatorClient(client_to_invite)) 
	{
		channel->addInvitedClient(client_to_invite);
		if (this->_reply->rplInviting(channel_name, nickname_to_invite) == false) {
			return (false);
		}
		return (this->_reply->rplInvited(client_to_invite, this->_client, "INVITE", nickname_to_invite, channel_name));
	}
	return (this->_reply->errUseronchannel(this->_client->getNickName(), nickname_to_invite, channel_name));
}
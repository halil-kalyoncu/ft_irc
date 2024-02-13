
#include "../include/Channel.hpp"

bool	Channel::_removeRegularClient(const Client* client)
{
	std::vector<const Client* >::iterator	pos_remove = this->_getRegularClientPos(client);

	if (pos_remove != this->_regularClients.end()) {
		this->_regularClients.erase(pos_remove);
		return (true);
	}
	return (false);
}

bool	Channel::_removeOperatorClient(const Client* client)
{
	std::vector<const Client* >::iterator	pos_remove = this->_getOperatorClientPos(client);

	if (pos_remove != this->_operatorClients.end()) {
		this->_operatorClients.erase(pos_remove);
		return (true);
	}
	return (false);
}

std::vector<const Client* >::iterator	Channel::_getRegularClientPos(const Client *client)
{
	return (std::find(this->_regularClients.begin(),
		this->_regularClients.end(), client));
}

std::vector<const Client* >::iterator	Channel::_getOperatorClientPos(const Client *client)
{
	return (std::find(this->_operatorClients.begin(),
		this->_operatorClients.end(), client));
}

std::vector<const Client* >::iterator	Channel::_getInvitedPos(const Client *client)
{
	return (std::find(this->_invitedClients.begin(),
		this->_invitedClients.end(), client));
}

Channel::Channel(void) : _channelName(""), _limit(0), _topic("\0"), _inviteOnly(false), _disallowTopicChange(true)
{
	return ;
}

Channel::Channel(const std::string& channelName) : _channelName(channelName),
	_limit(0), _topic("\0"), _inviteOnly(false), _disallowTopicChange(true)
{
	return ;
}

Channel::~Channel(void)
{
	return ;
}

bool	Channel::checkValidChannelName(const std::string& channel_name)
{
	if (channel_name.empty() == true || channel_name.length() > 50) {
		return (false);
	}
	// check for ^G
	if (channel_name.find(7) != std::string::npos) {
		return (false);
	}
	if (channel_name.find_first_of("&#+!") != 0) {
		return (false);
	}
	return (channel_name.find_first_of(" ,") == std::string::npos);
}

bool	Channel::checkValidChannelKey(const std::string& channel_key)
{
	if (channel_key.empty() == true || channel_key.length() < 5) {
		return (false);
	}
	// check for ^G
	if (channel_key.find(7) != std::string::npos) {
		return (false);
	}
	return (channel_key.find_first_of(" ,") == std::string::npos);
}

/**
 * Adds client as an regular user to the channel.
 *
 * @param	client Client that is added to the channel.
 *
 * @return	Number that indicates the outcome of the function.
 * 			Everything < 0 means that the client was not added to the channel.
 * 			If the client was already on the server the functions returns 1.
 */
t_ChannelERRNO	Channel::addClient(const Client* client, const std::string& password)
{
	size_t									channel_size = this->getChannelSize();
	std::vector<const Client* >::iterator	invite_pos = this->_getInvitedPos(client);

	if (this->_limit != 0 && this->_limit == channel_size) {
		return (ChannelERRNO_CHANNELFULL);
	}
	else if (this->checkJoined(client)) {
		return (ChannelERRNO_ALREADYJOINED);
	}
	else if (this->_inviteOnly == true && invite_pos == this->_invitedClients.end()) {
		return (ChannelERRNO_INVITEONLY);
	}

	if (this->_channelKey.empty() == false && password != this->_channelKey) {
		return (ChannelERRNO_BADCHANNELKEY);
	}

	//add client to the channel, first client is operator
	if (channel_size == 0) {
		this->_operatorClients.push_back(client);
	}
	else {
		this->_regularClients.push_back(client);
	}

	//remove from invitedClients
	if (this->_inviteOnly == true) {
		this->_invitedClients.erase(invite_pos);
	}
	return (ChannelERRNO_SUCCESS);
}

bool	Channel::removeClient(const Client* client)
{
	std::vector<const Client* >::iterator	pos_remove;
	std::vector<const Client* >::iterator	invite_pos = this->_getInvitedPos(client);

	if (invite_pos != this->_invitedClients.end()) {
		this->_invitedClients.erase(invite_pos);
	}
	if (this->checkIsOperatorClient(client)) {
		return (this->_removeOperatorClient(client));
	}
	return (this->_removeRegularClient(client));
}

bool	Channel::checkJoined(const Client* client) const
{
	return (this->checkIsRegularClient(client)
				|| this->checkIsOperatorClient(client));
}

std::string	Channel::stringRegularClients(void) const
{
	std::ostringstream							oss;
	std::vector<const Client* >::const_iterator	it = this->_regularClients.begin();

	while (it != this->_regularClients.end()) {
		oss << (*it)->getNickName() << " ";
		it++;
	}
	return (oss.str());
}

std::string	Channel::stringOperatorClients(void) const
{
	std::ostringstream							oss;
	std::vector<const Client* >::const_iterator	it = this->_operatorClients.begin();

	while (it != this->_operatorClients.end()) {
		oss << "@" << (*it)->getNickName() << " ";
		it++;
	}
	return (oss.str());
}

bool	Channel::checkIsRegularClient(const Client* client) const
{
	std::vector<const Client* >::const_iterator	pos_regular = std::find(this->_regularClients.begin(),
																			this->_regularClients.end(),
																			client);

	return (pos_regular != this->_regularClients.end());
}

bool	Channel::checkIsOperatorClient(const Client* client) const
{
	std::vector<const Client* >::const_iterator	pos_operator = std::find(this->_operatorClients.begin(),
																			this->_operatorClients.end(),
																			client);

	return (pos_operator != this->_operatorClients.end());
}

t_ChannelERRNO	Channel::addOperatorStatus(const Client *client)
{
	std::vector<const Client* >::iterator	regularClientPos;

	if (client == NULL) {
		return (ChannelERRNO_FAIL);
	}

	if (this->checkJoined(client) == false) {
		return (ChannelERRNO_NOTJOINED);
	}

	regularClientPos = this->_getRegularClientPos(client);
	if (regularClientPos == this->_regularClients.end()) {
		return (ChannelERRNO_NOSTATUSCHANGE);
	}
	this->_regularClients.erase(regularClientPos);
	this->_operatorClients.push_back(client);
	return (ChannelERRNO_SUCCESS);
}

t_ChannelERRNO	Channel::removeOperatorStatus(const Client *client)
{
	std::vector<const Client* >::iterator	operatorClientPos;

	if (client == NULL) {
		return (ChannelERRNO_FAIL);
	}

	if (this->checkJoined(client) == false) {
		return (ChannelERRNO_NOTJOINED);
	}

	operatorClientPos = this->_getOperatorClientPos(client);
	if (operatorClientPos == this->_regularClients.end()) {
		return (ChannelERRNO_NOSTATUSCHANGE);
	}
	this->_operatorClients.erase(operatorClientPos);
	this->_regularClients.push_back(client);
	return (ChannelERRNO_SUCCESS);
}

const std::vector<const Client* >&	Channel::getRegularClients(void) const
{
	return (this->_regularClients);
}

const std::vector<const Client* >&	Channel::getOperatorClients(void) const
{
	return (this->_operatorClients);
}

size_t	Channel::getLimit(void) const
{
	return (this->_limit);
}

const std::string&	Channel::getChannelKey(void) const
{
	return (this->_channelKey);
}

bool	Channel::getInviteOnly(void) const
{
	return (this->_inviteOnly);
}

//getter
const std::string&	Channel::getChannelName(void) const
{
	return (this->_channelName);
}

size_t	Channel::getChannelSize(void) const
{
	return (this->_regularClients.size() + this->_operatorClients.size());
}

const std::string&	Channel::getTopic(void) const
{
	return (this->_topic);
}

bool	Channel::getDisallowTopicChange(void) const
{
	return (this->_disallowTopicChange);
}

void Channel::addInvitedClient(const Client* client) 
{
	_invitedClients.push_back(client);
}

void	Channel::setLimit(size_t limit)
{
	this->_limit = limit;
}

void	Channel::setChannelKey(const std::string& channel_key)
{
	this->_channelKey = channel_key;
	return ;
}

void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

void	Channel::setInviteOnly(bool dis_allow)
{
	this->_inviteOnly = dis_allow;
	return ;
}

void Channel::setDisallowTopicChange(bool dis_allow)
{
	_disallowTopicChange = dis_allow;
}
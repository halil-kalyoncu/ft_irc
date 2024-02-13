
#pragma once

//string
#include <string>
//vector
#include <vector>
//find
#include <algorithm>
//ostringstream
#include <sstream>

#include "Client.hpp"

typedef enum e_channelERRNO
{
	ChannelERRNO_NOTJOINED = -5,
	ChannelERRNO_INVITEONLY = -4,
	ChannelERRNO_BADCHANNELKEY = -3,
	ChannelERRNO_CHANNELFULL = -2,
	ChannelERRNO_FAIL = -1,
	ChannelERRNO_SUCCESS = 0,
	ChannelERRNO_ALREADYJOINED = 1,
	ChannelERRNO_NOSTATUSCHANGE = 2
}	t_ChannelERRNO;

class Channel
{
private:
	std::string								_channelName;
	std::vector<const Client* >				_regularClients;
	std::vector<const Client* >				_operatorClients;
	std::vector<const Client* >				_invitedClients;
	size_t									_limit;
	std::string								_channelKey;
	std::string								_topic;
	bool									_inviteOnly;
	bool									_disallowTopicChange;

	bool									_removeRegularClient(const Client *client);
	bool									_removeOperatorClient(const Client *client);

	std::vector<const Client* >::iterator	_getRegularClientPos(const Client *client);
	std::vector<const Client* >::iterator	_getOperatorClientPos(const Client *client);
	std::vector<const Client* >::iterator	_getInvitedPos(const Client *client);

	Channel(void);
	Channel(const Channel& copy);

	Channel&							operator=(const Channel& src);
public:
	Channel(const std::string& channelName);
	~Channel(void);

	static bool							checkValidChannelName(const std::string& channel_name);
	static bool							checkValidChannelKey(const std::string& channel_key);

	t_ChannelERRNO						addClient(const Client* client,
											const std::string& password);
	bool								removeClient(const Client* client);
	bool								checkJoined(const Client* client) const;

	std::string							stringRegularClients(void) const;
	std::string							stringOperatorClients(void) const;
	bool								checkIsRegularClient(const Client *client) const;
	bool								checkIsOperatorClient(const Client *client) const;
	t_ChannelERRNO						addOperatorStatus(const Client *client);
	t_ChannelERRNO						removeOperatorStatus(const Client *client);
	void 								addInvitedClient(const Client* client);

	//getter
	const std::string&					getChannelName(void) const;
	const std::vector<const Client *>&	getRegularClients(void) const;
	const std::vector<const Client *>&	getOperatorClients(void) const;
	size_t								getLimit(void) const;
	const std::string&					getChannelKey(void) const;
	bool								getInviteOnly(void) const;
	const std::string&					getTopic(void) const;
	bool								getDisallowTopicChange(void) const;
	size_t								getChannelSize(void) const;

	//setter
	void								setLimit(size_t limit);
	void								setChannelKey(const std::string& channel_key);
	void 								setTopic(const std::string& topic);
	void								setInviteOnly(bool dis_allow);
	void								setDisallowTopicChange(bool dis_allow);
};


#include "../include/CommandUser.hpp"

CommandUser::CommandUser(Server* server, Client* client) : ACommand("USER", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandUser::~CommandUser(void)
{
	delete this->_reply;
	return ;
}

bool isValidUsername(const std::string& username) 
{
	if (username.empty()) {
		return (false);
	}
	if (username.size() > 31) {
		return (false);
	}
	for (std::string::const_iterator it = username.begin(); it != username.end(); ++it) 
	{
		char c = *it;
		if (c == '\0' || c == '\r' || c == '\n' || c == ' ' || c == '@')
			return (false);
		if (c < 0x01 || (c > 0x09 && c < 0x0B) || (c > 0x0C && c < 0x0E) ||
			(c > 0x1F && c < 0x21) || (c > 0x3F && c < 0x41))
			return (false);
	}
	return (true);
}

bool	CommandUser::execute(const std::vector<std::string>& request_args)
{
	if (request_args.size() < 2)
		return (this->_reply->errNeedmoreparams("USER"));
	if (!this->_client->getEnteredPassword())
		return (this->_reply->rplEnterpassword(request_args[0]));
	// check if valid
	const std::string& new_username = request_args[1];
	if (!isValidUsername(new_username))
		return(this->_reply->rplCustom(this->_client, this->_client, "USER", ": Not a valid Username"));
	// check if already in use
	bool username_in_use = false;
	const std::vector<Client*>& clients = this->_server->getClients();
	for (size_t i = 0; i < clients.size(); ++i) 
	{
		Client* client = clients[i];
		if (client != this->_client && client->getUserName() == new_username)
			username_in_use = true;
	}
	if (username_in_use)
		return (this->_reply->errAlreadyregistered());
	else
	{
		this->_client->setUserName(request_args[1]);
		this->_reply->rplSetuser();
	}
	if (this->_client->getEnteredNickname() && this->_client->getEnteredUsername())
	{
		this->_client->setIsRegistered();
		return (this->_reply->rplWelcome());
	}
	return (true);
}

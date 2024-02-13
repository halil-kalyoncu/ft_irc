
#include "../include/CommandNick.hpp"

CommandNick::CommandNick(Server* server, Client* client) : ACommand("NICK", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandNick::~CommandNick(void)
{
	delete this->_reply;
	return ;
}

bool isValidNicknameChar(char c) 
{
    return (isalpha(c) || isdigit(c) || (c == '[') || (c == ']') || (c == '\\') || (c == '`') ||
		(c == '_') || (c == '^') || (c == '{') || (c == '|') || (c == '}') || (c == '-'));
}

bool isValidNickname(const std::string& nickname) 
{
	std::string special_chars = "[]\\`_^{|}";
	std::size_t found = nickname.find_first_of(special_chars);
	if (nickname.size() > 9)
		return (false);
	if (!(isalpha(nickname[0]) || found == 0))
		return (false);
	for (size_t i = 1; i < nickname.size(); i++) 
	{
		if (!isValidNicknameChar(nickname[i])) 
			return (false);
	}
	return (true);
}

bool	CommandNick::execute(const std::vector<std::string>& request_args)
{
	Reply	reply(this->_server, this->_client);

	if (request_args.size() < 2)
		return (this->_reply->errNeedmoreparams(request_args[0]));
	if (!this->_client->getEnteredPassword())
		return (this->_reply->rplEnterpassword(request_args[0]));
	const std::string& new_nickname = request_args[1];
	// check if nick is erroneus
	if (!isValidNickname(new_nickname)) {
		return (this->_reply->errErroneusnickname(new_nickname));
	}
	// check if already in use
	bool nickname_in_use = false;
	const std::vector<Client*>& clients = this->_server->getClients();
	for (size_t i = 0; i < clients.size(); ++i) 
	{
		Client* client = clients[i];
		if (client != this->_client && client->getNickName() == new_nickname)
			nickname_in_use = true;
	}
	if (nickname_in_use)
		return (this->_reply->errNicknameinuse(new_nickname));
	else
	{
		this->_client->setNickName(request_args[1]);
		this->_reply->rplSetnick();
	}
	if (this->_client->getEnteredNickname() && this->_client->getEnteredUsername())
	{
		this->_client->setIsRegistered();
		return (reply.rplWelcome());
	}
	return (true);
}
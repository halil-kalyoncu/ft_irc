
#include "../include/CommandPass.hpp"

CommandPass::CommandPass(Server* server, Client* client) : ACommand("PASS", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandPass::~CommandPass(void)
{
	delete this->_reply;
	return ;
}

bool	CommandPass::execute(const std::vector<std::string>& request_args)
{
	Reply	reply(this->_server, this->_client);

	if (request_args.size() < 2)
	{
		return (this->_reply->errNeedmoreparams(request_args[0]));
	}
	if (this->_server->checkPassword(request_args[1]) == false)
	{
		return (this->_reply->errPasswdmismatch(request_args[0]));
	}
	this->_client->setEnteredPassword();
	return (true);
}

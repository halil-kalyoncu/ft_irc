
#include "../include/CommandQuit.hpp"

CommandQuit::CommandQuit(Server* server, Client* client) : ACommand("QUIT", server, client)
{
	this->_reply = new Reply(server, client);
	return ;
}

CommandQuit::~CommandQuit(void)
{
	delete this->_reply;
	return ;
}

bool	CommandQuit::execute(const std::vector<std::string>& request_args)
{
	std::string reason;

	 // if a reason is specified, extract it from the message
    if (request_args.size() > 1) {
        reason = request_args[1];
    }
	// send error message to leaving client
	std::ostringstream	osstr;
	std::string	quit;
	osstr << "[Client exited] - Goodbye!" << std::endl;
	quit = osstr.str();
	send(this->_client->getSocketFd(), quit.c_str(), quit.length(), 0);

	// notify other clients
	std::ostringstream	oss;
	std::string			quit_message;
	oss << this->_client->getNickName() << "!" << this->_client->getUserName();
	oss << "@" << this->_client->getHostName() << " " << request_args[0] << " ";
	if (!reason.empty()) {
		oss << " :" << reason;
	}
	oss << std::endl;
	quit_message = oss.str();

	// broadcast quit_message to all other clients
	std::vector<Client*>::const_iterator it = this->_server->getClients().begin();
	while (it != this->_server->getClients().end()) 
	{
		if (*it != this->_client)
			send((*it)->getSocketFd(), quit_message.c_str(), quit_message.length(), 0);
		++it;
	}
	close(this->_client->getSocketFd());
	delete this->_server->eraseClientEntry(this->_client->getSocketFd());

	return (true);
}
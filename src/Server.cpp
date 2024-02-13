
#include "../include/Server.hpp"

const std::string	Server::_commandNames[11] =	{"PASS",
												"NICK",
												"USER",
												"JOIN",
												"PART",
												"PRIVMSG",
												"KICK",
												"INVITE",
												"TOPIC",
												"MODE",
												"QUIT"};

/**
 * Converts lowercase into uppercase letters inside a string.
 *
 * @param	input String that has to be converted.
 *
 * @return	A new string with the correct format.
 */
std::string	Server::normalizeInput(const std::string& input)
{
	std::string	result = input;

	std::transform(input.begin(), input.end(), result.begin(), ::toupper);
	return (result);
}

void Server::_printLog(const std::string &message)
{
	std::cout << message << std::endl;
}

// printing an error message and exiting the process
void Server::_exit(const std::string &error_message)
{
	if (this->_listenFd != -1) {
		close(this->_listenFd);
	}
	this->_printLog("ERROR: " + error_message);
	exit(-1);
}

Server::Server(const int port, const std::string password) : _port(port),
	_password(password), _serverName("irc42.com"), _listenFd(-1), _nfds(0),
	_timeout(3 * 60 * 1000), _request(""), _startupTime(std::time(NULL))
{
	this->_startServer(port);
	this->_runServer();
}

void	Server::_startServer(int port)
{
	this->_listenFd = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->_listenFd < 0)
	{
		this->_exit("socket() failed");
	}

	int opt_value = 1;
	int ret = setsockopt(this->_listenFd, SOL_SOCKET,  SO_REUSEADDR,
					&opt_value, sizeof(opt_value));
	if (ret < 0)
	{
		this->_exit("setsockopt() failed");
	}

	ret = fcntl(this->_listenFd, F_SETFL, O_NONBLOCK);
	if (ret < 0)
	{
		this->_exit("fcntl() failed");
	}

	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port = htons(port);
	ret = bind(this->_listenFd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
	{
		this->_exit("bind() failed. Please check if you are already using this port.");
	}
	else
	{
		std::cout << "\n\033[33m*** Server listening on PORT " << port << " ***\033[0m\n";
	}

	ret = listen(this->_listenFd, 32);
	if (ret < 0)
	{
		this->_exit("listen() failed");
	}
	this->_nfds++;
	return ;
}

void Server::_compressFds(void)
{
	bool	compress_array = true;

	if (this->_nfds == 0) {
		return;
	}

	while (compress_array)
	{
		compress_array = false;
		for (int i = 0; i < this->_nfds; i++)
		{
			if (this->_fds[i].fd == -1)
			{
				for (int j = i; j < this->_nfds; j++)
				{
					this->_fds[j].fd = this->_fds[j + 1].fd;
				}
				i--;
				this->_nfds--;
				compress_array = true;
			}
		}
	}
	return ;
}

bool Server::_handleNewConnection(void)
{
	int	new_fd = accept(this->_listenFd, NULL, NULL);

	if (new_fd < 0)
	{
		if (errno != EWOULDBLOCK) {
			this->_printLog("Error: accept() failed");
		}
		return (false);
	}

	// std::cout << "New incoming connection - " << new_fd << std::endl;
	this->_fds[this->_nfds].fd = new_fd;
	this->_fds[this->_nfds].events = POLLIN;
	this->_nfds++;
	this->_clients.push_back(new Client(new_fd));
	return (true);
}

void	Server::_receiveRequest(int fds_id)
{
	char	buffer[80];
	int		ret = 0;

	//receive request
	memset(buffer, 0, sizeof(buffer));
	ret = recv(this->_fds[fds_id].fd, buffer, sizeof(buffer), 0);
	if (ret < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			this->_printLog("Error: recv() failed");
			this->_closeConnection(fds_id);
		}
		return ;
	}

	//check to see if client closed connection
	if (ret == 0)
	{
		this->_printLog("Connection closed");
		this->_closeConnection(fds_id);
		return ;
	}

	this->_request.append(std::string(buffer));

	if (this->_request.at(this->_request.size() - 1) == '\n') {
		if (this->_request.size() > 1) {
			//handle request
			if (this->_parseRequest(this->_fds[fds_id].fd, this->_request) == false) {
				this->_closeConnection(fds_id);
				return ;
			};
		}
		this->_request.clear();
	}
	else if (this->_request.at(this->_request.size() - 1) == 4) {
		this->_request.erase(this->_request.size() - 1);
	}
	return ;
}


void	Server::_closeConnection(int fds_id)
{
	close(this->_fds[fds_id].fd);
	delete this->eraseClientEntry(this->_fds[fds_id].fd);
	this->_fds[fds_id].fd = -1;
	return ;
}

void	Server::_closeOpenSockets(void)
{
	for (int i = 0; i < this->_nfds; i++)
	{
		if(this->_fds[i].fd >= 0) {
			close(this->_fds[i].fd);
			delete this->eraseClientEntry(this->_fds[i].fd);
		}
	}
	return ;
}

void	Server::_runServer(void)
{
	int	ret = 0;
	int	current_size;

	std::memset(this->_fds, 0 , sizeof(this->_fds));
	
	this->_fds[0].fd = this->_listenFd;
	this->_fds[0].events = POLLIN;

	while (true)
	{
		ret = poll(this->_fds, this->_nfds, _timeout);

		if (ret < 0)
		{
			//this->_printLog("Error: poll() failed");
			break;
		}

		// check if timeout has been reached
		if (ret == 0)
		{
			this->_printLog("Error: poll() timed out. End of program reached.");
			break;
		}
	
		current_size = this->_nfds;
		for (int i = 0; i < current_size; i++)
		{
			if (this->_fds[i].revents == 0) {
				continue;
			}

			if(this->_fds[i].revents != POLLIN)
			{
				//std::cout << "Error! revents = "<< this->_fds[i].revents << std::endl;
				if(this->_fds[i].revents == POLLHUP) {
					this->_closeConnection(i);
					continue ;
				}
			}
			if (this->_fds[i].fd == this->_listenFd)
			{
				//this->_printLog("Listening socket is readable");

				if (this->_handleNewConnection() == false) {
					break ;
				}
			}
			else
			{
				//std::cout << "Descriptor "<< this->_fds[i].fd << " is readable" << std::endl;
				this->_receiveRequest(i);
			}
		}
		this->_compressFds();
	}
}

Client*	Server::_getClient(int client_fd)
{
	std::vector<Client *>::iterator	it = this->_clients.begin();

	while (it != this->_clients.end()) {
		if ((*it)->getSocketFd() == client_fd) {
			return (*it);
		}
		it++;
	}
	return (NULL);
}

Client*	Server::eraseClientEntry(int client_fd)
{
	std::vector<Client* >::iterator		it_client = this->_getClientPos(client_fd);
	std::vector<Channel* >::iterator	it_channel = this->_channels.begin();
	size_t								size_channel;
	bool								client_exited;
	Client*								erased_client;

	if (it_client == this->_clients.end()) {
		return (NULL);
	}

	erased_client = *it_client;
	while (it_channel != this->_channels.end()) {
		size_channel = (*it_channel)->getChannelSize();
		client_exited = this->exitChannel(erased_client, *it_channel);
		if (size_channel == 1 && client_exited == true) {
			it_channel = this->_channels.begin();
			if (it_channel == this->_channels.end()) {
				break ;
			}
			continue ;
		}
		it_channel++;
	}

	this->_clients.erase(it_client);
	return (erased_client);
}

std::vector<std::string>	Server::_splitRequest(std::string client_request)
{
	std::vector<std::string>	request_args;
	size_t						pos_split;
	std::string					token;

	if (client_request.empty()) {
		return (request_args);
	}

	client_request.erase(0, client_request.find_first_not_of(" \t\r\n"));
	client_request.erase(client_request.find_last_not_of(" \t\r\n") + 1);

	pos_split = client_request.find_first_of(' ');
	while (pos_split != client_request.npos) {
		if (client_request[0] == ':') {
			request_args.push_back(client_request.substr(1));
			client_request.clear();
			break ;
		}

		token = client_request.substr(0, pos_split);
		if (token.empty() == false) {
			request_args.push_back(token);
		}

		client_request.erase(0, pos_split + 1);
		pos_split = client_request.find_first_of(' ');
	}

	if (client_request[0] == ':') {
		client_request = client_request.substr(1);
		request_args.push_back(client_request);
	}
	else if (client_request.empty() == false) {
		request_args.push_back(client_request);
	}

	request_args[0] = Server::normalizeInput(request_args[0]);
	return (request_args);
}

ACommand*	Server::_identifyCommand(const std::string& request_name, Client* client)
{
	int	command_id = 0;

	while (command_id < 11) {
		if (Server::_commandNames[command_id] == request_name) {
			break ;
		}
		command_id++;
	}
	switch (command_id) {
	case 0:
		return (new CommandPass(this, client));
	case 1:
		return (new CommandNick(this, client));
	case 2:
		return (new CommandUser(this, client));
	case 3:
		return (new CommandJoin(this, client));
	case 4:
		return (new CommandPart(this, client));
	case 5:
		return (new CommandPrivmsg(this, client));
	case 6:
		return (new CommandKick(this, client));
	case 7:
		return (new CommandInvite(this, client));
	case 8:
		return (new CommandTopic(this, client));
	case 9:
		return (new CommandMode(this, client));
	case 10:
		return (new CommandQuit(this, client));
	}
	return (NULL);
}

bool	Server::_parseRequest(int client_fd, const std::string& client_request)
{
	Client*						request_client = this->_getClient(client_fd);
	std::vector<std::string>	request_args;
	ACommand*					command;
	Reply						reply_unknown(this, request_client);
	bool						success_request;

	request_args = Server::_splitRequest(client_request);
	command = Server::_identifyCommand(request_args[0], request_client);
	if (command == NULL) {
		//check if client has successfully entered server
		if (!request_client->getIsRegistered())
			return (true);
		else
			return (reply_unknown.errUnknowncommand(request_args[0]));
	}
	success_request = command->execute(request_args);
	delete command;
	return (success_request);
}

std::vector<Channel* >::iterator	Server::_getChannelPos(const std::string& channel_name)
{
	std::vector<Channel* >::iterator	it = this->_channels.begin();
	std::string							input_normalized;
	std::string							entry_normalized;

	input_normalized = Server::normalizeInput(channel_name);
	while (it != this->_channels.end()) {
		entry_normalized = Server::normalizeInput((*it)->getChannelName());
		if (entry_normalized == input_normalized) {
			break ;
		}
		it++;
	}
	return (it);
}

void	Server::_deleteChannel(const std::string& channel_name)
{
	std::vector<Channel *>::iterator	it = this->_getChannelPos(channel_name);
	Channel*							channel = *it;

	this->_channels.erase(it);
	delete channel;
	return ;
}

std::vector<Client* >::iterator	Server::_getClientPos(int client_fd)
{
	std::vector<Client *>::iterator	it = this->_clients.begin();

	while (it != this->_clients.end()) {
		if ((*it)->getSocketFd() == client_fd) {
			break ;
		}
		it++;
	}
	return (it);
}

std::vector<Client* >::iterator	Server::_getClientPos(const std::string& nick_name)
{
	std::vector<Client* >::iterator		it =this->_clients.begin();
	std::string							input_normalized;
	std::string							entry_normalized;

	input_normalized = Server::normalizeInput(nick_name);
	while (it != this->_clients.end()) {
		entry_normalized = Server::normalizeInput((*it)->getNickName());
		if (entry_normalized == input_normalized) {
			break ;
		}
		it++;
	}
	return (it);
}

Server::~Server()
{
	this->_closeOpenSockets();
	this->deleteChannels();
	return ;
}

bool	Server::checkPassword(const std::string& client_input) const
{
	return (client_input == this->_password);
}

//Channel functions
Channel*	Server::getChannel(const std::string& channel_name)
{
	std::vector<Channel* >::iterator	it = this->_getChannelPos(channel_name);

	if (it == this->_channels.end()) {
		return (NULL);
	}
	return ((*it));
}

Channel*	Server::createChannel(const std::string& channel_name)
{
	Channel*	new_channel = new Channel(channel_name);

	this->_channels.push_back(new_channel);
	return (new_channel);
}

t_ChannelERRNO	Server::joinChannel(Client* client, Channel* channel,
	const std::string& password)
{
	if (client == NULL || channel == NULL) {
		return (ChannelERRNO_FAIL);
	}
	return (channel->addClient(client, password));
}

bool	Server::exitChannel(Client* client, Channel* channel)
{
	bool	clientWasOnChannel;
	size_t	channel_size;

	if (client == NULL || channel == NULL) {
		return (false);
	}

	channel_size = channel->getChannelSize();
	clientWasOnChannel = channel->removeClient(client);
	if (channel_size == 1 && clientWasOnChannel == true) {
		this->_deleteChannel(channel->getChannelName());
	}
	return (clientWasOnChannel);
}

void	Server::deleteChannels(void)
{
	std::vector<Channel* >::iterator	it = this->_channels.begin();

	while (it != this->_channels.end()) {
		delete (*it);
		it++;
	}
	return ;
}

Client*	Server::getClient(const std::string& nick_name)
{
	std::vector<Client* >::iterator	it = this->_getClientPos(nick_name);

	if (it == this->_clients.end()) {
		return (NULL);
	}
	return ((*it));
}

//getter
const std::string&	Server::getServerName(void) const
{
	return (this->_serverName);
}

const std::vector<Client*>& Server::getClients(void) const 
{
	return (_clients);
}

const std::vector<Channel*>& Server::getChannels(void) const 
{
	return (_channels);
}

std::time_t Server::getTime(void) const
{
	return (this->_startupTime);
}

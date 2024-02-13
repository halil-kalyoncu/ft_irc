
#include "../include/Reply.hpp"

std::string	Reply::_replyPrefix(const std::string& code) const
{
	std::ostringstream	oss;

	oss << ":" << this->_server->getServerName() << " " << code << " ";
	oss << this->_client->getNickName() << " ";
	return (oss.str());
}

std::string	Reply::_advancedName(const Client* client)
{
	std::ostringstream	oss;

	oss << client->getNickName() << "!" << client->getUserName();
	oss << "@" << client->getHostName();
	return (oss.str());
}

Reply::Reply(void)
{
	return ;
}

Reply::Reply(const Server* server, const Client* client) : _server(server),
	_client(client)
{
	return ;
}

Reply::Reply(const Reply& copy) : _server(copy._server), _client(copy._client)
{
	return ;
}

Reply::~Reply(void)
{
	return ;
}

Reply&	Reply::operator=(const Reply& src)
{
	if (this != &src) {
		this->_server = src._server;
		this->_client = src._client;
	}
	return (*this);
}

bool	Reply::sendMultiReply(const std::vector<const Client* >& client_list,
								const Client* client_about,
								const std::string& command_name,
								const std::string& additional_message)
{
	std::vector<const Client* >::const_iterator	it = client_list.begin();

	while (it != client_list.end()) {
		if (*it == client_about) {
			it++;
			continue;
		}
		if (Reply::rplCustom(*it, client_about, command_name, additional_message) == false) {
			return (false);
		}
		it++;
	}
	return (true);
}

bool	Reply::rplCustom(const Client* client_to, const Client* client_about,
							const std::string& command_name,
							const std::string& additional_message)
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << Reply::_advancedName(client_about) << " " << command_name << " :" << additional_message << std::endl;
	replyMessage = oss.str();
	return (send(client_to->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplSetnick(void) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << ":" << this->_server->getServerName() << " NICK " << this->_client->getNickName() << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplWelcome(void) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	// current date and time 
	std::time_t serverTime = this->_server->getTime();
	std::tm *localTime = std::localtime(&serverTime);

	oss << this->_replyPrefix("001") << ":Welcome to the Internet Relay Network ";
	oss << Reply::_advancedName(this->_client) << std::endl;
	oss << this->_replyPrefix("002") << ":Your host is irc42.com, running version ircserv-1" << std::endl;
	char* buffer = new char[80];
	std::strftime(buffer, 80, "%H:%M:%S %b %e %Y", localTime);
	oss << this->_replyPrefix("003") <<":This server was created " << buffer << std::endl;
	oss << this->_replyPrefix("004") << ":irc42.com ircserv-1 - :iklot" << std::endl;
	replyMessage = oss.str();
	delete[] buffer;
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplNamreply(const Channel* channel) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("353");
	if (channel->getInviteOnly() == true) {
		oss << "* ";
	}
	else {
		oss << "= ";
	}
	oss << channel->getChannelName() << " " << channel->stringOperatorClients();
	oss << channel->stringRegularClients() << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplEndofnames(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("366");
	oss << channel_name << " :End of NAMES list" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNosuchnick(const std::string& nick_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("401");
	oss << nick_name << " :No such nick/channel" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNosuchchannel(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("403");
	oss << channel_name << " :No such channel" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errToomanytargets(const std::string& nick, size_t targets,
	size_t max_targets) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("407");
	oss << nick << " :" << targets << " recipients. Can only handle " << max_targets;
	oss << max_targets << " recipients with one call" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNorecipient(const std::string& command_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("411");
	oss << ":No such channel (" << command_name << ")" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNotexttosend(void) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("412");
	oss << ":No text to send" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errUnknowncommand(const std::string& command_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("421");
	oss << command_name << " :Unknown command" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNonicknamegiven(void) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("431");
	oss << ":No nickname given" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errErroneusnickname(const std::string& new_nickname) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("432");
	oss << new_nickname << " :Erroneous nickname" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNicknameinuse(const std::string& new_nickname) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("433");
	oss << new_nickname << " :Nickname is already in use" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errUsernotonchannel(const std::string& nickname, const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("441");
	oss << nickname << " " << channel_name << " :They aren't on that channel" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNotonchannel(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("442");
	oss << channel_name << " :You're not on that channel" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNeedmoreparams(const std::string& command_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("461");
	oss << command_name << " :Not enough parameters" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errAlreadyregistered(void) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("462");
	oss << ":Unauthorized command (already registered)" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errKeyset(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("467");
	oss << channel_name << " :Channel key already set" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errChannelisfull(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("471");
	oss << channel_name << " :Cannot join channel (+l)" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errUnknownmode(const std::string& mode, const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("472");
	oss << mode << " :is unknown mode char to me for " << channel_name << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errInviteonlychan(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("473");
	oss << channel_name << " :Cannot join channel (+i)" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errBadchannelkey(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("475");
	oss << channel_name << " :Cannot join channel (+k)" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errBadchanmask(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("476");
	oss << channel_name << " :Bad Channel Mask" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplSetuser(void) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << ":" << this->_server->getServerName() << " USER " << this->_client->getUserName() << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errUseronchannel(const std::string& nick, const std::string& nickname_to_invite, const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("443");
	oss << nick << " " << nickname_to_invite << " " << channel_name << " :is already on channel"<< std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errSomethingwentwrong(void) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("600");
	oss << ":An internal error occured.";
	oss << "  If the problem persists, contact your administrator" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errWrongChannelKey(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("601");
	oss << channel_name << " k * :Invalid key mode parameter. Syntax: <key>." << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNolimitgiven(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("602");
	oss << channel_name << " l * :No limit given" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errWronglimit(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("603");
	oss << channel_name << " l * :Invalid limit mode parameter. Syntax: <limit>" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errChanoprivsneeded(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("482");
	oss << channel_name << " :You're not channel operator" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplInviting(const std::string& channel_name, const std::string& nick_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("341");
	oss << nick_name << " :" << channel_name << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplInvited(const Client* client_to, const Client* client_about,
							const std::string& command_name, const std::string& nickname_to_invite,
							const std::string& channel_name)
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << Reply::_advancedName(client_about) << " " << command_name << " ";
	oss << nickname_to_invite << " :" << channel_name << std::endl;
	replyMessage = oss.str();
	return (send(client_to->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplNotopic(const std::string& channel_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("331");
	oss << channel_name << " :No topic is set"<< std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplTopic(const std::string& channel_name, const std::string& topic) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("332");
	oss << "TOPIC " << channel_name << " :" << topic << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::sendMultiReplyExtra(const std::vector<const Client* >& client_list,
								const Client* client_about,
								const std::string& command_name,
								const std::string& channel_name,
								const std::string& additional_message)
{
	std::vector<const Client* >::const_iterator	it = client_list.begin();

	while (it != client_list.end()) {
		if (Reply::rplCustomExtra(*it, client_about, command_name, channel_name, additional_message) == false) {
			return (false);
		}
		it++;
	}
	return (true);
}

bool	Reply::rplCustomExtra(const Client* client_to, const Client* client_about,
							const std::string& command_name,
							const std::string& channel_name,
							const std::string& additional_message)
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << Reply::_advancedName(client_about) << " " << command_name << " " << channel_name << " :" << additional_message << std::endl;
	replyMessage = oss.str();
	return (send(client_to->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errNotregistered(const std::string& command_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("451") << command_name;
	oss << " :You have not registered" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::errPasswdmismatch(const std::string& command_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << this->_replyPrefix("464") << command_name;
	oss << " :Password incorrect" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

bool	Reply::rplEnterpassword(const std::string& command_name) const
{
	std::ostringstream	oss;
	std::string			replyMessage;

	oss << ":" << this->_server->getServerName() << " " << command_name << " :Please enter password" << std::endl;
	replyMessage = oss.str();
	return (send(this->_client->getSocketFd(), replyMessage.c_str(),
		replyMessage.length(), 0) >= 0);
}

#pragma once

//string
#include <string>
//ostringstream
#include <sstream>
//send
#include <sys/socket.h>
//current date
#include <ctime> 

#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Reply
{
private:
	const Server*	_server;
	const Client*	_client;

	std::string			_replyPrefix(const std::string& code) const;
	//<nick>!<user>@<host>
	static std::string	_advancedName(const Client* client);
	Reply(void);
	Reply(const Reply& copy);
	Reply&	operator=(const Reply& src);
public:
	Reply(const Server* server, const Client* client);
	~Reply(void);

	static bool		sendMultiReply(const std::vector<const Client* >& client_list,
										const Client* client_about,
										const std::string& command_name,
										const std::string& additional_message);
	
	static bool		sendMultiReplyExtra(const std::vector<const Client* >& client_list,
										const Client* client_about,
										const std::string& command_name,
										const std::string& channel_name,
										const std::string& additional_message);

	//no code
	static bool		rplCustom(const Client* client_to, const Client* client_about,
								const std::string& command_name,
								const std::string& additional_message);

	static bool		rplCustomExtra(const Client* client_to, const Client* client_about,
								const std::string& command_name,
								const std::string& channel_name,
								const std::string& additional_message);

	bool			rplSetnick(void) const;
	bool			rplSetuser(void) const;
	bool			rplInvited(const Client* client_to, const Client* client_about,
							const std::string& command_name, const std::string& nickname_to_invite,
							const std::string& channel_name);
	bool			rplEnterpassword(const std::string& command_name) const;

	//001
	bool			rplWelcome(void) const;
	//331
	bool			rplNotopic(const std::string& channel_name) const;
	//332
	bool			rplTopic(const std::string& channel_name, const std::string& topic) const;
	//341
	bool			rplInviting(const std::string& channel_name, const std::string& nick_name) const;
	//353
	bool			rplNamreply(const Channel* channel) const;
	//366
	bool			rplEndofnames(const std::string& channel_name) const;
	//401
	bool			errNosuchnick(const std::string& nick_name) const;
	//403
	bool			errNosuchchannel(const std::string& channel_name) const;
	//407
	bool			errToomanytargets(const std::string& nick, size_t targets, size_t max_targets) const;
	//411
	bool			errNorecipient(const std::string& command_name) const;
	//412
	bool			errNotexttosend(void) const;
	//421
	bool			errUnknowncommand(const std::string& command_name) const;
	//431
	bool			errNonicknamegiven(void) const;
	//432
	bool			errErroneusnickname(const std::string& new_nickname) const;
	//433
	bool			errNicknameinuse(const std::string& new_nickname) const;
	//441
	bool			errUsernotonchannel(const std::string& nickname, const std::string& channel_name) const;
	//442
	bool			errNotonchannel(const std::string& channel_name) const;
	//443
	bool			errUseronchannel(const std::string& nick, const std::string& nickname_to_invite, const std::string& channel_name) const;
	//451
	bool			errNotregistered(const std::string& command_name) const;
	//461
	bool			errNeedmoreparams(const std::string& command_name) const;
	//462
	bool			errAlreadyregistered(void) const;
	//464
	bool			errPasswdmismatch(const std::string& command_name) const;
	//467
	bool			errKeyset(const std::string& channel_name) const;
	//471
	bool			errChannelisfull(const std::string& channel_name) const;
	//472
	bool			errUnknownmode(const std::string& mode, const std::string& channel_name) const;
	//473
	bool			errInviteonlychan(const std::string& channel_name) const;
	//475
	bool			errBadchannelkey(const std::string& channel_name) const;
	//476
	bool			errBadchanmask(const std::string& channel_name) const;
	//482
	bool			errChanoprivsneeded(const std::string& channel_name) const;
	//600
	bool			errSomethingwentwrong(void) const;
	//601
	bool			errWrongChannelKey(const std::string& channel_name) const;
	//602
	bool			errNolimitgiven(const std::string& channel_name) const;
	//603
	bool			errWronglimit(const std::string& channel_name) const;
};


#pragma once

//string
#include <string>
//vector
#include <vector>

class Server;
class Client;
class Reply;

class ACommand
{
protected:
	const std::string				_commandName;
	Server*							_server;
	Client*							_client;
	Reply*							_reply;

	static bool						_checkValidSingleArg(const std::string& arg);
	static std::vector<std::string>	_splitArgument(std::string arg);

	ACommand(void);
	ACommand(const ACommand& copy);

	ACommand&						operator=(const ACommand& src);
public:
	ACommand(const std::string& command_name, Server* server, Client* client);
	virtual ~ACommand(void);

	virtual bool					execute(const std::vector<std::string>& request_args) = 0;
};

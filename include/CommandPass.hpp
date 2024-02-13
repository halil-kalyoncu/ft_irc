
#pragma once

//string
#include <string>
//vector
#include <vector>

#include "ACommand.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Reply.hpp"

class CommandPass : public ACommand
{
private:
	CommandPass(void);
	CommandPass(const CommandPass& copy);
	CommandPass&	operator=(const CommandPass& src);
public:
	CommandPass(Server* server, Client* client);
	~CommandPass(void);

	bool			execute(const std::vector<std::string>& request_args);
};


#include "../include/ACommand.hpp"

/**
 * Checks argument for commands with only one argument.
 *
 * @param	arg Argument to be checked (request_args[1])
 */
bool	ACommand::_checkValidSingleArg(const std::string& arg)
{
	return (arg.find_first_of(" :,\r\n"));
}

std::vector<std::string>	ACommand::_splitArgument(std::string arg)
{
	std::vector<std::string>	split_args;
	size_t						pos_split;
	std::string					token;

	if (arg.empty() == true) {
		return (split_args);
	}

	pos_split = arg.find_first_of(',');
	while (pos_split != arg.npos) {
		token = arg.substr(0, pos_split);
		if (token.empty() == false) {
			split_args.push_back(token);
		}

		arg = arg.erase(0, pos_split + 1);
		pos_split = arg.find_first_of(',');
	}
	if (arg.empty() == false) {
		split_args.push_back(arg);
	}
	return (split_args);
}

ACommand::ACommand(const std::string& command_name, Server* server,
	Client* client) : _commandName(command_name), _server(server), _client(client),
	_reply(NULL)
{
	return ;
}

ACommand::~ACommand(void)
{
	return ;
}

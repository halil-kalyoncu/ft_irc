/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpsyk <lpsyk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:07:10 by lpsyk             #+#    #+#             */
/*   Updated: 2023/05/10 17:47:22 by lpsyk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

// SIGINT/CTRL + C
#include <signal.h>

static void	signal_handler(int signal)
{
	(void)signal;
	std::cout << "\033[31m *** Shutting down Server ***\033[0m\n";
}

int	main (int argc, char *argv[])
{
	if (argc == 3)
	{
		// reading in & setting port & password
		const int port = std::atoi(argv[1]);
		const std::string password = argv[2];
		// SIGINT handling
		signal(SIGINT, signal_handler);
		// constructing
		Server Server(port, password);
		return (0);
	}
	else
	{
		std::cout << "\n*** Please, start the Server like this: \033[33m./ircserv [port] [password]\033[0m ***\n\n";
		return (1);
	}
}
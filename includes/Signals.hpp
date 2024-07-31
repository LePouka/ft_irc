#pragma once

#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include <csignal>

int		set_signals( void );
void	signals_handler( int signal );

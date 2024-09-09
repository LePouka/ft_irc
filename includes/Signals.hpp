#pragma once

#include "Client.hpp"
#include "Server.hpp"
#include <csignal>

int		set_signals( void );
void	signals_handler( int signal );

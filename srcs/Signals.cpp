#include "../includes/Signals.hpp"

bool g_status;

void	mySignalHandler( int signo ) {

	( void )signo;

	std::cout << std::endl << "Closing ircserv..." << std::endl;

	g_status = false;
}

bool	setSignals( void ) {

	g_status = true;

	struct	sigaction act;

	act.sa_flags = SA_SIGINFO | SA_RESTART;
	act.sa_handler = &mySignalHandler;

	if ( sigaction( SIGINT, &act, NULL ) == -1 ||
		sigaction( SIGTSTP, &act, NULL ) == -1 ||
		sigaction( SIGQUIT, &act, NULL ) == -1 ) {

		return false;
	}

	return true;
}

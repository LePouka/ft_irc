#include "../includes/Signals.hpp"

bool	g_signals;

int	set_signals( void ) {

	g_signals = true;
	struct sigaction	signal;

	memset( &signal, 0, sizeof( signal ));
	signal.sa_flags = SA_SIGINFO | SA_RESTART;
	if ( sigaction( SIGINT, &signal, NULL ) == -1 \
		|| sigaction( SIGQUIT, &signal, NULL ) == -1 \
		|| sigaction( SIGTSTP, &signal, NULL ) == -1 )
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

void	signals_handler( int signal ) {

	( void )signal;
	g_signals = false;
}

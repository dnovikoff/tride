#include <csignal>
#include <iostream>
#include <stdexcept>
#include "signals.hpp"
#include "fcgi_server.hpp"

namespace tride {

namespace {

typedef void (*signalhandler_t) (int);
signalhandler_t oldSignalHandler = NULL;

void signalHandler( int signum ) {
	signal(SIGINT, oldSignalHandler);
	std::cout << "Interrupt signal (" << signum << ") received.\n";
	FcgiServer::currentStop();
}

} // namespace

void Signals::init() {
	oldSignalHandler = signal(SIGINT, signalHandler);
	if( oldSignalHandler == SIG_ERR) {
		oldSignalHandler = NULL;
		throw std::runtime_error("Error setting up signal handlers");
	}
}

} // namespace tride

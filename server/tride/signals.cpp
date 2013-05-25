#include <csignal>
#include <iostream>
#include <stdexcept>
#include "signals.hpp"
#include "fcgi_server.hpp"

namespace tride {

namespace {

typedef void Func(int);
void installSignal(int signo, Func* func) {
	struct sigaction act, old_act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = func;
	sigaction(signo, &act, &old_act);
}

void signalHandler( int signum ) {
	std::cout << "Interrupt signal (" << signum << ") received.\n";
	FcgiServer::currentStop();
}

} // namespace

void Signals::init() {
	installSignal(SIGINT, signalHandler);
}

} // namespace tride

#include <iostream>
#include "request.hpp"
#include "exceptions.hpp"

namespace tride {
namespace fcgi {

namespace {

// Designed to be only one static member
class LibraryInitializer {
	// Non copy
	LibraryInitializer(const LibraryInitializer&);
	LibraryInitializer& operator=(const LibraryInitializer&);
public:
	LibraryInitializer() {
		auto r = FCGX_Init();
		if( r != 0 ) {
			throw Exception("Failed to initialize fcgi");
		}
	}
};

}

Request::Request():inited(false) {}

void Request::free() {
	if(inited) {
		FCGX_Finish_r(&request);
		inited = false;
	}
}

Request::~Request() {
	free();
}

bool Request::accept() {
	// One time library init on first request
	static LibraryInitializer initializer;
	free();
	auto initResult = FCGX_InitRequest(&request, 0, FCGI_FAIL_ACCEPT_ON_INTR);
	if( initResult != 0 ) {
		throw Exception("Request failed to initialize");
	}
	if(FCGX_Accept_r(&request) != 0) return false;
	inited = true;
	return true;
}

Request& Request::operator<<(const char* const out) {
	FCGX_PutS(out, request.out);
	return *this;
}

void Request::installSignal(int signo, SingalFunction* func) {
	struct sigaction act, old_act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = func;
	sigaction(signo, &act, &old_act);
}

} // namespace fcgi
} // namespace tride

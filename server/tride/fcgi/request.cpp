#include "request.hpp"
#include "exceptions.hpp"

namespace tride {
namespace fcgi {

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
	free();
	auto initResult = FCGX_InitRequest(&request, 0, 0);
	if( initResult != 0 ) {
		throw Exception("Request failed to initialize");
	}
	auto acceptResult = FCGX_Accept_r(&request);
	if( acceptResult != 0 ) {
		throw Exception("Failed to accept request");
	}
	inited = true;
	return true;
}

void Request::Init() {
	static bool initialized = false;
	if( initialized ) {
		throw std::logic_error("fcgi already initialized");
	}
	auto r = FCGX_Init();
	if( r != 0 ) {
		throw Exception("Failed to initialize fcgi");
	}
}

Request& Request::operator<<(const char* const out) {
	FCGX_PutS(out, request.out);
	return *this;
}

} // namespace fcgi
} // namespace tride

#include <map>
#include <iostream>

#include "request_manager.hpp"
#include "fcgi_server.hpp"
#include "handler.hpp"
#include "error_handler.hpp"

namespace {
	const char * const lineEnd = "\r\n";
} // namespace

namespace tride {

bool RequestManager::response() {
	std::cout << "RequestManager::response()" << std::endl;
	switch (state) {
	case START: {
		state = PROCESS;
		Fastcgipp::Message msg;
		msg.type = 1;
		FcgiServer::currentIo().post(boost::bind(callback(), msg));
		return false;
	}
	case PROCESS: {
		std::cout << "Working with request " << environment().scriptName << std::endl;

		const std::string& scriptName = environment().scriptName;
		boost::scoped_ptr<Handler> handler( createHandler(scriptName) );
		if( !handler ) {
			ErrorHandler eh("No handler for \"" + scriptName + "\"");
			eh.handle(*this);
		} else {
			try {
				handler->handle( *this );
			} catch (const std::runtime_error& err) {
				ErrorHandler eh(err.what());
				eh.handle(*this);
			}
		}
		return true;
	}
	}
	return true;
}

RequestManager::RequestManager(): state(START) {}

RequestManager::~RequestManager() {}

typedef std::map<std::string, HandlerFactory> FactoryMap;

static FactoryMap factoryMap;

void RequestManager::registerHandler(const std::string& path, HandlerFactory f) {
	auto range = factoryMap.equal_range(path);
	if(!boost::empty(range)) {
		throw std::logic_error("Trying to register already registred path " + path);
	}
	factoryMap.insert( range.first, std::make_pair("/" + path, f) );
}

HandlerPtr RequestManager::createHandler(const std::string& path) {
	auto range = factoryMap.equal_range(path);
	if(boost::empty(range)) {
		return NULL;
	}
	return range.first->second();
}

void RequestManager::errorHandler(const std::exception& error) {
	std::cout << "Internal error " << error.what() << std::endl;
}

} // namespace tride

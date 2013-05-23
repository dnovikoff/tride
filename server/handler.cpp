#include "handler.hpp"
#include "request_manager.hpp"

namespace tride {

static const char * const nextLine = "\r\n";

Handler::Handler() {}

Handler::~Handler() {}

void Handler::addHeader(const std::string& key, const std::string& value) {
	header_ << key << ": " << value << nextLine;
}

void Handler::handle(RequestManager& rm) {
	std::cout << "Handle" << std::endl;
	doHandle();
	rm.out << header_.str() << nextLine << out_.str();
	std::cout << "Post size = " << rm.environment().posts.size() << std::endl;
}

void Handler::beforeHandle() {

}

} // namespace tride

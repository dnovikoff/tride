#include "handler.hpp"
#include "request_manager.hpp"

namespace tride {

static const char * const nextLine = "\r\n";

Handler::Handler():data(NULL) {}

Handler::~Handler() {}

void Handler::addHeader(const std::string& key, const std::string& value) {
	header_ << key << ": " << value << nextLine;
}

const char * Handler::getData() {
	return data;
}

void Handler::handle(RequestManager& rm) {
	const auto& mp = rm.environment().posts;
	auto iter = mp.find("data");
	if( iter != mp.end() ) {
		data = iter->second.value.c_str();
	}
	std::cout << "Handle" << std::endl;
	beforeHandle();
	doHandle();
	rm.out << header_.str() << nextLine << out_.str();
	std::cout << "Post size = " << rm.environment().posts.size() << std::endl;
}

void Handler::beforeHandle() {

}

} // namespace tride

#include <cstring>
#include <stdexcept>
#include <boost/make_shared.hpp>

#include "logger.hpp"
#include "message.hpp"

namespace tride {
namespace log {

Message::Message(Logger& l, LogLevel level):logger(l), logLevel(level) {
	message = boost::make_shared<std::ostringstream>();
}

void Message::write(const char* data, const size_t size) {
	message->write(data, size);
}

Message::~Message() {
	// Exception thrown, while constructing log message
	if(std::uncaught_exception()) return;
	if( !message.unique() ) return;

	// Message construction is over. We can now pass the message to logger
	logger.write(logLevel, message->str());
}

}  // namespace log
}  // namespace tride

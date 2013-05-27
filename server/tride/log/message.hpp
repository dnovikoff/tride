#ifndef TRIDE_LOG_MESSAGE_HPP_
#define TRIDE_LOG_MESSAGE_HPP_

#include <sstream>
#include <boost/shared_ptr.hpp>

#include <tride/log/log_level.hpp>

namespace tride {
namespace log {

class Logger;

class Message {
	Logger& logger;
	LogLevel logLevel;
	boost::shared_ptr<std::ostringstream> message;

	void write(const char* data, const size_t size);

	friend class Logger;
	Message(Logger& l, LogLevel level);
public:
	template<typename T>
	Message operator<<(const T& data) {
		// Optimization. If the level check fails - nothing will be done.
		if(message) {
			(*message) << data;
		}
		// Return by value!
		return *this;
	}
	~Message();
};

}  // namespace log
}  // namespace tride

#endif /* TRIDE_LOG_MESSAGE_HPP_ */

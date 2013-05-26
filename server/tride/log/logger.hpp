#ifndef TRIDE_LOG_LOGGER_HPP_
#define TRIDE_LOG_LOGGER_HPP_

#include <tride/log/message.hpp>
#include <tride/log/log_level.hpp>

namespace tride {
namespace log {

class Logger {
	Message message(const LogLevel level);
public:
	virtual ~Logger() = 0;
	virtual void write(const LogLevel level, const std::string& message) = 0;

	Message note()  { return message( NOTE  ); }
	Message error() { return message( ERROR ); }

	template<typename T>
	Message operator<<(const T& x) { return note() << x; }
};

}  // namespace log
}  // namespace tride

#endif /* TRIDE_LOG_LOGGER_HPP_ */

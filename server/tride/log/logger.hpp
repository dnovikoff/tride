#ifndef TRIDE_LOG_LOGGER_HPP_
#define TRIDE_LOG_LOGGER_HPP_

#include <tride/log/message.hpp>
#include <tride/log/log_level.hpp>

namespace tride {
namespace log {

/**
 * Abstract class for creating specific loggers
 * Does know nothing, about where and how the messages will be written
 * Provides threshold abilities to filter messages and shugar to easily log them
 */
class Logger {
	const LogLevel threshold;
	Message message(const LogLevel level);

	/**
	 * The only function, that should be overriden
	 */
	virtual void doWrite(const LogLevel level, const std::string& message) = 0;

	// non copyable
	Logger(const Logger&);
	Logger& operator=(const Logger&);
protected:
	static LogLevel defaultThreshold() { return INFO; }
public:
	/**
	 * @param threshold the last level that will be logged
	 */
	Logger(const LogLevel th = defaultThreshold()):threshold(th) {}

	/**
	 * Some messages could be heavy to construct or
	 * present in some perfomance code
	 * in this case you can use the following construction:
	 * if(logger.checkLevel(LogLevel::DEBUG)) {
	 *   logger.debug() << "Your message";
	 * }
	 *
	 * @param level to check aginst threshold
	 */
	bool checkLevel(const LogLevel level) { return level <= threshold; }

	virtual ~Logger() = 0;
	void write(const LogLevel level, const std::string& message) {
		if( !checkLevel(level) ) return;
		doWrite(level, message);
	}

	/**
	 * Shugar for logging into different levels
	 */
	//@{
	Message fatal()   { return message( FATAL   ); }
	Message error()   { return message( ERROR   ); }
	Message warning() { return message( WARNING ); }
	Message info()    { return message( INFO    ); }
	Message debug()   { return message( DEBUG   ); }
	Message trace()   { return message( TRACE   ); }
	//@}

	/**
	 * That means, that default logging level is info
	 */
	template<typename T>
	Message operator<<(const T& x) { return info() << x; }
};

}  // namespace log
}  // namespace tride

#endif /* TRIDE_LOG_LOGGER_HPP_ */

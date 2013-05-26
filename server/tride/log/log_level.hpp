#ifndef TRIDE_LOG_LOG_LEVEL_HPP_
#define TRIDE_LOG_LOG_LEVEL_HPP_


namespace tride {
namespace log {

enum LogLevel { FATAL=1, ERROR, WARNING, INFO, DEBUG, TRACE };

/**
 * Get human readable name of log level
 */
const char* logLevelName(const LogLevel code);

}  // namespace log
}  // namespace tride

#endif /* TRIDE_LOG_LOG_LEVEL_HPP_ */

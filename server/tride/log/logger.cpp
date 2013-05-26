#include "logger.hpp"

namespace tride {
namespace log {

Logger::~Logger() {}

Message Logger::message(const LogLevel level) {
	return Message(*this, level);
}

}  // namespace log
}  // namespace tride

#ifndef COUT_LOGGER_HPP_
#define COUT_LOGGER_HPP_

#include <tride/log/logger.hpp>

namespace tride {

/**
 * Writes to std::cout
 * Thread safe (mutex locking inside)
 */
class CoutLogger:public log::Logger {
	void doWrite(const log::LogLevel level, const std::string& msg) override;
};

}  // namespace tride

#endif /* COUT_LOGGER_HPP_ */

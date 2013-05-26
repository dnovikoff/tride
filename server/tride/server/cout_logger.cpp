#include <iostream>

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <tride/log/logger.hpp>

#include "cout_logger.hpp"

namespace tride {

namespace {
const std::string& levelStr(const log::LogLevel level) {
	static std::string noteTxt("NTE");
	static std::string errorTxt("ERR");
	return level == log::LogLevel::ERROR? errorTxt : noteTxt;
}
}  // namespace

void CoutLogger::doWrite(const log::LogLevel level, const std::string& msg) {
	static boost::mutex writeMutex;
	boost::mutex::scoped_lock l(writeMutex);
	std::cout << '[' << boost::posix_time::second_clock::local_time() << " " << levelStr(level) << "] " << msg << std::endl;
}

}  // namespace tride

#include <sstream>
#include <stdexcept>
#include "log_level.hpp"

namespace tride {
namespace log {

const char* logLevelName(const LogLevel code) {
	switch(code) {
	case(FATAL):   return "FTL";
	case(ERROR):   return "ERR";
	case(WARNING): return "WRN";
	case(INFO):    return "INF";
	case(DEBUG):   return "DBG";
	case(TRACE):   return "TRC";
	}

	std::ostringstream oss;
	oss << "Unknown loggin level " << code;
	throw std::logic_error(oss.str());
}

}  // namespace log
}  // namespace tride

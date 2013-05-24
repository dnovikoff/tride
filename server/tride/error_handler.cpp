#include <boost/lexical_cast.hpp>
#include "error_handler.hpp"

namespace tride {

ErrorHandler::ErrorHandler(const std::string& m, int c):message(m), statusCode(c) {}

void ErrorHandler::doHandle() {
	addHeader("Content-Type", "text/plain; charset=ISO-8859-1");
	addHeader("Status",  boost::lexical_cast<std::string>(statusCode) + " Internal Server Error");
	out() << "Error " << statusCode << ": " << message;
}

} // namespace tride

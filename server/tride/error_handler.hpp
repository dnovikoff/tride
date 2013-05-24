#ifndef ERROR_HANDLER_HPP_
#define ERROR_HANDLER_HPP_

#include <string>
#include <tride/handler.hpp>

namespace tride {

/**
 * Abstract class for handling errors in a same way
 */
class ErrorHandler: public Handler {
	const std::string message;
	const int statusCode;
public:
	explicit ErrorHandler(const std::string& message, int code = 500);
	void doHandle() override;
};

} // namespace tride

#endif /* ERROR_HANDLER_HPP_ */

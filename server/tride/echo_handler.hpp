#ifndef ECHO_HANDLER_HPP_
#define ECHO_HANDLER_HPP_

#include <tride/json_handler.hpp>

namespace tride {

class EchoHandler: public JsonHandler {
	void doHandle() override;
};

} // namespace tride

#endif /* ECHO_HANDLER_HPP_ */

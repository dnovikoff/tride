#include "echo_handler.hpp"

namespace tride {

void EchoHandler::doHandle() {
	out() << "Hello! World";
}

} // namespace tride

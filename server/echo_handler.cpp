#include "echo_handler.hpp"

namespace tride {

void EchoHandler::doHandle() {
	if(getData()) {
		out() << getData();
	} else {
		out() << "{\"message\": \"no data posted\"}";
	}
}

} // namespace tride

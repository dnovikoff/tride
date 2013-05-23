#include "json_handler.hpp"

namespace tride {

void JsonHandler::beforeHandle() {
	addHeader("Content-Type" , "text/json; charset=utf-8");
}

} // namespace tride

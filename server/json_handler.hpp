#ifndef JSON_HANDLER_HPP_
#define JSON_HANDLER_HPP_

#include "handler.hpp"

namespace tride {

class JsonHandler: public Handler {
	void beforeHandle() override;
};

} // namespace tride

#endif /* JSON_HANDLER_HPP_ */

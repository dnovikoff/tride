#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <sstream>

namespace tride {

class RequestManager;

/**
 * Abstract request handler. Inherit it
 */
class Handler {
	std::ostringstream out_;
	std::ostringstream header_;
	const char * data;
	virtual void doHandle() = 0;
	/**
	 * You can write headers for base classes and so
	 * Empty by default
	 */
	virtual void beforeHandle();
	// non copy
	Handler(const Handler&);
	Handler& operator=(const Handler&);
protected:
	void addHeader(const std::string& key, const std::string& value);
	std::ostream& out() { return out_; }
	const char * getData();
public:
	Handler();
	virtual ~Handler() = 0;
	void handle(RequestManager& requestManager);
};

} // namespace tride;

#endif /* HANDLER_HPP_ */

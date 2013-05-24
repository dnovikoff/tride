#ifndef SIGNALS_HPP_
#define SIGNALS_HPP_

namespace tride {

class Signals {
	// No objects
	Signals();
	~Signals();
public:
	/**
	 * Captures signal to stop the system correctly
	 * The handler is reset before any action, so the second signal will kill the system defenetly
	 */
	static void init();
};

} // namespace tride

#endif /* SIGNALS_HPP_ */

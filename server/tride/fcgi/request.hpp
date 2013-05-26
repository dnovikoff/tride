#ifndef TRIDE_FCGI_REQUEST_HPP_
#define TRIDE_FCGI_REQUEST_HPP_

#include <string>
#include <fcgiapp.h>

namespace tride {
namespace fcgi {

class Data;

class Request {
	typedef Request self_t;
	// Noncopy
	Request(const Request&);
	Request& operator=(const Request&);

	FCGX_Request request;
	bool inited;
	void free();
public:
	enum ContentType { POST, GET };

	Request();
	~Request();

	self_t& operator<<(const char* const out);

	self_t& operator<<(const std::string& str) { return operator <<(str.c_str()); }

	/**
	 * Returns content, posted with request (after header ends)
	 * If no countent - the null Data returned
	 */
	Data getContent();

	/**
	 * Returns requested path to the script. everything after # or ? is ommited
	 * The path starts with /. No local address
	 */
	Data getPath();

	/**
	 * Returns content type. Witch could be POST/GET
	 */
	ContentType getContentType();

	/**
	 * Returns true if request accepted.
	 * Returns false if error occures (i.e. server stopped)
	 * Can throw an exeption if:
	 * 1) fcgi library fails to initialize
	 * 2) or Reqest failed to initialize
	 * Note that using accept from different threads could be not thread safe (even for different objects)
	 */
	bool accept();

	typedef void SingalFunction(int);

	/**
	 * Correct installer of signal handlers (to support accept interruption on signal)
	 * example: installSignal(SIGINT, signalHandler);
	 */
	static void installSignal(int signo, SingalFunction* func);
};

} // namespace fcgi
} // namespace tride

#endif /* TRIDE_FCGI_REQUEST_HPP_ */

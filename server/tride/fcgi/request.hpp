#ifndef TRIDE_FCGI_REQUEST_HPP_
#define TRIDE_FCGI_REQUEST_HPP_

#include <string>
#include <fcgiapp.h>

namespace tride {
namespace fcgi {
class Data;
class Acceptor;

class Request {
	typedef Request self_t;
	// Noncopy
	Request(const Request&);
	Request& operator=(const Request&);

	FCGX_Request request;
	friend class Acceptor;
public:
	enum ContentType { POST, GET };

	Request();
	~Request();

	self_t& operator<<(const char* const out);

	inline self_t& operator<<(const std::string& str) { operator <<(str.c_str()); }

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
};

}  // namespace fcgi
}  // namespace tride


#endif /* TRIDE_FCGI_REQUEST_HPP_ */

#ifndef TRIDE_FCGI_EXCEPTIONS_HPP_
#define TRIDE_FCGI_EXCEPTIONS_HPP_

#include <stdexcept>

namespace tride {
namespace fcgi  {

class Exception: public std::runtime_error {
	typedef std::runtime_error parent_t;
public:
	Exception(const std::string& x):parent_t(x) {}
};

} // namespace fcgi
} // namespace tride


#endif /* TRIDE_FCGI_EXCEPTIONS_HPP_ */

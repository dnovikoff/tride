#include <cstring>
#include "data.hpp"

namespace tride {
namespace fcgi {

Data::Data(const char * const d, const size_t l):data(d), len(l) {}

Data::Data(const char * const d):data(d), len(d?strlen(d):0u) {}

bool Data::operator==(const Data& rhs) const {
	if(rhs.len != len) return false;
	if(rhs.data == data) return true;
	if(!rhs.data || !data) return false;
	return memcmp(rhs.data, data, len) == 0;
}

}  // namespace fcgi
}  // namespace tride

std::ostream& operator<<(std::ostream& out, const tride::fcgi::Data& data) {
	if(data.isNull()) {
		return out << "<NULL>";
	}
	return out.write(data.getData(), data.getLength());
}

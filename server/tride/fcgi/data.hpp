#ifndef TRIDE_FCGI_DATA_HPP_
#define TRIDE_FCGI_DATA_HPP_

#include <string>
#include <ostream>

namespace tride {
namespace fcgi {

/**
 * Points to POST data. Does not own that data
 * Data is valid, until Reqest finishes
 * Copies are ok. This is a small object
 */
class Data {
	const char * data;
	size_t len;
public:
	explicit Data(const char * const d);
	Data(const char * const d, const size_t l);
	bool isNull() const { return data == NULL; }
	const char * getData() const { return data; }
	size_t getLength() const { return len; }
	bool operator==(const Data& rhs) const;
	inline bool operator==(const char* rdata) const { return operator==(Data(rdata)); }
	inline bool operator==(const std::string& x) const { return operator==(x.c_str()); }

	template<typename T>
	inline bool operator!=(const T& rhs) const { return !operator==(rhs); }
	std::string toString() const { return std::string(data, len); }
	static const Data& nullData() {
		static Data nullData_(NULL, 0);
		return nullData_;
	}
};

}  // namespace fcgi
}  // namespace tride

std::ostream& operator<<(std::ostream& out, const tride::fcgi::Data& data);

#endif /* TRIDE_FCGI_DATA_HPP_ */

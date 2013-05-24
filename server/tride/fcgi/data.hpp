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
	/**
	 * @param data Zero-terminated string or NULL
	 */
	explicit Data(const char * const data);
	Data(const char * const d, const size_t l);

	bool isNull() const { return data == NULL; }
	const char * getData() const { return data; }
	size_t getLength() const { return len; }

	bool operator==(const Data& rhs) const;
	bool operator==(const char* rdata) const { return operator==(Data(rdata)); }
	bool operator==(const std::string& x) const { return operator==(Data(x.c_str(), x.length())); }

	template<typename T>
	bool operator!=(const T& rhs) const { return !operator==(rhs); }

	/**
	 * Creates a string object, based on data and its length
	 * Do not call on null data (assert inside)
	 */
	std::string toString() const;

	static const Data& nullData();
};

}  // namespace fcgi
}  // namespace tride

std::ostream& operator<<(std::ostream& out, const tride::fcgi::Data& data);

#endif /* TRIDE_FCGI_DATA_HPP_ */

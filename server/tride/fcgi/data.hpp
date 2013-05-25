#ifndef TRIDE_FCGI_DATA_HPP_
#define TRIDE_FCGI_DATA_HPP_

#include <string>
#include <ostream>

namespace tride {
namespace fcgi {

/**
 * Points to any data. I.e. content, or some parameter
 * Not using extra memory - points to existing one.
 * Not owning the memory, not making a copy. Have no options to modify data
 * Will be valid, till data exists (i.e. while request not finished)
 * Feel free to use operator= or copy-constructor
 * Don't be aware of passing by value - the object is small
 */
class Data {
	const char * data;
	size_t len;
public:
	/**
	 * Construcsts data from a zero-terimnated string
	 * Uses \0 to determinate size of data segment
	 * If NULL passed, then it will be treated like null data
	 * @param data Zero-terminated string or NULL
	 */
	explicit Data(const char * const data);

	/**
	 * Constructed based on pointer to existing data segment and it's size
	 */
	Data(const char * const d, const size_t l);

	/**
	 * Checks whether the data is null (null is not empty)
	 */
	bool isNull() const { return data == NULL; }

	/**
	 * Returns pointer to data segment
	 */
	const char * getData() const { return data; }

	/**
	 * Returns data segment length
	 */
	size_t getLength() const { return len; }

	/**
	 * Comparation operators
	 */
	//@{
	bool operator==(const Data& rhs) const;
	bool operator==(const char* rdata) const { return operator==(Data(rdata)); }
	bool operator==(const std::string& x) const { return operator==(Data(x.c_str(), x.length())); }

	template<typename T>
	bool operator!=(const T& rhs) const { return !operator==(rhs); }
	//@}

	/**
	 * Creates a string object, based on data and its length
	 * Do not call on null data (assert inside)
	 */
	std::string toString() const;

	/**
	 * returns refernce to static null data. Could be usefull for syntax shugar
	 */
	static const Data& nullData();
};

}  // namespace fcgi
}  // namespace tride

std::ostream& operator<<(std::ostream& out, const tride::fcgi::Data& data);

#endif /* TRIDE_FCGI_DATA_HPP_ */

#if !defined(__RMU_ERROR_HPP__) && defined(__cplusplus)
#define __RMU_ERROR_HPP__
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

class RMUError : public std::runtime_error {
    typedef std::runtime_error _mybase;
#if defined(WIN32)
    static const char PATH_SEPARATOR = '\\';
#else
    static const char PATH_SEPARATOR = '/';
#endif
public:
    explicit RMUError(const std::string& message) : _mybase(message) {}
    explicit RMUError(const char* message) : _mybase(message) {}
    static void Raise(const std::string& message, const char* fileName, int lineNo) {
        if (!fileName)
            fileName = "";
        const char* pFileName = strrchr(fileName, PATH_SEPARATOR);
        std::stringstream s;
        s << "RMUError in file " << (pFileName == NULL ? fileName : pFileName + 1) << ", line " << lineNo << ": " << message;
        throw RMUError(s.str());
    }
    static void Raise(const char* message, const char* fileName, int lineNo) {
        Raise(std::string(message), fileName, lineNo);
    }
};

#define RAISE_RMU_ERR(msg) (RMUError::Raise((msg), __FILE__, __LINE__))

#endif  //__RMU_ERROR_HPP__

#ifndef MUSTARD_MUSTARDEXCEPTION_H
#define MUSTARD_MUSTARDEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class MustardException : public exception
{
public:
    MustardException ( const char *what ) : reason ( what ) {}
    const char *what() const noexcept {
        return reason.c_str();
    }
    ~MustardException() noexcept {}
private:
    const string reason;

};

#endif //MUSTARD_MUSTARDEXCEPTION_H

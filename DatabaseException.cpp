#include "DatabaseException.h"


DatabaseException::DatabaseException(char const* const message) throw(): std::runtime_error(message)
{

}

char const* DatabaseException::what() const throw()
{
    return exception::what();
}
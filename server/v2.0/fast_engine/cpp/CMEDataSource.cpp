

#include <Application/QuickFAST.h>

#include "CMEDataSource.hpp"

fast_engine::CMEDataSource::CMEDataSource()
{
    first = false;
}

fast_engine::CMEDataSource::~CMEDataSource() { }

void fast_engine::CMEDataSource::setBuffer( unsigned char * msg_buf, const int msg_len )
{
    first = true;
    reset();
    end = msg_len;
    buf = msg_buf;
}


bool fast_engine::CMEDataSource::getBuffer( const unsigned char *& msg_buf, size_t & msg_len )
{
    if(! first )
    {
        return false;
    }

    first = false;
    msg_buf = buf;
    msg_len = end;
    return true;
}
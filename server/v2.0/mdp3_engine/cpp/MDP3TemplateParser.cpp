#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <xmlParser.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

void initLogging( const XMLNode& config )
{
    const XMLNode node = config.getChildNode( "logging" );
    log4cpp::PropertyConfigurator::configure( node.getAttribute( "properties" ) );
}

int main( int argc, char *argv[] )
{
    typedef boost::iostreams::basic_array_source<char> Device;
    const XMLNode& root = XMLNode::openFileHelper( argv[ 1 ] );
    const XMLNode& types_node = root.getChildNode( "types" );
    
    for( int i=0; i<types_node.nChildNode( "type" ); ++i )
    {
        const XMLNode& type_node = types_node.getChildNode( "type", i );
    }

    return 0;
}

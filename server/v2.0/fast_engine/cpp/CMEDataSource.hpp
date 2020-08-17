#ifndef QUICKFAST_CME_DATASOURCE_H
#define QUICKFAST_CME_DATASOURCE_H

#include <Codecs/DataSource.h>

namespace fast_engine
{
    class CMEDataSource : public QuickFAST::Codecs::DataSource
    {
        public:

            CMEDataSource();
            ~CMEDataSource();

            void setBuffer( unsigned char * , const int );
            virtual bool getBuffer( const unsigned char *& , size_t & );

        private:

            CMEDataSource (const CMEDataSource &);
            CMEDataSource & operator= (const CMEDataSource &);

            unsigned char * buf;
            size_t end;
            bool first;
    };
}

#endif // QUICKFAST_CME_DATASOURCE_H

#ifndef QUANTMODEL_SYSTEMPARAMETER_H
#define QUANTMODEL_SYSTEMPARAMETER_H

#include <quantmodel.hpp>

namespace quantmodel
{
    class SystemParameter : public DataObject
    {
        public:

            SystemParameter( const quantmodel::DatabaseEngineResponse::ResultSet::Row& row )
            {
                message = &data;
                reflector = data.GetReflection();
                descriptor = data.GetDescriptor();

                for( int k=0; k<row.column_size(); ++k )
                {
                    setAttribute( row.column( k ).name(), row.column( k ).value() );
                }
            }

            ~SystemParameter(){};

            inline const std::string& getParameterId() { return data.parameter_id(); }
            inline const std::string& getCategory() { return data.category(); }
            inline const std::string& getName() { return data.name(); }
            inline const std::string& getValue() { return data.value(); }

        private:

            // prevent copying and assignment; not implemented
            SystemParameter (const SystemParameter &);
            SystemParameter & operator= (const SystemParameter &);

            quantmodel::SystemParameterData data;
    };
}

#endif // QUANTMODEL_SYSTEMPARAMETER_H

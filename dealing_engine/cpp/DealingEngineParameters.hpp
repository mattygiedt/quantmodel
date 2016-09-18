#ifndef DEALING_DEALINGENGINEPARAMETERS_H
#define DEALING_DEALINGENGINEPARAMETERS_H

namespace dealing
{
    class DealingEngineParameters
    {
        public:

            enum ObjectType {
                ORDER,
                ALLOCATION,
                RELEASE,
                EXECUTION
            };

            DealingEngineParameters(){};
            ~DealingEngineParameters(){};

            inline const std::string& getParameter( const char* name ) { return parameter_map[ name ]->getValue(); }
            inline const std::string& getParameter( const std::string& name ) { return parameter_map[ name ]->getValue(); }
            inline const void setParameter( const std::string& name, const std::string& value ) { parameter_map[ name ]->setAttribute( quantmodel::VALUE, value ); }
            inline const bool sendToFix() { return parameter_map[ "send_to_fix" ]->getAttribute( quantmodel::VALUE ).compare( "1" ) == 0; }

            void getNextObjectId( std::string& , std::string& , ObjectType );

            void initLogging();
            void loadParameters( zmq::socket_t* );
            void updateParameter( const quantmodel::SystemParameterData& , quantmodel::DatabaseEngineRequest& );
            void getParameters( quantmodel::DealingEngineResponse& );
            void getParameters( quantmodel::DealingEngineBroadcast& );

            const void close()
            {
                std::map< const std::string , quantmodel::SystemParameter* >::iterator iter;
                iter = parameter_map.begin();
                while( iter != parameter_map.end() )
                {
                    delete iter->second;
                    ++iter;
                }

                parameter_map.clear();
            }

        private:
            log4cpp::Category* log;
            std::map< const std::string , quantmodel::SystemParameter* > parameter_map;
    };
}

#endif // DEALING_DEALINGENGINEPARAMETERS_H

use `quantmodel`;

--  system_parameter
insert into system_parameter( category, name, value ) values ( 'DEALING_ENGINE', 'curr_order_id', '0' );
insert into system_parameter( category, name, value ) values ( 'DEALING_ENGINE', 'curr_allocation_id', '0' );
insert into system_parameter( category, name, value ) values ( 'DEALING_ENGINE', 'curr_release_id', '0' );
insert into system_parameter( category, name, value ) values ( 'DEALING_ENGINE', 'curr_execution_id', '0' );
insert into system_parameter( category, name, value ) values ( 'DEALING_ENGINE', 'send_to_fix', '1' );

-- account
insert into account( name, market_account_id, custody_account_id, notional_val, profit_target, stop_loss, signal_weight, is_suppressed, is_active, added_datetime, added_by, updated_datetime, updated_by ) values ( 'BDS_QUANT I',  '737A1122', 'A1122', 1000000, .015, .15, 1, 0, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );
insert into account( name, market_account_id, custody_account_id, notional_val, profit_target, stop_loss, signal_weight, is_suppressed, is_active, added_datetime, added_by, updated_datetime, updated_by ) values ( 'BDS_QUANT II', '737A1123', 'A1123', 1000000, .015, .15, 1, 0, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

-- account_limit
insert into account_limit select distinct account_id, symbol, security_exchange, 'FIX.4.2:BDS->TTDEV3O', 10, 10, 10, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' from account, instrument;

-- investment_system
insert into investment_system ( investment_system_id, name, is_active ) values ( 'WRIGHTWOOD', 'WRIGHTWOOD', 1 );
insert into investment_system ( investment_system_id, name, is_active ) values ( 'SUPERIOR', 'SUPERIOR', 1 );

-- strategy
insert into strategy ( investment_system_id, signal_id, name, default_weight, is_active ) values ( 'WRIGHTWOOD', 'ES', 'ES', 1, 1 );
insert into strategy ( investment_system_id, signal_id, name, default_weight, is_active ) values ( 'WRIGHTWOOD', 'NQ', 'NQ', 1, 1 );
insert into strategy ( investment_system_id, signal_id, name, default_weight, is_active ) values ( 'WRIGHTWOOD', 'CL', 'CL', 1, 1 );
insert into strategy ( investment_system_id, signal_id, name, default_weight, is_active ) values ( 'WRIGHTWOOD', 'R2K', 'R2K', 1, 1 );

insert into strategy ( investment_system_id, signal_id, name, default_weight, is_active ) values ( 'SUPERIOR', 'CL_5', 'CL_5', 1, 1 );

-- account_strategy
insert into account_strategy select account_id, strategy_id, 1, 1 from account, strategy;

--  instrument_id_type
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'NAME', 'name' );
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'SECID', 'secid' );
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'SYMBOL', 'symbol' );
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'CUSIP', 'cusip' );
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'SEDOL', 'sedol' );
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'ISIN', 'isin' );
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'RIC', 'ric' );
insert into instrument_id_type( instrument_id_type_id, name ) values ( 'INSTR', 'instrument_id' );

--  instrument_type
insert into instrument_type( name ) values ( 'EQUITY' );
insert into instrument_type( name ) values ( 'FUTURE' );
insert into instrument_type( name ) values ( 'FOREX' );
insert into instrument_type( name ) values ( 'CREDIT' );
insert into instrument_type( name ) values ( 'DERIVATIVE' );

--  user
insert into user( username, password, firstname, lastname, is_active ) values ( 'bdsq', 'bdsq', 'sysadmin', '', 1 );
insert into user( username, password, firstname, lastname, is_active ) values ( 'trader', 'trader', 'Mark', 'De Souza', 1 );
insert into user( username, password, firstname, lastname, is_active ) values ( 'dorman', 'dorman', 'Dorman', '', 1 );
insert into user( username, password, firstname, lastname, is_active ) values ( 'advantage', 'advantage', 'Advantage', '', 1 );

insert into application( application_id, application_name, is_active ) values ( 'WEB', 'WEB', 1 );
insert into application( application_id, application_name, is_active ) values ( 'CLIENT', 'CLIENT', 1 );
insert into application( application_id, application_name, is_active ) values ( 'VIEWER', 'VIEWER', 1 );
insert into application( application_id, application_name, is_active ) values ( 'COMPLNCE', 'COMPLIANCE', 1 );

insert into user_application select user_id, application_id from user, application;
insert into user_account select user_id, account_id from user, account;
insert into user_investment_system select user_id, investment_system_id from user, investment_system;

use `quantmodel`;

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

-- CHANNEL 961
insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999462, 2, 'CME_CERT 999462', '95', '999462', 'FUT', '2BBM1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20100401000000000000', 'INIT', '20100401000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999472, 2, 'CME_CERT 999472', '95', '999472', 'FUT', '2BBU1', 'XNYM', '201109', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999482, 2, 'CME_CERT 999482', '95', '999482', 'FUT', '2BBM1-2BBU1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999492, 2, 'CME_CERT 999492', '95', '999492', 'FUT', '2CLM1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999502, 2, 'CME_CERT 999502', '95', '999502', 'FUT', '2CLU1', 'XNYM', '201109', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999512, 2, 'CME_CERT 999512', '95', '999512', 'FUT', '2CLM1-2CLU1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

-- CHANNEL 941
insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999460, 2, 'CME_CERT 999460', '95', '999460', 'FUT', '0BBM1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20100401000000000000', 'INIT', '20100401000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999470, 2, 'CME_CERT 999470', '95', '999470', 'FUT', '0BBU1', 'XNYM', '201109', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999480, 2, 'CME_CERT 999480', '95', '999480', 'FUT', '0BBM1-0BBU1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999490, 2, 'CME_CERT 999490', '95', '999490', 'FUT', '0CLM1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999500, 2, 'CME_CERT 999500', '95', '999500', 'FUT', '0CLU1', 'XNYM', '201109', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

insert into instrument( instrument_id, instrument_type, name, symbol, security_id, security_type, security_desc, security_exchange, maturity_month_year, base_ccy, last_prc, mark_prc, multiplier, exchange_price_factor, is_active, added_datetime, added_by, updated_datetime, updated_by ) values
( 999510, 2, 'CME_CERT 999510', '95', '999510', 'FUT', '0CLM1-0CLU1', 'XNYM', '201106', 'USD', 0.0, 0.0, 50, 1, 1, '20110101000000000000', 'INIT', '20110101000000000000', 'INIT' );

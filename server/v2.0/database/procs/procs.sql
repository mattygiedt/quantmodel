 -- ---------------------
 --
 -- QuantModel Stored Procedures: Mon May 21 09:57:28 CDT 2012
 --
 -- ---------------------

use `quantmodel`;

 -- ---------------------
 --
 -- GET_DEALING_EVENTS_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_DEALING_EVENTS_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_DEALING_EVENTS_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    dealing_audit
                WHERE   order_id = p_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_FIX_EVENTS_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_FIX_EVENTS_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_FIX_EVENTS_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  f.*
                FROM    fix_audit f, client_order_id_map m
                WHERE   m.order_id = p_order_id
                    AND f.clordid = m.client_order_id
                ORDER BY f.added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ORDERS_FROM_ADDED_DATETIME
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ORDERS_FROM_ADDED_DATETIME;

            DELIMITER |

            CREATE PROCEDURE GET_ORDERS_FROM_ADDED_DATETIME(
                IN p_added_datetime CHAR(20)
            )
            BEGIN

                SELECT  order_id
                FROM    orders
                WHERE   added_datetime >= p_added_datetime
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_AUDIT_EVENT(
                IN p_order_id VARCHAR(16),
                IN p_element_id VARCHAR(16),
                IN p_element_type CHAR(1),
                IN p_audit_desc VARCHAR(256),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO audit_event(
                    order_id,
                    element_id,
                    element_type,
                    audit_desc,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_order_id,
                    p_element_id,
                    p_element_type,
                    p_audit_desc,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_COMPLIANCE_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_COMPLIANCE_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_COMPLIANCE_AUDIT_EVENT(
                IN p_compliance_audit_id INT,
                IN p_session_id VARCHAR(64),
                IN p_account_id INT,
                IN p_instrument_id INT,
                IN p_order_id VARCHAR(16),
                IN p_allocation_id VARCHAR(16),
                IN p_release_id VARCHAR(16),
                IN p_side_code TINYINT,
                IN p_release_qty VARCHAR(30),
                IN p_worst_case_long_qty INT,
                IN p_worst_case_short_qty INT,
                IN p_curr_open_long_qty INT,
                IN p_curr_open_short_qty INT,
                IN p_max_open_long_qty INT,
                IN p_max_open_short_qty INT,
                IN p_max_release_qty INT,
                IN p_result_code VARCHAR(8),
                IN p_added_datetime CHAR(17),
                IN p_added_by VARCHAR(20)
            )
            BEGIN

                INSERT INTO compliance_audit(
                    compliance_audit_id,
                    session_id,
                    account_id,
                    instrument_id,
                    order_id,
                    allocation_id,
                    release_id,
                    side_code,
                    release_qty,
                    worst_case_long_qty,
                    worst_case_short_qty,
                    curr_open_long_qty,
                    curr_open_short_qty,
                    max_open_long_qty,
                    max_open_short_qty,
                    max_release_qty,
                    result_code,
                    added_datetime,
                    added_by )
                VALUES (
                    p_compliance_audit_id,
                    p_session_id,
                    p_account_id,
                    p_instrument_id,
                    p_order_id,
                    p_allocation_id,
                    p_release_id,
                    p_side_code,
                    p_release_qty,
                    p_worst_case_long_qty,
                    p_worst_case_short_qty,
                    p_curr_open_long_qty,
                    p_curr_open_short_qty,
                    p_max_open_long_qty,
                    p_max_open_short_qty,
                    p_max_release_qty,
                    p_result_code,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_DEALING_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_DEALING_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_DEALING_AUDIT_EVENT(
                IN p_request_id VARCHAR(64),
                IN p_request_type CHAR(32),
                IN p_request_ts CHAR(17),
                IN p_response_id CHAR(16),
                IN p_response_type CHAR(16),
                IN p_response_ts CHAR(17),
                IN p_error_msg VARCHAR(255),
                IN p_added_datetime CHAR(17),
                IN p_added_by VARCHAR(20)
            )
            BEGIN

                INSERT INTO dealing_audit(
                    request_id,
                    request_type,
                    request_ts,
                    response_id,
                    response_type,
                    response_ts,
                    error_msg,
                    added_datetime,
                    added_by )
                VALUES (
                    p_request_id,
                    p_request_type,
                    p_request_ts,
                    p_response_id,
                    p_response_type,
                    p_response_ts,
                    p_error_msg,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_DEALING_EXECUTION_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_DEALING_EXECUTION_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_DEALING_EXECUTION_AUDIT_EVENT(
                IN p_request_id VARCHAR(64),
                IN p_request_type CHAR(32),
                IN p_request_ts CHAR(17),
                IN p_response_id CHAR(16),
                IN p_response_type CHAR(16),
                IN p_response_ts CHAR(17),
                IN p_error_msg VARCHAR(255),
                IN p_investment_system_id VARCHAR(16),
                IN p_order_id VARCHAR(16),
                IN p_release_id VARCHAR(16),
                IN p_execution_id VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_account_id VARCHAR(16),
                IN p_instrument_id VARCHAR(16),
                IN p_side_code CHAR(1),
                IN p_execution_status CHAR(1),
                IN p_execution_qty VARCHAR(30),
                IN p_execution_prc VARCHAR(30),
                IN p_added_datetime CHAR(17),
                IN p_added_by VARCHAR(20)
            )
            BEGIN

                INSERT INTO dealing_audit(
                    request_id,
                    request_type,
                    request_ts,
                    response_id,
                    response_type,
                    response_ts,
                    error_msg,
                    investment_system_id,
                    order_id,
                    release_id,
                    execution_id,
                    session_id,
                    account_id,
                    instrument_id,
                    side_code,
                    execution_status,
                    execution_qty,
                    execution_prc,
                    added_datetime,
                    added_by )
                VALUES (
                    p_request_id,
                    p_request_type,
                    p_request_ts,
                    p_response_id,
                    p_response_type,
                    p_response_ts,
                    p_error_msg,
                    p_investment_system_id,
                    p_order_id,
                    p_release_id,
                    p_execution_id,
                    p_session_id,
                    p_account_id,
                    p_instrument_id,
                    p_side_code,
                    p_execution_status,
                    p_execution_qty,
                    p_execution_prc,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_DEALING_FIX_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_DEALING_FIX_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_DEALING_FIX_AUDIT_EVENT(
                IN p_request_id VARCHAR(64),
                IN p_request_type CHAR(32),
                IN p_request_ts CHAR(17),
                IN p_response_id CHAR(16),
                IN p_response_type CHAR(16),
                IN p_response_ts CHAR(17),
                IN p_error_msg VARCHAR(255),
                IN p_fix_engine_id VARCHAR(32),
                IN p_session_id VARCHAR(64),
                IN p_added_datetime CHAR(17),
                IN p_added_by VARCHAR(20)
            )
            BEGIN

                INSERT INTO dealing_audit(
                    request_id,
                    request_type,
                    request_ts,
                    response_id,
                    response_type,
                    response_ts,
                    error_msg,
                    fix_engine_id,
                    session_id,
                    added_datetime,
                    added_by )
                VALUES (
                    p_request_id,
                    p_request_type,
                    p_request_ts,
                    p_response_id,
                    p_response_type,
                    p_response_ts,
                    p_error_msg,
                    p_fix_engine_id,
                    p_session_id,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_DEALING_ORDER_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_DEALING_ORDER_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_DEALING_ORDER_AUDIT_EVENT(
                IN p_request_id VARCHAR(64),
                IN p_request_type CHAR(32),
                IN p_request_ts CHAR(17),
                IN p_response_id CHAR(16),
                IN p_response_type CHAR(16),
                IN p_response_ts CHAR(17),
                IN p_error_msg VARCHAR(255),
                IN p_investment_system_id VARCHAR(16),
                IN p_order_id VARCHAR(16),
                IN p_instrument_id VARCHAR(16),
                IN p_side_code CHAR(1),
                IN p_tif CHAR(1),
                IN p_limit_prc VARCHAR(30),
                IN p_stop_prc VARCHAR(30),
                IN p_order_status CHAR(1),
                IN p_order_qty VARCHAR(30),
                IN p_order_released_qty VARCHAR(30),
                IN p_order_executed_qty VARCHAR(30),
                IN p_order_average_prc VARCHAR(30),
                IN p_added_datetime CHAR(17),
                IN p_added_by VARCHAR(20)
            )
            BEGIN

                INSERT INTO dealing_audit(
                    request_id,
                    request_type,
                    request_ts,
                    response_id,
                    response_type,
                    response_ts,
                    error_msg,
                    investment_system_id,
                    order_id,
                    instrument_id,
                    side_code,
                    tif,
                    limit_prc,
                    stop_prc,
                    order_status,
                    order_qty,
                    order_released_qty,
                    order_executed_qty,
                    order_average_prc,
                    added_datetime,
                    added_by )
                VALUES (
                    p_request_id,
                    p_request_type,
                    p_request_ts,
                    p_response_id,
                    p_response_type,
                    p_response_ts,
                    p_error_msg,
                    p_investment_system_id,
                    p_order_id,
                    p_instrument_id,
                    p_side_code,
                    p_tif,
                    p_limit_prc,
                    p_stop_prc,
                    p_order_status,
                    p_order_qty,
                    p_order_released_qty,
                    p_order_executed_qty,
                    p_order_average_prc,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_DEALING_RELEASE_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_DEALING_RELEASE_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_DEALING_RELEASE_AUDIT_EVENT(
                IN p_request_id VARCHAR(64),
                IN p_request_type CHAR(32),
                IN p_request_ts CHAR(17),
                IN p_response_id CHAR(16),
                IN p_response_type CHAR(16),
                IN p_response_ts CHAR(17),
                IN p_error_msg VARCHAR(255),
                IN p_investment_system_id VARCHAR(16),
                IN p_order_id VARCHAR(16),
                IN p_release_id VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_account_id VARCHAR(16),
                IN p_instrument_id VARCHAR(16),
                IN p_side_code CHAR(1),
                IN p_tif CHAR(1),
                IN p_limit_prc VARCHAR(30),
                IN p_stop_prc VARCHAR(30),
                IN p_release_status CHAR(1),
                IN p_release_qty VARCHAR(30),
                IN p_release_executed_qty VARCHAR(30),
                IN p_release_average_prc VARCHAR(30),
                IN p_added_datetime CHAR(17),
                IN p_added_by VARCHAR(20)
            )
            BEGIN

                INSERT INTO dealing_audit(
                    request_id,
                    request_type,
                    request_ts,
                    response_id,
                    response_type,
                    response_ts,
                    error_msg,
                    investment_system_id,
                    order_id,
                    release_id,
                    session_id,
                    account_id,
                    instrument_id,
                    side_code,
                    tif,
                    limit_prc,
                    stop_prc,
                    release_status,
                    release_qty,
                    release_executed_qty,
                    release_average_prc,
                    added_datetime,
                    added_by )
                VALUES (
                    p_request_id,
                    p_request_type,
                    p_request_ts,
                    p_response_id,
                    p_response_type,
                    p_response_ts,
                    p_error_msg,
                    p_investment_system_id,
                    p_order_id,
                    p_release_id,
                    p_session_id,
                    p_account_id,
                    p_instrument_id,
                    p_side_code,
                    p_tif,
                    p_limit_prc,
                    p_stop_prc,
                    p_release_status,
                    p_release_qty,
                    p_release_executed_qty,
                    p_release_average_prc,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_FIX_AUDIT_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_FIX_AUDIT_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_FIX_AUDIT_EVENT(
                IN p_session_id VARCHAR(64) ,
                IN p_message_direction VARCHAR(8) ,
                IN p_account VARCHAR(16) ,
                IN p_avgpx VARCHAR(30) ,
                IN p_clordid VARCHAR(32) ,
                IN p_cumqty VARCHAR(30) ,
                IN p_execid VARCHAR(32) ,
                IN p_execrefid VARCHAR(32) ,
                IN p_exectranstype VARCHAR(2) ,
                IN p_lastpx VARCHAR(30) ,
                IN p_lastshares VARCHAR(30) ,
                IN p_msgtype VARCHAR(2) ,
                IN p_orderid VARCHAR(32) ,
                IN p_orderqty VARCHAR(30) ,
                IN p_ordstatus VARCHAR(2) ,
                IN p_ordtype VARCHAR(2) ,
                IN p_origclordid VARCHAR(32) ,
                IN p_price VARCHAR(30) ,
                IN p_securityid VARCHAR(32) ,
                IN p_sendercompid VARCHAR(32) ,
                IN p_sendersubid VARCHAR(32) ,
                IN p_sendingtime VARCHAR(32) ,
                IN p_shares VARCHAR(30) ,
                IN p_side VARCHAR(2) ,
                IN p_symbol VARCHAR(16) ,
                IN p_targetcompid VARCHAR(32) ,
                IN p_targetsubid VARCHAR(32) ,
                IN p_text VARCHAR(255) ,
                IN p_timeinforce VARCHAR(2) ,
                IN p_transacttime VARCHAR(32) ,
                IN p_stoppx VARCHAR(30) ,
                IN p_senderlocationid VARCHAR(16) ,
                IN p_exectype VARCHAR(2) ,
                IN p_leavesqty VARCHAR(30) ,
                IN p_securitytype VARCHAR(8) ,
                IN p_maturitymonthyear VARCHAR(45) ,
                IN p_customerorfirm VARCHAR(2) ,
                IN p_securityexchange VARCHAR(16) ,
                IN p_maxshow VARCHAR(30) ,
                IN p_manualorderindicator VARCHAR(2) ,
                IN p_cticode VARCHAR(2) ,
                IN p_correlationclordid VARCHAR(32) ,
                IN p_added_datetime CHAR(17) ,
                IN p_added_by VARCHAR(20)
            )
            BEGIN

            INSERT INTO fix_audit(
                session_id,
                message_direction,
                account,
                avgpx,
                clordid,
                cumqty,
                execid,
                execrefid,
                exectranstype,
                lastpx,
                lastshares,
                msgtype,
                orderid,
                orderqty,
                ordstatus,
                ordtype,
                origclordid,
                price,
                securityid,
                sendercompid,
                sendersubid,
                sendingtime,
                shares,
                side,
                symbol,
                targetcompid,
                targetsubid,
                text,
                timeinforce,
                transacttime,
                stoppx,
                senderlocationid,
                exectype,
                leavesqty,
                securitytype,
                maturitymonthyear,
                customerorfirm,
                securityexchange,
                maxshow,
                manualorderindicator,
                cticode,
                correlationclordid,
                added_datetime,
                added_by )
            VALUES (
                p_session_id,
                p_message_direction,
                p_account,
                p_avgpx,
                p_clordid,
                p_cumqty,
                p_execid,
                p_execrefid,
                p_exectranstype,
                p_lastpx,
                p_lastshares,
                p_msgtype,
                p_orderid,
                p_orderqty,
                p_ordstatus,
                p_ordtype,
                p_origclordid,
                p_price,
                p_securityid,
                p_sendercompid,
                p_sendersubid,
                p_sendingtime,
                p_shares,
                p_side,
                p_symbol,
                p_targetcompid,
                p_targetsubid,
                p_text,
                p_timeinforce,
                p_transacttime,
                p_stoppx,
                p_senderlocationid,
                p_exectype,
                p_leavesqty,
                p_securitytype,
                p_maturitymonthyear,
                p_customerorfirm,
                p_securityexchange,
                p_maxshow,
                p_manualorderindicator,
                p_cticode,
                p_correlationclordid,
                p_added_datetime,
                p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_LIMITS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACCOUNT_LIMITS;

            DELIMITER |

            CREATE PROCEDURE GET_ACCOUNT_LIMITS(
            )
            BEGIN

                SELECT  *
                FROM    account_limit;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID;

            DELIMITER |

            CREATE PROCEDURE GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID(
                IN p_account_id INT
            )
            BEGIN

                SELECT  *
                FROM    account_limit
                WHERE   account_id = p_account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID;

            DELIMITER |

            CREATE PROCEDURE GET_ACCOUNT_LIMIT_BY_ACCOUNT_INSTRUMENT_ID(
                IN p_account_id INT,
                IN p_instrument_id INT
            )
            BEGIN

                SELECT  *
                FROM    account_limit
                WHERE   account_id = p_account_id
                AND     instrument_id = p_instrument_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_ACCOUNT_LIMIT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_ACCOUNT_LIMIT;

            DELIMITER |

            CREATE PROCEDURE INSERT_ACCOUNT_LIMIT(
                IN p_account_id INT,
                IN p_symbol VARCHAR(16),
                IN p_security_exchange VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_max_open_long_qty INT,
                IN p_max_open_short_qty INT,
                IN p_max_release_qty INT,
                IN p_is_active TINYINT,
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO account_limit (
                    account_id,
                    symbol,
                    security_exchange,
                    session_id,
                    max_open_long_qty,
                    max_open_short_qty,
                    max_release_qty,
                    is_active,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_account_id,
                    p_symbol,
                    p_security_exchange,
                    p_session_id,
                    p_max_open_long_qty,
                    p_max_open_short_qty,
                    p_max_release_qty,
                    p_is_active,
                    p_added_datetime,
                    p_added_by,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_ACCOUNT_LIMIT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_ACCOUNT_LIMIT;

            DELIMITER |

            CREATE PROCEDURE UPDATE_ACCOUNT_LIMIT(
                IN p_account_id INT,
                IN p_symbol VARCHAR(16),
                IN p_security_exchange VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_max_open_long_qty INT,
                IN p_max_open_short_qty INT,
                IN p_max_release_qty INT,
                IN p_is_active TINYINT,
                IN p_updated_datetime CHAR(17),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE account_limit SET
                    session_id = p_session_id,
                    max_open_long_qty = p_max_open_long_qty,
                    max_open_short_qty = p_max_open_short_qty,
                    max_release_qty = p_max_release_qty,
                    is_active = p_is_active,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE account_id = p_account_id
                  AND symbol = p_symbol
                  AND security_exchange = p_security_exchange;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_ACCOUNT_LIMIT_SESSION_IDS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_ACCOUNT_LIMIT_SESSION_IDS;

            DELIMITER |

            CREATE PROCEDURE UPDATE_ACCOUNT_LIMIT_SESSION_IDS(
                IN p_account_id INT,
                IN p_session_id VARCHAR(64),
                IN p_updated_datetime CHAR(17),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE account_limit SET
                    session_id = p_session_id,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE account_id = p_account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- CLEARDOWN
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS CLEARDOWN;

            DELIMITER |

            CREATE PROCEDURE CLEARDOWN(
            )
            BEGIN

                delete from executions;
                delete from releases;
                delete from allocations;
                delete from orders;
                delete from position_overnight;
                update system_parameter set value = '0' where name = 'curr_lot_id' and category = 'DEALING_ENGINE';
                update system_parameter set value = '0' where name = 'curr_order_id' and category = 'DEALING_ENGINE';
                update system_parameter set value = '0' where name = 'curr_release_id' and category = 'DEALING_ENGINE';
                update system_parameter set value = '0' where name = 'curr_execution_id' and category = 'DEALING_ENGINE';
                update system_parameter set value = '0' where name = 'curr_transaction_id' and category = 'DEALING_ENGINE';

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_POSITION_OVERNIGHT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS DELETE_POSITION_OVERNIGHT;

            DELIMITER |

            CREATE PROCEDURE DELETE_POSITION_OVERNIGHT(
            )
            BEGIN

                UPDATE position_overnight SET is_active = 0;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACTIVE_INSTRUMENTS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACTIVE_INSTRUMENTS;

            DELIMITER |

            CREATE PROCEDURE GET_ACTIVE_INSTRUMENTS(
            )
            BEGIN

                SELECT  *
                FROM    instrument
                WHERE   is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACTIVE_INSTRUMENTS_BY_SYMBOL
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACTIVE_INSTRUMENTS_BY_SYMBOL;

            DELIMITER |

            CREATE PROCEDURE GET_ACTIVE_INSTRUMENTS_BY_SYMBOL(
                IN p_symbol VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    instrument
                WHERE   symbol = p_symbol
                    AND is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACTIVE_INSTRUMENT_IDS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACTIVE_INSTRUMENT_IDS;

            DELIMITER |

            CREATE PROCEDURE GET_ACTIVE_INSTRUMENT_IDS(
            )
            BEGIN

                SELECT  instrument_id
                FROM    instrument
                WHERE   is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ALLOCATIONS_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ALLOCATIONS_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_ALLOCATIONS_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    allocations
                WHERE   order_id = p_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ALLOCATION_BY_ALLOCATION_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ALLOCATION_BY_ALLOCATION_ID;

            DELIMITER |

            CREATE PROCEDURE GET_ALLOCATION_BY_ALLOCATION_ID(
                IN p_allocation_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    allocations
                WHERE   p_allocation_id = p_allocation_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_EXECUTIONS_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_EXECUTIONS_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_EXECUTIONS_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    executions
                WHERE   order_id = p_order_id
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_EXECUTION_BY_EXECUTION_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_EXECUTION_BY_EXECUTION_ID;

            DELIMITER |

            CREATE PROCEDURE GET_EXECUTION_BY_EXECUTION_ID(
                IN p_execution_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    executions
                WHERE   execution_id = p_execution_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID;

            DELIMITER |

            CREATE PROCEDURE GET_FIX_ENGINE_BY_ACCOUNT_INSTRUMENT_ID(
                IN p_account_id INT,
                IN p_instrument_id INT
            )
            BEGIN

                SELECT  session_id
                FROM    fix_engine_session
                WHERE   account_id = p_account_id
                AND     instrument_id = p_instrument_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_FIX_ENGINE_BY_SESSION_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_FIX_ENGINE_BY_SESSION_ID;

            DELIMITER |

            CREATE PROCEDURE GET_FIX_ENGINE_BY_SESSION_ID(
                IN p_session_id VARCHAR(48)
            )
            BEGIN

                SELECT  fix_engine_id
                FROM    fix_engine_session
                WHERE   session_id = p_session_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENTS_BY_INSTRUMENT_TYPE
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENTS_BY_INSTRUMENT_TYPE;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENTS_BY_INSTRUMENT_TYPE(
                IN p_instrument_type INT
            )
            BEGIN

                SELECT  *
                FROM    instrument
                WHERE   instrument_type = p_instrument_id
                AND     is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENTS_BY_INVESTMENT_SYSTEM(
                IN p_investment_system_id VARCHAR(16)
            )
            BEGIN

                SELECT  i.*
                FROM    instrument i, investment_system_instrument isi
                WHERE   isi.investment_system_id = p_investment_system_id
                    AND isi.instrument_id = i.instrument_id
                    AND i.is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENTS_BY_SYMBOL
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENTS_BY_SYMBOL;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENTS_BY_SYMBOL(
                IN p_symbol VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    instrument
                WHERE   symbol = p_symbol;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENTS_BY_SYMBOL_MATURITY_MONTH_YEAR(
                IN p_symbol VARCHAR(16),
                IN p_maturity_month_year VARCHAR(8)
            )
            BEGIN

                SELECT  *
                FROM    instrument
                WHERE   symbol = p_symbol
                    AND maturity_month_year = p_maturity_month_year;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENT_BY_EXCHANGE_SECURITY_ID(
                IN p_security_exchange VARCHAR(8),
                IN p_security_id VARCHAR(32)
            )
            BEGIN

                SELECT  i.*
                FROM    instrument i
                WHERE   i.security_id = p_security_id
                AND     i.security_exchange = p_security_exchange;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENT_BY_FIX_ENGINE_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENT_BY_FIX_ENGINE_ID;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENT_BY_FIX_ENGINE_ID(
                IN p_fix_engine_id VARCHAR(32)
            )
            BEGIN

                SELECT  i.*
                FROM    instrument i, market_data_instrument mdi
                WHERE   mdi.fix_engine_id = p_fix_engine_id
                AND     mdi.instrument_id = i.instrument_id
                AND     i.is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENT_BY_INSTRUMENT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENT_BY_INSTRUMENT_ID;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENT_BY_INSTRUMENT_ID(
                IN p_instrument_id INT
            )
            BEGIN

                SELECT  *
                FROM    instrument
                WHERE   instrument_id = p_instrument_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENT_SYMBOLS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENT_SYMBOLS;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENT_SYMBOLS(
            )
            BEGIN

                SELECT  symbol
                FROM    instrument
                WHERE   is_active = 1
                GROUP BY symbol
                ORDER BY symbol;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_EXECUTIONS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_EXECUTIONS;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_EXECUTIONS(
            )
            BEGIN

                SELECT  *
                FROM    executions
                WHERE   execution_status < 4
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_ORDERS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_ORDERS;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_ORDERS(
            )
            BEGIN

                SELECT  *
                FROM    orders
                WHERE   order_status < 6
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_RELEASES
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_RELEASES;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_RELEASES(
            )
            BEGIN

                SELECT  *
                FROM    releases
                WHERE   release_status < 10
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ORDER_BY_EXTERNAL_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ORDER_BY_EXTERNAL_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_ORDER_BY_EXTERNAL_ORDER_ID(
                IN p_external_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    orders
                WHERE   external_order_id = p_external_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ORDER_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ORDER_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_ORDER_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    orders
                WHERE   order_id = p_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OVERNIGHT_POSITIONS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OVERNIGHT_POSITIONS;

            DELIMITER |

            CREATE PROCEDURE GET_OVERNIGHT_POSITIONS(
            )
            BEGIN

                SELECT  *
                FROM    position_overnight
                WHERE   is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_RELEASES_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_RELEASES_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_RELEASES_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    releases
                WHERE   order_id = p_order_id
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_RELEASE_BY_RELEASE_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_RELEASE_BY_RELEASE_ID;

            DELIMITER |

            CREATE PROCEDURE GET_RELEASE_BY_RELEASE_ID(
                IN p_release_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    releases
                WHERE   release_id = p_release_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_ALLOCATION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_ALLOCATION;

            DELIMITER |

            CREATE PROCEDURE INSERT_ALLOCATION(
                IN p_id INT,
                IN p_allocation_id VARCHAR(16),
                IN p_order_id VARCHAR(16),
                IN p_account_id INT,
                IN p_instrument_id INT,
                IN p_side_code TINYINT,
                IN p_allocation_qty VARCHAR(30),
                IN p_released_qty VARCHAR(30),
                IN p_executed_qty VARCHAR(30),
                IN p_executed_val VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                CALL UPDATE_PARAMETER( 'DEALING_ENGINE', 'curr_allocation_id', p_id );

                INSERT INTO allocations (
                    allocation_id,
                    order_id,
                    account_id,
                    instrument_id,
                    side_code,
                    allocation_qty,
                    released_qty,
                    executed_qty,
                    executed_val,
                    average_prc,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_allocation_id,
                    p_order_id,
                    p_account_id,
                    p_instrument_id,
                    p_side_code,
                    p_allocation_qty,
                    p_released_qty,
                    p_executed_qty,
                    p_executed_val,
                    p_average_prc,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_EXECUTION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_EXECUTION;

            DELIMITER |

            CREATE PROCEDURE INSERT_EXECUTION(
                IN p_id INT,
                IN p_execution_id VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_account_id INT,
                IN p_order_id VARCHAR(16),
                IN p_release_id VARCHAR(16),
                IN p_investment_system_id VARCHAR(16),
                IN p_instrument_id INT,
                IN p_client_order_id VARCHAR(32),
                IN p_counterparty_execution_id VARCHAR(32),
                IN p_side_code TINYINT,
                IN p_execution_ccy CHAR(3),
                IN p_execution_status TINYINT,
                IN p_execution_prc VARCHAR(30),
                IN p_execution_qty VARCHAR(30),
                IN p_execution_val VARCHAR(30),
                IN p_leaves_qty VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_leg_type TINYINT,
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                CALL UPDATE_PARAMETER( 'DEALING_ENGINE', 'curr_execution_id', p_id );

                INSERT INTO executions (
                    execution_id,
                    session_id,
                    account_id,
                    order_id,
                    release_id,
                    investment_system_id,
                    instrument_id,
                    client_order_id,
                    counterparty_execution_id,
                    side_code,
                    execution_ccy,
                    execution_status,
                    execution_prc,
                    execution_qty,
                    execution_val,
                    leaves_qty,
                    average_prc,
                    leg_type,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_execution_id,
                    p_session_id,
                    p_account_id,
                    p_order_id,
                    p_release_id,
                    p_investment_system_id,
                    p_instrument_id,
                    p_client_order_id,
                    p_counterparty_execution_id,
                    p_side_code,
                    p_execution_ccy,
                    p_execution_status,
                    p_execution_prc,
                    p_execution_qty,
                    p_execution_val,
                    p_leaves_qty,
                    p_average_prc,
                    p_leg_type,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_ORDER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_ORDER;

            DELIMITER |

            CREATE PROCEDURE INSERT_ORDER(
                IN p_id INT,
                IN p_order_id VARCHAR(16),
                IN p_external_order_id VARCHAR(16),
                IN p_investment_system_id VARCHAR(16),
                IN p_order_type TINYINT,
                IN p_order_status TINYINT,
                IN p_instrument_id INT,
                IN p_symbol VARCHAR(16),
                IN p_tif TINYINT,
                IN p_side_code TINYINT,
                IN p_base_ccy CHAR(3),
                IN p_order_qty VARCHAR(30),
                IN p_open_qty VARCHAR(30),
                IN p_released_qty VARCHAR(30),
                IN p_executed_qty VARCHAR(30),
                IN p_limit_prc VARCHAR(30),
                IN p_stop_prc VARCHAR(30),
                IN p_mark_prc VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_entry_prc VARCHAR(30),
                IN p_released_val VARCHAR(30),
                IN p_executed_val VARCHAR(30),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                CALL UPDATE_PARAMETER( 'DEALING_ENGINE', 'curr_order_id', p_id );

                INSERT INTO orders (
                    order_id,
                    external_order_id,
                    investment_system_id,
                    order_type,
                    order_status,
                    instrument_id,
                    symbol,
                    tif,
                    side_code,
                    base_ccy,
                    order_qty,
                    open_qty,
                    released_qty,
                    executed_qty,
                    limit_prc,
                    stop_prc,
                    mark_prc,
                    average_prc,
                    entry_prc,
                    released_val,
                    executed_val,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_order_id,
                    p_external_order_id,
                    p_investment_system_id,
                    p_order_type,
                    p_order_status,
                    p_instrument_id,
                    p_symbol,
                    p_tif,
                    p_side_code,
                    p_base_ccy,
                    p_order_qty,
                    p_open_qty,
                    p_released_qty,
                    p_executed_qty,
                    p_limit_prc,
                    p_stop_prc,
                    p_mark_prc,
                    p_average_prc,
                    p_entry_prc,
                    p_released_val,
                    p_executed_val,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_POSITION_OVERNIGHT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_POSITION_OVERNIGHT;

            DELIMITER |

            CREATE PROCEDURE INSERT_POSITION_OVERNIGHT(
                IN p_account_id INT,
                IN p_instrument_id INT,
                IN p_investment_system_id VARCHAR(16),
                IN p_position_type TINYINT,
                IN p_open_pos VARCHAR(30),
                IN p_open_prc VARCHAR(30),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO position_overnight (
                    account_id,
                    instrument_id,
                    investment_system_id,
                    position_type,
                    open_pos,
                    open_prc,
                    is_active,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_account_id,
                    p_instrument_id,
                    p_investment_system_id,
                    p_position_type,
                    p_open_pos,
                    p_open_prc,
                    1,
                    p_added_datetime,
                    p_added_by,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_RELEASE
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_RELEASE;

            DELIMITER |

            CREATE PROCEDURE INSERT_RELEASE(
                IN p_id INT,
                IN p_release_id VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_account_id INT,
                IN p_order_id VARCHAR(16),
                IN p_allocation_id VARCHAR(16),
                IN p_investment_system_id VARCHAR(16),
                IN p_instrument_id INT,
                IN p_compliance_audit_id CHAR(16),
                IN p_client_order_id CHAR(20),
                IN p_counterparty_order_id VARCHAR(64),
                IN p_release_status TINYINT,
                IN p_release_type TINYINT,
                IN p_tif TINYINT,
                IN p_side_code TINYINT,
                IN p_release_qty VARCHAR(30),
                IN p_limit_prc VARCHAR(30),
                IN p_stop_prc VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_executed_val VARCHAR(30),
                IN p_executed_qty VARCHAR(30),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                CALL UPDATE_PARAMETER( 'DEALING_ENGINE', 'curr_release_id', p_id );

                INSERT INTO releases (
                    release_id,
                    session_id,
                    account_id,
                    order_id,
                    allocation_id,
                    investment_system_id,
                    instrument_id,
                    compliance_audit_id,
                    client_order_id,
                    counterparty_order_id,
                    release_status,
                    release_type,
                    tif,
                    side_code,
                    release_qty,
                    limit_prc,
                    stop_prc,
                    average_prc,
                    executed_val,
                    executed_qty,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_release_id,
                    p_session_id,
                    p_account_id,
                    p_order_id,
                    p_allocation_id,
                    p_investment_system_id,
                    p_instrument_id,
                    p_compliance_audit_id,
                    p_client_order_id,
                    p_counterparty_order_id,
                    p_release_status,
                    p_release_type,
                    p_tif,
                    p_side_code,
                    p_release_qty,
                    p_limit_prc,
                    p_stop_prc,
                    p_average_prc,
                    p_executed_val,
                    p_executed_qty,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- MARK_INSTRUMENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS MARK_INSTRUMENT;

            DELIMITER |

            CREATE PROCEDURE MARK_INSTRUMENT(
                IN p_instrument_id INT ,
                IN p_mark_prc VARCHAR(30) ,
                IN p_mark_datetime CHAR(20) ,
                IN p_updated_datetime CHAR(20) ,
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE instrument SET
                    mark_prc = p_mark_prc,
                    mark_datetime = p_mark_datetime,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE instrument_id = p_instrument_id;

                UPDATE instrument_mark_price SET
                    is_current = 0
                WHERE instrument_id = p_instrument_id
                AND is_current = 1;

                INSERT INTO instrument_mark_price (
                    instrument_id,
                    mark_prc,
                    mark_datetime,
                    is_current )
                VALUES (
                    p_instrument_id,
                    p_mark_prc,
                    p_mark_datetime,
                    1 );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_ALLOCATION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_ALLOCATION;

            DELIMITER |

            CREATE PROCEDURE UPDATE_ALLOCATION(
                IN p_allocation_id VARCHAR(16),
                IN p_order_id VARCHAR(16),
                IN p_account_id INT,
                IN p_instrument_id INT,
                IN p_side_code TINYINT,
                IN p_allocation_qty VARCHAR(30),
                IN p_released_qty VARCHAR(30),
                IN p_executed_qty VARCHAR(30),
                IN p_executed_val VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE allocations SET
                    order_id = p_order_id,
                    account_id = p_account_id,
                    instrument_id = p_instrument_id,
                    side_code = p_side_code,
                    allocation_qty = p_allocation_qty,
                    released_qty = p_released_qty,
                    executed_qty = p_executed_qty,
                    executed_val = p_executed_val,
                    average_prc = p_average_prc,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE allocation_id = p_allocation_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_EXECUTION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_EXECUTION;

            DELIMITER |

            CREATE PROCEDURE UPDATE_EXECUTION(
                IN p_execution_id VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_account_id INT,
                IN p_order_id VARCHAR(16),
                IN p_release_id VARCHAR(16),
                IN p_investment_system_id VARCHAR(16),
                IN p_instrument_id INT,
                IN p_client_order_id VARCHAR(32),
                IN p_counterparty_execution_id VARCHAR(32),
                IN p_side_code TINYINT,
                IN p_execution_ccy CHAR(3),
                IN p_execution_status TINYINT,
                IN p_execution_prc VARCHAR(30),
                IN p_execution_qty VARCHAR(30),
                IN p_execution_val VARCHAR(30),
                IN p_leaves_qty VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE executions SET
                    session_id = p_session_id,
                    account_id = p_account_id,
                    order_id = p_order_id,
                    release_id = p_release_id,
                    investment_system_id = p_investment_system_id,
                    instrument_id = p_instrument_id,
                    client_order_id = p_client_order_id,
                    counterparty_execution_id = p_counterparty_execution_id,
                    side_code = p_side_code,
                    execution_ccy = p_execution_ccy,
                    execution_status = p_execution_status,
                    execution_prc = p_execution_prc,
                    execution_qty = p_execution_qty,
                    execution_val = p_execution_val,
                    leaves_qty = p_leaves_qty,
                    average_prc = p_average_prc,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE execution_id = p_execution_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_INSTRUMENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_INSTRUMENT;

            DELIMITER |

            CREATE PROCEDURE UPDATE_INSTRUMENT(
                IN p_instrument_id INT ,
                IN p_instrument_type INT ,
                IN p_name VARCHAR(64) ,
                IN p_symbol VARCHAR(16) ,
                IN p_security_id VARCHAR(32) ,
                IN p_cusip VARCHAR(32) ,
                IN p_sedol VARCHAR(32) ,
                IN p_isin VARCHAR(32) ,
                IN p_ric VARCHAR(32) ,
                IN p_security_type VARCHAR(8) ,
                IN p_security_desc VARCHAR(16) ,
                IN p_security_exchange VARCHAR(8) ,
                IN p_maturity_month_year VARCHAR(8) ,
                IN p_base_ccy CHAR(3) ,
                IN p_last_prc VARCHAR(30) ,
                IN p_multiplier VARCHAR(30) ,
                IN p_exchange_price_factor VARCHAR(30) ,
                IN p_is_active TINYINT(1) ,
                IN p_updated_datetime CHAR(20) ,
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE instrument SET
                    instrument_type = p_instrument_type,
                    name = p_name,
                    symbol = p_symbol,
                    security_id = p_security_id,
                    cusip = p_cusip,
                    sedol = p_sedol,
                    isin = p_isin,
                    ric = p_ric,
                    security_type = p_security_type,
                    security_desc = p_security_desc,
                    security_exchange = p_security_exchange,
                    maturity_month_year = p_maturity_month_year,
                    base_ccy = p_base_ccy,
                    last_prc = p_last_prc,
                    multiplier = p_multiplier,
                    exchange_price_factor = p_exchange_price_factor,
                    is_active = p_is_active,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE instrument_id = p_instrument_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_INSTRUMENT_LAST_PRICE
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_INSTRUMENT_LAST_PRICE;

            DELIMITER |

            CREATE PROCEDURE UPDATE_INSTRUMENT_LAST_PRICE(
                IN p_instrument_id INT ,
                IN p_last_prc VARCHAR(30) ,
                IN p_updated_datetime CHAR(20) ,
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE instrument SET
                    last_prc = p_last_prc,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE instrument_id = p_instrument_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_ORDER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_ORDER;

            DELIMITER |

            CREATE PROCEDURE UPDATE_ORDER(
                IN p_order_id VARCHAR(16),
                IN p_order_type TINYINT,
                IN p_order_status TINYINT,
                IN p_instrument_id INT,
                IN p_symbol VARCHAR(16),
                IN p_tif TINYINT,
                IN p_side_code TINYINT,
                IN p_base_ccy CHAR(3),
                IN p_order_qty VARCHAR(30),
                IN p_open_qty VARCHAR(30),
                IN p_released_qty VARCHAR(30),
                IN p_executed_qty VARCHAR(30),
                IN p_limit_prc VARCHAR(30),
                IN p_stop_prc VARCHAR(30),
                IN p_mark_prc VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_released_val VARCHAR(30),
                IN p_executed_val VARCHAR(30),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE orders SET
                    order_type = p_order_type,
                    order_status = p_order_status,
                    instrument_id = p_instrument_id,
                    symbol = p_symbol,
                    tif = p_tif,
                    side_code = p_side_code,
                    base_ccy = p_base_ccy,
                    order_qty = p_order_qty,
                    open_qty = p_open_qty,
                    released_qty = p_released_qty,
                    executed_qty = p_executed_qty,
                    limit_prc = p_limit_prc,
                    stop_prc = p_stop_prc,
                    mark_prc = p_mark_prc,
                    average_prc = p_average_prc,
                    released_val = p_released_val,
                    executed_val = p_executed_val,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE order_id = p_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_RELEASE
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_RELEASE;

            DELIMITER |

            CREATE PROCEDURE UPDATE_RELEASE(
                IN p_release_id VARCHAR(16),
                IN p_session_id VARCHAR(64),
                IN p_account_id INT,
                IN p_order_id VARCHAR(16),
                IN p_investment_system_id VARCHAR(16),
                IN p_instrument_id INT,
                IN p_client_order_id CHAR(20),
                IN p_counterparty_order_id VARCHAR(64),
                IN p_release_status TINYINT,
                IN p_release_type TINYINT,
                IN p_tif TINYINT,
                IN p_side_code TINYINT,
                IN p_release_qty VARCHAR(30),
                IN p_limit_prc VARCHAR(30),
                IN p_average_prc VARCHAR(30),
                IN p_executed_val VARCHAR(30),
                IN p_executed_qty VARCHAR(30),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE releases SET
                    session_id = p_session_id,
                    account_id = p_account_id,
                    order_id = p_order_id,
                    investment_system_id = p_investment_system_id,
                    instrument_id = p_instrument_id,
                    client_order_id = p_client_order_id,
                    counterparty_order_id = p_counterparty_order_id,
                    release_status = p_release_status,
                    release_type = p_release_type,
                    tif = p_tif,
                    side_code = p_side_code,
                    release_qty = p_release_qty,
                    limit_prc = p_limit_prc,
                    average_prc = p_average_prc,
                    executed_val = p_executed_val,
                    executed_qty = p_executed_qty,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE release_id = p_release_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- CLEAR_MARKET_DATA_MAP
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS CLEAR_MARKET_DATA_MAP;

            DELIMITER |

            CREATE PROCEDURE CLEAR_MARKET_DATA_MAP(
                IN p_session_id VARCHAR(48),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE market_data_map
                   SET is_active = 0,
                       updated_datetime = p_updated_datetime,
                       updated_by = p_updated_by
                 WHERE session_id = p_session_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_CLIENT_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS DELETE_CLIENT_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE DELETE_CLIENT_ORDER_ID(
                IN p_client_order_id CHAR(20),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE  client_order_id_map
                SET     is_active = 0,
                        updated_datetime = p_updated_datetime,
                        updated_by = p_updated_by
                WHERE   client_order_id = p_client_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_CLIENT_ORDER_ID_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS DELETE_CLIENT_ORDER_ID_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE DELETE_CLIENT_ORDER_ID_BY_ORDER_ID(
                IN p_order_id VARCHAR(20),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE  client_order_id_map
                SET     is_active = 0,
                        updated_datetime = p_updated_datetime,
                        updated_by = p_updated_by
                WHERE   order_id = p_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_MARKET_DATA_MAP
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS DELETE_MARKET_DATA_MAP;

            DELIMITER |

            CREATE PROCEDURE DELETE_MARKET_DATA_MAP(
                IN p_market_data_map_id CHAR(20),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE  market_data_map
                    SET is_active = 0,
                        updated_datetime = p_updated_datetime,
                        updated_by = p_updated_by
                WHERE   market_data_map_id = p_market_data_map_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_CLIENT_ORDER_ID_MAP
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_CLIENT_ORDER_ID_MAP;

            DELIMITER |

            CREATE PROCEDURE GET_CLIENT_ORDER_ID_MAP(

            )
            BEGIN

                SELECT  *
                FROM    client_order_id_map
                WHERE   is_active = 1
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_CLIENT_ORDER_ID_MAP_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    client_order_id_map
                WHERE   order_id = p_order_id
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID;

            DELIMITER |

            CREATE PROCEDURE GET_CLIENT_ORDER_ID_MAP_BY_SESSION_ID(
                IN p_session_id VARCHAR(64)
            )
            BEGIN

                SELECT  *
                FROM    client_order_id_map
                WHERE   session_id = p_session_id
                AND     is_active = 1
                ORDER BY added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_FIX_ENGINE_SESSIONS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_FIX_ENGINE_SESSIONS;

            DELIMITER |

            CREATE PROCEDURE GET_FIX_ENGINE_SESSIONS(
            )
            BEGIN

                SELECT  *
                FROM    fix_engine_session;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID;

            DELIMITER |

            CREATE PROCEDURE GET_MARKET_DATA_MAP_ID_BY_INSTRUMENT_ID(
                IN p_instrument_id INT
            )
            BEGIN

                SELECT  market_data_map_id
                FROM    market_data_map
                WHERE   instrument_id = p_instrument_id
                    AND is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_CLIENT_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_CLIENT_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE INSERT_CLIENT_ORDER_ID(
                IN p_client_order_id CHAR(20) ,
                IN p_correlation_order_id CHAR(20) ,
                IN p_orig_client_order_id CHAR(20) ,
                IN p_session_id VARCHAR(64) ,
                IN p_release_id VARCHAR(16) ,
                IN p_order_id VARCHAR(16) ,
                IN p_instrument_id INT ,
                IN p_investment_system_id VARCHAR(16),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO client_order_id_map (
                    client_order_id,
                    correlation_order_id,
                    orig_client_order_id,
                    session_id,
                    release_id,
                    order_id,
                    instrument_id,
                    investment_system_id,
                    is_active,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_client_order_id,
                    p_correlation_order_id,
                    p_orig_client_order_id,
                    p_session_id,
                    p_release_id,
                    p_order_id,
                    p_instrument_id,
                    p_investment_system_id,
                    1,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_MARKET_DATA_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_MARKET_DATA_EVENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_MARKET_DATA_EVENT(
                IN p_instrument_id INT,
                IN p_event_type CHAR(1),
                IN p_event_action CHAR(1),
                IN p_event_prc VARCHAR(30),
                IN p_event_qty VARCHAR(30),
                IN p_event_tm CHAR(21),
                IN p_timebar_key CHAR(14),
                IN p_arrival_tm CHAR(20)
            )
            BEGIN

                INSERT INTO market_data_event (
                    instrument_id,
                    event_type,
                    event_action,
                    event_prc,
                    event_qty,
                    event_tm,
                    timebar_key,
                    arrival_tm )
                VALUES (
                    p_instrument_id,
                    p_event_type,
                    p_event_action,
                    p_event_prc,
                    p_event_qty,
                    p_event_tm,
                    p_timebar_key,
                    p_arrival_tm );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_MARKET_DATA_MAP
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_MARKET_DATA_MAP;

            DELIMITER |

            CREATE PROCEDURE INSERT_MARKET_DATA_MAP(
                IN p_market_data_map_id CHAR(20) ,
                IN p_instrument_id INT,
                IN p_session_id VARCHAR(64),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO market_data_map (
                    market_data_map_id,
                    instrument_id,
                    session_id,
                    is_active,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_market_data_map_id,
                    p_instrument_id,
                    p_session_id,
                    1,
                    p_added_datetime,
                    p_added_by,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_CLIENT_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_CLIENT_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE UPDATE_CLIENT_ORDER_ID(
                IN p_client_order_id CHAR(20) ,
                IN p_release_id VARCHAR(16) ,
                IN p_order_id VARCHAR(16) ,
                IN p_instrument_id INT ,
                IN p_investment_system_id VARCHAR(16) ,
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE client_order_id_map SET
                    release_id = p_release_id,
                    order_id = p_order_id,
                    instrument_id = p_instrument_id,
                    investment_system_id = p_investment_system_id,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE client_order_id = p_client_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENT_TIMEBARS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENT_TIMEBARS;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENT_TIMEBARS(
                IN p_instrument_id INT,
                IN p_timestep INT,
                IN p_start CHAR(12),
                IN p_stop CHAR(12)
            )
            BEGIN

                DECLARE p_beg_tm CHAR(12);
                DECLARE p_end_tm CHAR(12);
                DECLARE p_beg_datetime DATETIME;
                DECLARE p_end_datetime DATETIME;
                DECLARE p_open_prc VARCHAR(30);
                DECLARE p_close_prc VARCHAR(30);

                SET p_beg_datetime = STR_TO_DATE( p_start, '%Y%m%d%H%i' );
                SET p_end_datetime = STR_TO_DATE( p_stop, '%Y%m%d%H%i' );

                DELETE FROM agg_market_data_event;

                WHILE ( TIMESTAMPDIFF( MINUTE, p_beg_datetime, p_end_datetime ) > 0 ) DO

                    SET p_beg_tm = DATE_FORMAT( p_beg_datetime, '%Y%m%d%H%i' );
                    SET p_beg_datetime = DATE_ADD( p_beg_datetime, INTERVAL p_timestep MINUTE );
                    SET p_end_tm = DATE_FORMAT( p_beg_datetime, '%Y%m%d%H%i' );

                    SET p_open_prc = 0.0;
                    SET p_close_prc = 0.0;

                    SELECT  event_prc INTO p_open_prc
                    FROM    market_data_event
                    WHERE   market_data_event_id =
                        ( SELECT MIN( market_data_event_id )
                          FROM  market_data_event
                          WHERE instrument_id = p_instrument_id
                          AND   event_datetime BETWEEN p_beg_tm AND p_end_tm );

                    SELECT  event_prc INTO p_close_prc
                    FROM    market_data_event
                    WHERE   market_data_event_id =
                        ( SELECT MAX( market_data_event_id )
                          FROM  market_data_event
                          WHERE instrument_id = p_instrument_id
                          AND   event_datetime BETWEEN p_beg_tm AND p_end_tm );

                    INSERT INTO agg_market_data_event (
                        event_qty,
                        event_val,
                        min_event_prc,
                        max_event_prc,
                        open_prc,
                        close_prc,
                        beg_tm,
                        end_tm )
                    SELECT  IFNULL( SUM( event_qty ), 0 ) as event_qty,
                            IFNULL( SUM( event_prc * event_qty ), 0 ) as event_val,
                            IFNULL( MIN( event_prc ), 0 ) as min_event_prc,
                            IFNULL( MAX( event_prc ), 0 ) as max_event_prc,
                            p_open_prc,
                            p_close_prc,
                            p_beg_tm as beg_tm,
                            p_end_tm as end_tm
                    FROM    market_data_event
                    WHERE   instrument_id = p_instrument_id
                    AND     event_datetime BETWEEN p_beg_tm AND p_end_tm;



                END WHILE;

                SELECT * FROM agg_market_data_event;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY;

            DELIMITER |

            CREATE PROCEDURE GET_MARKET_DATA_EVENTS_BY_TIMEBAR_KEY(
                IN p_timebar_key CHAR(14)
            )
            BEGIN

                SELECT * FROM market_data_event
                WHERE timebar_key = p_timebar_key
                ORDER BY market_data_event_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_NEXT_MARKET_DATA_EVENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_NEXT_MARKET_DATA_EVENT;

            DELIMITER |

            CREATE PROCEDURE GET_NEXT_MARKET_DATA_EVENT(
                IN p_instrument_id INT,
                IN p_date CHAR(12)
            )
            BEGIN

                DECLARE id INT DEFAULT 0;

                SELECT MAX( market_data_event_id )
                INTO id
                FROM market_data_event;

                SELECT  CASE WHEN market_data_event_id IS NULL
                            THEN id
                            ELSE market_data_event_id
                        END
                INTO    id
                FROM    market_data_event
                WHERE   event_datetime >= p_date
                AND     instrument_id = p_instrument_id
                ORDER BY market_data_event_id
                LIMIT 1;

                SELECT id as market_data_event_id;


            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_REPORT_DATA
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACCOUNT_REPORT_DATA;

            DELIMITER |

            CREATE PROCEDURE GET_ACCOUNT_REPORT_DATA(
                IN p_account_id INT,
                IN p_beg_dt CHAR(10),
                IN p_end_dt CHAR(10)
            )
            BEGIN

                SELECT  a.*,
                        i.name as instrument_name,
                        i.security_exchange,
                        acc.name as account_name,
                        acc.custody_account_id,
                        acc.giveup_account_id,
                        acc.giveup_firm_id
                FROM    allocations a, instrument i, account acc
                WHERE   a.added_datetime BETWEEN p_beg_dt AND p_end_dt
                    AND a.account_id = p_account_id
                    AND a.account_id = acc.account_id
                    AND a.instrument_id = i.instrument_id
                HAVING  a.executed_qty > 0
                ORDER BY a.side_code;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID`;

            DELIMITER |

            CREATE PROCEDURE `DELETE_ACCOUNT_STRATEGY_BY_ACCOUNT_ID`(
                IN p_account_id INT
            )
            BEGIN

                DELETE FROM account_strategy
                WHERE   account_id = p_account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID`;

            DELIMITER |

            CREATE PROCEDURE `DELETE_INSTRUMENTS_BY_INVESTMENT_SYSTEM_ID`(
                IN p_investment_system_id VARCHAR(16)
            )
            BEGIN

                DELETE FROM investment_system_instrument
                WHERE investment_system_id = p_investment_system_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_STRATEGY
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `DELETE_STRATEGY`;

            DELIMITER |

            CREATE PROCEDURE `DELETE_STRATEGY`(
                IN p_strategy_id INT
            )
            BEGIN

                DELETE FROM account_strategy
                WHERE   strategy_id = p_strategy_id;

                DELETE FROM strategy
                WHERE   strategy_id = p_strategy_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNTS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNTS`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNTS`(
            )
            BEGIN

                SELECT  *
                FROM    account
                ORDER BY name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNTS_BY_INVESTMENT_SYSTEM
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNTS_BY_INVESTMENT_SYSTEM`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNTS_BY_INVESTMENT_SYSTEM`(
                IN p_investment_system_id VARCHAR(16)
            )
            BEGIN

                SELECT  a.account_id,
                        a.name
                FROM    investment_system i, account a, account_strategy a_s, strategy s
                WHERE   i.investment_system_id = p_investment_system_id
                    AND s.investment_system_id = i.investment_system_id
                    AND s.is_active = 1
                    AND a_s.strategy_id = s.strategy_id
                    AND a_s.is_enabled = 1
                    AND a_s.account_id = a.account_id
                    AND a.is_active = 1
                GROUP BY a.account_id, a.name
                ORDER BY a.account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNTS_BY_STRATEGY_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNTS_BY_STRATEGY_ID`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNTS_BY_STRATEGY_ID`(
                IN p_strategy_id INT
            )
            BEGIN

                SELECT  a.account_id,
                        a.name,
                        a.notional_val,
                        a.is_active,
                        a_str.strategy_id
                FROM    account a, account_strategy a_str
                WHERE   a.account_id = a_str.account_id
                    AND a_str.strategy_id = p_strategy_id
                ORDER BY a.name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_BY_ACCOUNT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNT_BY_ACCOUNT_ID`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNT_BY_ACCOUNT_ID`(
                IN p_account_id INT
            )
            BEGIN

                SELECT  *
                FROM    account
                WHERE   account_id = p_account_id
                ORDER BY name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_CONFIGURATION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNT_CONFIGURATION`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNT_CONFIGURATION`(
                IN p_account_id INT
            )
            BEGIN

                SELECT  name,
                        value
                FROM    account_configuration
                WHERE   account_id = p_account_id
                ORDER BY name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_INSTRUMENTS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNT_INSTRUMENTS`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNT_INSTRUMENTS`(
            )
            BEGIN

                SELECT  f.account_id,
                        i.instrument_id,
                        i.name,
                        f.session_id,
                        f.fix_engine_id
                FROM    fix_engine_session f, instrument i
                WHERE   f.instrument_id = i.instrument_id
                ORDER BY f.account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_STRATEGIES
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNT_STRATEGIES`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNT_STRATEGIES`(
            )
            BEGIN

                SELECT  *
                FROM    account_strategy
                ORDER BY account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL`;

            DELIMITER |

            CREATE PROCEDURE `GET_ACCOUNT_STRATEGY_BY_INVESTMENT_SYSTEM_SIGNAL`(
                IN p_investment_system_id VARCHAR(16),
                IN p_signal_id VARCHAR(32)
            )
            BEGIN

                SELECT  a.account_id,
                        s.strategy_id,
                        a_s.weight,
                        a_s.is_enabled,
                        a.signal_weight,
                        a.is_suppressed,
                        a.is_active as is_account_active,
                        s.is_active as is_strategy_active
                FROM    account_strategy a_s, account a, strategy s
                WHERE   s.investment_system_id = p_investment_system_id
                    AND s.signal_id = p_signal_id
                    AND s.strategy_id = a_s.strategy_id
                    AND a_s.account_id = a.account_id
                ORDER BY a.account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENT_BY_INVESTMENT_SYSTEM
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENT_BY_INVESTMENT_SYSTEM;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENT_BY_INVESTMENT_SYSTEM(
                IN p_investment_system_id VARCHAR(16),
                IN p_lookup_id VARCHAR(64)
            )
            BEGIN

                DECLARE p_instrument_id_type_id VARCHAR(8);

                --
                --  Find the insrument id type
                --

                SELECT  instrument_id_type_id INTO p_instrument_id_type_id
                FROM    instrument_lookup il
                WHERE   investment_system_id = p_investment_system_id;

                --
                --  Based on the 'instrument id type', create a dynamic
                --  select statement with the appropriate WHERE clause.
                --

                SELECT  i.*
                FROM instrument i, investment_system_instrument isi
                WHERE
                    CASE p_instrument_id_type_id
                       WHEN 'SECID' THEN i.security_id = p_lookup_id
                       WHEN 'CUSIP' THEN i.cusip = p_lookup_id
                       WHEN 'ISIN' THEN i.isin = p_lookup_id
                       WHEN 'NAME' THEN i.name = p_lookup_id
                       WHEN 'RIC' THEN i.ric = p_lookup_id
                       WHEN 'SEDOL' THEN i.sedol = p_lookup_id
                       WHEN 'SYMBOL' THEN i.symbol = p_lookup_id
                       WHEN 'INSTR' THEN i.instrument_id = p_lookup_id
                       ELSE 1=0
                    END
                AND isi.investment_system_id = p_investment_system_id
                AND isi.instrument_id = i.instrument_id
                AND i.is_active = 1
                ORDER BY 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENT_ID_TYPE_BY_INVESTMENT_SYSTEM(
                IN p_investment_system_id VARCHAR(16)
            )
            BEGIN

                --
                --  Find the insrument id type
                --

                SELECT  instrument_id_type_id
                FROM    instrument_lookup il
                WHERE   investment_system_id = p_investment_system_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INVESTMENT_SYSTEMS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_INVESTMENT_SYSTEMS`;

            DELIMITER |

            CREATE PROCEDURE `GET_INVESTMENT_SYSTEMS`(
            )
            BEGIN

                --
                --  This is how we used to do it, prior to now requiring
                --  instrument_id on the signal request.
                --
                -- SELECT  i_s.investment_system_id,
                --         i_s.name,
                --         i_s.is_active,
                --         i_l.instrument_id_type_id
                -- FROM    investment_system i_s, instrument_lookup i_l
                -- WHERE   i_s.investment_system_id = i_l.investment_system_id
                -- ORDER BY i_s.name;

                SELECT  *
                FROM    investment_system
                ORDER BY investment_system_id;


            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INVESTMENT_SYSTEMS_BY_ACCOUNT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_INVESTMENT_SYSTEMS_BY_ACCOUNT`;

            DELIMITER |

            CREATE PROCEDURE `GET_INVESTMENT_SYSTEMS_BY_ACCOUNT`(
                IN p_account_id INT
            )
            BEGIN

                SELECT  i.investment_system_id,
                        i.name,
                        i.is_active
                FROM    investment_system i, account a, account_strategy a_s, strategy s
                WHERE   a_s.account_id = p_account_id
                    AND s.strategy_id = a_s.strategy_id
                    AND s.investment_system_id = i.investment_system_id
                GROUP BY i.investment_system_id, i.name, i.is_active;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID`;

            DELIMITER |

            CREATE PROCEDURE `GET_INVESTMENT_SYSTEM_BY_INVESTMENT_SYSTEM_ID`(
                IN p_investment_system_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    investment_system
                WHERE   investment_system_id = p_investment_system_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_STRATEGIES
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_STRATEGIES`;

            DELIMITER |

            CREATE PROCEDURE `GET_STRATEGIES`(
            )
            BEGIN

                SELECT  *
                FROM    strategy
                ORDER BY name, investment_system_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_STRATEGIES_BY_ACCOUNT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_STRATEGIES_BY_ACCOUNT`;

            DELIMITER |

            CREATE PROCEDURE `GET_STRATEGIES_BY_ACCOUNT`(
                IN p_account_id INT
            )
            BEGIN

                SELECT  s.strategy_id,
                        s.investment_system_id,
                        s.signal_id,
                        s.name,
                        s.is_active,
                        a_s.weight,
                        a_s.is_enabled
                FROM    account_strategy a_s, strategy s
                WHERE   a_s.account_id = p_account_id
                    AND s.strategy_id = a_s.strategy_id
                ORDER BY s.name, s.investment_system_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_STRATEGIES_BY_INVESTMENT_SYSTEM
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_STRATEGIES_BY_INVESTMENT_SYSTEM`;

            DELIMITER |

            CREATE PROCEDURE `GET_STRATEGIES_BY_INVESTMENT_SYSTEM`(
                IN p_investment_system_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    strategy
                WHERE   investment_system_id = p_investment_system_id
                ORDER BY name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_STRATEGY_BY_STRATEGY_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_STRATEGY_BY_STRATEGY_ID`;

            DELIMITER |

            CREATE PROCEDURE `GET_STRATEGY_BY_STRATEGY_ID`(
                IN p_strategy_id INT
            )
            BEGIN

                SELECT  strategy_id,
                        investment_system_id,
                        signal_id,
                        name,
                        is_active
                FROM    strategy
                WHERE   strategy_id = p_strategy_id
                ORDER BY 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_STRATEGY_CONFIGURATION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_STRATEGY_CONFIGURATION`;

            DELIMITER |

            CREATE PROCEDURE `GET_STRATEGY_CONFIGURATION`(
                IN p_account_id INT,
                IN p_strategy_id INT
            )
            BEGIN

                SELECT  strategy_configuration_id,
                        name,
                        value
                FROM    strategy_configuration s
                WHERE   s.account_id = p_account_id
                    AND s.strategy_id = p_strategy_id
                ORDER BY name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_STRATEGY_CONFIGURATION_BY_ACCOUNT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_STRATEGY_CONFIGURATION_BY_ACCOUNT`;

            DELIMITER |

            CREATE PROCEDURE `GET_STRATEGY_CONFIGURATION_BY_ACCOUNT`(
                IN p_account_id INT
            )
            BEGIN

                SELECT  *
                FROM    strategy_configuration s
                WHERE   s.account_id = p_account_id
                ORDER BY name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_UNPROCESSED_SIGNALS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_UNPROCESSED_SIGNALS;

            DELIMITER |

            CREATE PROCEDURE GET_UNPROCESSED_SIGNALS(
            )
            BEGIN

                SELECT  ss.strategy_signal_id,
                        ss.strategy_id,
                        ss.request_id,
                        ss.instrument_id,
                        ss.instrument_name,
                        ss.last_prc,
                        ss.signal_qty,
                        ss.signal_limit_prc,
                        ss.is_suppressed,
                        ss.is_processed,
                        ss.added_datetime,
                        ss.added_by,
                        ss.updated_datetime,
                        ss.updated_by
                FROM    strategy_signal ss, strategy s
                WHERE   ss.strategy_id = s.strategy_id
                    AND ss.is_processed = 0
                ORDER BY ss.added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_UNPROCESSED_SIGNALS_BY_REQUEST
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_UNPROCESSED_SIGNALS_BY_REQUEST;

            DELIMITER |

            CREATE PROCEDURE GET_UNPROCESSED_SIGNALS_BY_REQUEST(
                IN p_request_id VARCHAR(64),
                IN p_investment_system_id VARCHAR(16)
            )
            BEGIN

                SELECT  ss.strategy_signal_id,
                        ss.strategy_id,
                        ss.request_id,
                        ss.instrument_id,
                        ss.instrument_name,
                        ss.last_prc,
                        ss.signal_qty,
                        ss.signal_limit_prc,
                        ss.is_suppressed,
                        ss.is_processed,
                        ss.added_datetime,
                        ss.added_by,
                        ss.updated_datetime,
                        ss.updated_by
                FROM    strategy_signal ss, strategy s
                WHERE   ss.strategy_id = p_strategy_id
                    AND ss.strategy_id = s.strategy_id
                    AND s.investment_system_id = p_investment_system_id
                    AND ss.is_processed = 0
                ORDER BY ss.added_datetime;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_ACCOUNT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_ACCOUNT;

            DELIMITER |

            CREATE PROCEDURE INSERT_ACCOUNT(
                IN p_name VARCHAR(64) ,
                IN p_market_account_id VARCHAR(32) ,
                IN p_custody_account_id VARCHAR(32) ,
                IN p_notional_val VARCHAR(30) ,
                IN p_profit_target VARCHAR(30) ,
                IN p_stop_loss VARCHAR(30) ,
                IN p_signal_weight INT,
                IN p_is_active TINYINT,
                IN p_added_datetime CHAR(17),
                IN p_added_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO account(
                    name,
                    market_account_id,
                    custody_account_id,
                    notional_val,
                    profit_target,
                    stop_loss,
                    signal_weight,
                    is_suppressed,
                    is_active,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_name,
                    p_market_account_id,
                    p_custody_account_id,
                    p_notional_val,
                    p_profit_target,
                    p_stop_loss,
                    p_signal_weight,
                    0,
                    p_is_active,
                    p_added_datetime,
                    p_added_by,
                    p_added_datetime,
                    p_added_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_ACCOUNT_STRATEGY
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `INSERT_ACCOUNT_STRATEGY`;

            DELIMITER |

            CREATE PROCEDURE `INSERT_ACCOUNT_STRATEGY`(
                IN p_account_id INT,
                IN p_strategy_id INT,
                IN p_weight INT,
                IN p_is_enabled TINYINT
            )
            BEGIN

                INSERT INTO account_strategy (
                    account_id,
                    strategy_id,
                    weight,
                    is_enabled )
                VALUES (
                    p_account_id,
                    p_strategy_id,
                    p_weight,
                    p_is_enabled );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_INVESTMENT_SYSTEM
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_INVESTMENT_SYSTEM;

            DELIMITER |

            CREATE PROCEDURE INSERT_INVESTMENT_SYSTEM(
                IN p_investment_system_id VARCHAR(16) ,
                IN p_name VARCHAR(32)
            )
            BEGIN

                INSERT INTO investment_system (
                    investment_system_id,
                    name,
                    is_active )
                VALUES (
                    p_investment_system_id,
                    p_name,
                    0 );

                INSERT INTO instrument_lookup (
                    investment_system_id,
                    instrument_id_type_id )
                VALUES (
                    p_investment_system_id,
                    'SYMBOL' );

                INSERT into investment_system_instrument (
                    investment_system_id, instrument_id )
                    SELECT p_investment_system_id, instrument_id
                    FROM instrument
                    WHERE is_active = 1;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_INVESTMENT_SYSTEM_INSTRUMENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_INVESTMENT_SYSTEM_INSTRUMENT;

            DELIMITER |

            CREATE PROCEDURE INSERT_INVESTMENT_SYSTEM_INSTRUMENT(
                IN p_investment_system_id VARCHAR(16) ,
                IN p_instrument_id INT
            )
            BEGIN

                INSERT INTO investment_system_instrument (
                    investment_system_id,
                    instrument_id )
                VALUES (
                    p_investment_system_id,
                    p_instrument_id );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_STRATEGY
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `INSERT_STRATEGY`;

            DELIMITER |

            CREATE PROCEDURE `INSERT_STRATEGY`(
                IN p_investment_system_id VARCHAR(16),
                IN p_signal_id VARCHAR(32),
                IN p_name VARCHAR(32),
                IN p_default_weight INT
            )
            BEGIN

                INSERT INTO strategy (
                    investment_system_id,
                    signal_id,
                    name,
                    default_weight,
                    is_active )
                VALUES (
                    p_investment_system_id,
                    p_signal_id,
                    p_name,
                    p_default_weight,
                    0
                );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_STRATEGY_CONFIGURATION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `INSERT_STRATEGY_CONFIGURATION`;

            DELIMITER |

            CREATE PROCEDURE `INSERT_STRATEGY_CONFIGURATION`(
                IN p_account_id INT,
                IN p_strategy_id INT,
                IN p_name VARCHAR(16),
                IN p_value VARCHAR(32)
            )
            BEGIN

                INSERT INTO strategy_configuration (
                    account_id,
                    strategy_id,
                    name,
                    value )
                VALUES (
                    p_account_id,
                    p_strategy_id,
                    p_name,
                    p_value
                );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_STRATEGY_SIGNAL
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `INSERT_STRATEGY_SIGNAL`;

            DELIMITER |

            CREATE PROCEDURE `INSERT_STRATEGY_SIGNAL`(
                IN p_strategy_id INT,
                IN p_account_id INT,
                IN p_investment_system_id VARCHAR(16),
                IN p_request_id VARCHAR(64),
                IN p_instrument_id INT,
                IN p_instrument_name VARCHAR(64),
                IN p_last_prc VARCHAR(30),
                IN p_limit_prc VARCHAR(30),
                IN p_signal_qty VARCHAR(30),
                IN p_allocation_qty VARCHAR(30),
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO strategy_signal (
                    strategy_id,
                    account_id,
                    investment_system_id,
                    request_id,
                    instrument_id,
                    instrument_name,
                    last_prc,
                    limit_prc,
                    signal_qty,
                    allocation_qty,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_strategy_id,
                    p_account_id,
                    p_investment_system_id,
                    p_request_id,
                    p_instrument_id,
                    p_instrument_name,
                    p_last_prc,
                    p_limit_prc,
                    p_signal_qty,
                    p_allocation_qty,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_ACCOUNT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_ACCOUNT;

            DELIMITER |

            CREATE PROCEDURE UPDATE_ACCOUNT(
                IN p_account_id INT ,
                IN p_name VARCHAR(64) ,
                IN p_market_account_id VARCHAR(32) ,
                IN p_custody_account_id VARCHAR(32) ,
                IN p_giveup_account_id VARCHAR(32) ,
                IN p_giveup_firm_id VARCHAR(32) ,
                IN p_notional_val VARCHAR(30) ,
                IN p_profit_target VARCHAR(30) ,
                IN p_stop_loss VARCHAR(30) ,
                IN p_signal_weight INT ,
                IN p_is_suppressed TINYINT ,
                IN p_is_active TINYINT,
                IN p_updated_datetime CHAR(17),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE account SET
                    name = p_name,
                    market_account_id = p_market_account_id,
                    custody_account_id = p_custody_account_id,
                    giveup_account_id = p_giveup_account_id,
                    giveup_firm_id = p_giveup_firm_id,
                    notional_val = p_notional_val,
                    profit_target = p_profit_target,
                    stop_loss = p_stop_loss,
                    signal_weight = p_signal_weight,
                    is_suppressed = p_is_suppressed,
                    is_active = p_is_active
                WHERE account_id = p_account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_INSTRUMENT_LOOKUP
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_INSTRUMENT_LOOKUP;

            DELIMITER |

            CREATE PROCEDURE UPDATE_INSTRUMENT_LOOKUP(
                IN p_investment_system_id VARCHAR(16) ,
                IN p_instrument_id_type_id VARCHAR(8)
            )
            BEGIN

                DELETE FROM instrument_lookup
                WHERE investment_system_id = p_investment_system_id;

                INSERT INTO instrument_lookup (
                    investment_system_id,
                    instrument_id_type_id )
                VALUES (
                    p_investment_system_id,
                    p_instrument_id_type_id );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_INVESTMENT_SYSTEM
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_INVESTMENT_SYSTEM;

            DELIMITER |

            CREATE PROCEDURE UPDATE_INVESTMENT_SYSTEM(
                IN p_investment_system_id VARCHAR(16) ,
                IN p_name VARCHAR(32) ,
                IN p_is_active TINYINT
            )
            BEGIN

                UPDATE investment_system
                SET name = p_name,
                    is_active = p_is_active
                WHERE investment_system_id = p_investment_system_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_STRATEGY
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `UPDATE_STRATEGY`;

            DELIMITER |

            CREATE PROCEDURE `UPDATE_STRATEGY`(
                IN p_strategy_id INT,
                IN p_signal_id VARCHAR(32),
                IN p_name VARCHAR(32),
                IN p_default_weight INT ,
                IN p_is_active TINYINT
            )
            BEGIN

                UPDATE strategy
                SET signal_id = p_signal_id,
                    name = p_name,
                    default_weight = p_default_weight,
                    is_active = p_is_active
                WHERE strategy_id = p_strategy_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_SYSTEM_PARAMETERS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_SYSTEM_PARAMETERS`;

            DELIMITER |

            CREATE PROCEDURE `GET_SYSTEM_PARAMETERS`(
            )
            BEGIN

                SELECT  parameter_id,
                        category,
                        name,
                        value
                FROM    system_parameter
                ORDER BY category, name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_SYSTEM_PARAMETERS_BY_CATEGORY
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `GET_SYSTEM_PARAMETERS_BY_CATEGORY`;

            DELIMITER |

            CREATE PROCEDURE `GET_SYSTEM_PARAMETERS_BY_CATEGORY`(
                IN p_category VARCHAR(64)
            )
            BEGIN

                SELECT  parameter_id,
                        category,
                        name,
                        value
                FROM    system_parameter
                WHERE   category = p_category
                ORDER BY category, name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_PARAMETER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `UPDATE_PARAMETER`;

            DELIMITER |

            CREATE PROCEDURE `UPDATE_PARAMETER`(
                IN  p_category VARCHAR(64),
                IN  p_name VARCHAR(64),
                IN  p_value VARCHAR(64)
            )
            BEGIN

                UPDATE  system_parameter
                SET     value = p_value
                WHERE   category = p_category
                    AND name = p_name;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- VALIDATE_DEALING_ENGINE_PARAMETERS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `VALIDATE_DEALING_ENGINE_PARAMETERS`;

            DELIMITER |

            CREATE PROCEDURE `VALIDATE_DEALING_ENGINE_PARAMETERS`(
            )
            BEGIN

                DECLARE p_id VARCHAR(64);

                --
                --  Ensure that the system_parameter table contains the following
                --  [category / name] values:
                --
                --      DEALING_ENGINE / curr_order_id
                --      DEALING_ENGINE / curr_release_id
                --      DEALING_ENGINE / curr_execution_id
                --      DEALING_ENGINE / curr_allocation_id
                --

                --
                --  curr_order_id
                --

                SELECT  value INTO p_id
                FROM    system_parameter
                WHERE   category = 'DEALING_ENGINE'
                AND     name = 'curr_order_id';

                IF( p_id IS NULL ) THEN

                    INSERT INTO system_parameter (
                        category,
                        name,
                        value )
                    VALUES (
                        'DEALING_ENGINE',
                        'curr_order_id',
                        '0' );
                END IF;

                --
                --  curr_release_id
                --

                SELECT  value INTO p_id
                FROM    system_parameter
                WHERE   category = 'DEALING_ENGINE'
                AND     name = 'curr_release_id';

                IF( p_id IS NULL ) THEN

                    INSERT INTO system_parameter (
                        category,
                        name,
                        value )
                    VALUES (
                        'DEALING_ENGINE',
                        'curr_release_id',
                        '0' );
                END IF;

                --
                --  curr_execution_id
                --

                SELECT  value INTO p_id
                FROM    system_parameter
                WHERE   category = 'DEALING_ENGINE'
                AND     name = 'curr_execution_id';

                IF( p_id IS NULL ) THEN

                    INSERT INTO system_parameter (
                        category,
                        name,
                        value )
                    VALUES (
                        'DEALING_ENGINE',
                        'curr_execution_id',
                        '0' );
                END IF;

                --
                --  curr_allocation_id
                --

                SELECT  value INTO p_id
                FROM    system_parameter
                WHERE   category = 'DEALING_ENGINE'
                AND     name = 'curr_allocation_id';

                IF( p_id IS NULL ) THEN

                    INSERT INTO system_parameter (
                        category,
                        name,
                        value )
                    VALUES (
                        'DEALING_ENGINE',
                        'curr_allocation_id',
                        '0' );
                END IF;

                --
                --  Return parameter values
                --

                SELECT  parameter_id,
                        category,
                        name,
                        value
                FROM    system_parameter
                WHERE   category = 'DEALING_ENGINE'
                ORDER BY parameter_id;


            END|
            DELIMITER ;



 -- ---------------------
 --
 -- VALIDATE_MKTDATA_ENGINE_PARAMETERS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `VALIDATE_MKTDATA_ENGINE_PARAMETERS`;

            DELIMITER |

            CREATE PROCEDURE `VALIDATE_MKTDATA_ENGINE_PARAMETERS`(
            )
            BEGIN

                DECLARE p_id VARCHAR(64);

                --
                --  Ensure that the system_parameter table contains the following
                --  [category / name] values:
                --
                --      MKTDATA_ENGINE / curr_transaction_id
                --

                --
                --  curr_transaction_id
                --

                SELECT  value INTO p_id
                FROM    system_parameter
                WHERE   category = 'MKTDATA_ENGINE'
                AND     name = 'curr_transaction_id';

                IF( p_id IS NULL ) THEN

                    INSERT INTO system_parameter (
                        category,
                        name,
                        value )
                    VALUES (
                        'MKTDATA_ENGINE',
                        'curr_transaction_id',
                        '0' );
                END IF;

                --
                --  Return parameter values
                --

                SELECT  parameter_id,
                        category,
                        name,
                        value
                FROM    system_parameter
                WHERE   category = 'MKTDATA_ENGINE'
                ORDER BY parameter_id;


            END|
            DELIMITER ;



 -- ---------------------
 --
 -- VALIDATE_SIGNAL_ENGINE_PARAMETERS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `VALIDATE_SIGNAL_ENGINE_PARAMETERS`;

            DELIMITER |

            CREATE PROCEDURE `VALIDATE_SIGNAL_ENGINE_PARAMETERS`(
            )
            BEGIN

                DECLARE p_id VARCHAR(64);

                --
                --  Ensure that the system_parameter table contains the following
                --  [category / name] values:
                --
                --      SIGNAL_ENGINE / curr_signal_id
                --      SIGNAL_ENGINE / curr_transaction_id
                --

                --
                --  curr_signal_id
                --

                SELECT  value INTO p_id
                FROM    system_parameter
                WHERE   category = 'SIGNAL_ENGINE'
                AND     name = 'curr_signal_id';

                IF( p_id IS NULL ) THEN

                    INSERT INTO system_parameter (
                        category,
                        name,
                        value )
                    VALUES (
                        'SIGNAL_ENGINE',
                        'curr_signal_id',
                        '0' );
                END IF;

                --
                --  curr_transaction_id
                --

                SELECT  value INTO p_id
                FROM    system_parameter
                WHERE   category = 'SIGNAL_ENGINE'
                AND     name = 'curr_transaction_id';

                IF( p_id IS NULL ) THEN

                    INSERT INTO system_parameter (
                        category,
                        name,
                        value )
                    VALUES (
                        'SIGNAL_ENGINE',
                        'curr_transaction_id',
                        '0' );
                END IF;

                --
                --  Return parameter values
                --

                SELECT  parameter_id,
                        category,
                        name,
                        value
                FROM    system_parameter
                WHERE   category = 'SIGNAL_ENGINE'
                ORDER BY parameter_id;


            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DIREXION_DUPLICATE_TEST
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS DIREXION_DUPLICATE_TEST;

            DELIMITER |

            CREATE PROCEDURE DIREXION_DUPLICATE_TEST(
                IN p_instrument_id INT,
                IN p_strategy_id VARCHAR(32),
                IN p_order_type VARCHAR(8),
                IN p_side VARCHAR(8),
                IN p_price VARCHAR(16)
            )
            BEGIN

                SELECT *
                FROM tradestation_order tso, orders o
                WHERE tso.instrument_id = p_instrument_id
                AND tso.strategy_id = p_strategy_id
                AND tso.order_type = p_order_type
                AND tso.side = p_side
                AND tso.price = p_price
                AND tso.order_id = o.order_id
                AND o.order_status < 5
                ORDER BY tradestation_order_id desc;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DIREXION_GET_TRADESTATION_ORDER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS DIREXION_GET_TRADESTATION_ORDER;

            DELIMITER |

            CREATE PROCEDURE DIREXION_GET_TRADESTATION_ORDER(
                IN p_instrument_id INT,
                IN p_strategy_id VARCHAR(32),
                IN p_order_type VARCHAR(8),
                IN p_side VARCHAR(8),
                IN p_price VARCHAR(16)
            )
            BEGIN

                SELECT *
                FROM tradestation_order
                WHERE instrument_id = p_instrument_id
                AND strategy_id = p_strategy_id
                AND order_type = p_order_type
                AND side = p_side
                AND price = p_price
                AND order_id != 'NULL'
                ORDER BY tradestation_order_id desc;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_TRADESTATION_ORDERS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_TRADESTATION_ORDERS;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_TRADESTATION_ORDERS(
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   is_open = 1
                ORDER BY order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_TRADESTATION_ORDERS2
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_TRADESTATION_ORDERS2;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_TRADESTATION_ORDERS2(
                IN p_instrument_id INT,
                IN p_strategy_id VARCHAR(32),
                IN p_open_close VARCHAR(8),
                IN p_order_type VARCHAR(8)
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   is_open = 1
                AND     instrument_id = p_instrument_id
                AND     strategy_id = p_strategy_id
                AND     open_close = p_open_close
                AND     order_type = p_order_type
                ORDER BY order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_ID(
                IN p_instrument_id INT
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   is_open = 1
                AND     instrument_id = p_instrument_id
                ORDER BY order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_ID(
                IN p_instrument_id INT,
                IN p_strategy_id VARCHAR(32)
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   is_open = 1
                AND     instrument_id = p_instrument_id
                AND     strategy_id = p_strategy_id
                ORDER BY order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID;

            DELIMITER |

            CREATE PROCEDURE GET_OPEN_TRADESTATION_ORDERS_BY_STRATEGY_ID(
                IN p_strategy_id VARCHAR(32)
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   is_open = 1
                AND     strategy_id = p_strategy_id
                ORDER BY order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_TRADESTATION_INSTRUMENT
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_TRADESTATION_INSTRUMENT;

            DELIMITER |

            CREATE PROCEDURE GET_TRADESTATION_INSTRUMENT(
                IN p_symbol VARCHAR(8)
            )
            BEGIN

                SELECT  tis.*, i.name
                FROM    tradestation_instrument_symbol tis, instrument i
                WHERE   tis.symbol = p_symbol
                AND     tis.instrument_id = i.instrument_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION;

            DELIMITER |

            CREATE PROCEDURE GET_TRADESTATION_ORDERS_BY_INSTRUMENT_STRATEGY_EXPIRATION(
                IN p_instrument_id INT,
                IN p_strategy_id VARCHAR(32),
                IN p_expiration_dt CHAR(12)
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   instrument_id = p_instrument_id
                AND     signal_id = p_signal_id
                AND     expiration_dt = p_expiration_dt
                ORDER BY order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_TRADESTATION_ORDER_BY_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_TRADESTATION_ORDER_BY_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_TRADESTATION_ORDER_BY_ORDER_ID(
                IN p_order_id VARCHAR(16)
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   order_id = p_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID;

            DELIMITER |

            CREATE PROCEDURE GET_TRADESTATION_ORDER_BY_TRADESTATION_ORDER_ID(
                IN p_tradestation_order_id INT
            )
            BEGIN

                SELECT  *
                FROM    tradestation_order
                WHERE   tradestation_order_id = p_tradestation_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_TRADESTATION_ORDER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_TRADESTATION_ORDER;

            DELIMITER |

            CREATE PROCEDURE INSERT_TRADESTATION_ORDER(
                IN p_strategy_id VARCHAR(32),
                IN p_order_data VARCHAR(64),
                IN p_order_status VARCHAR(16),
                IN p_order_type VARCHAR(8),
                IN p_open_close VARCHAR(8),
                IN p_signal VARCHAR(32),
                IN p_timestamp VARCHAR(24),
                IN p_side VARCHAR(8),
                IN p_quantity VARCHAR(8),
                IN p_symbol VARCHAR(8),
                IN p_price VARCHAR(16),
                IN p_instrument_id INT,
                IN p_instrument_name VARCHAR(64),
                IN p_signal_quantity VARCHAR(30),
                IN p_signal_price VARCHAR(30),
                IN p_expiration_dt CHAR(12),
                IN p_is_executed TINYINT,
                IN p_is_open TINYINT,
                IN p_added_datetime CHAR(20),
                IN p_added_by VARCHAR(16),
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                INSERT INTO tradestation_order (
                    strategy_id,
                    order_data,
                    order_status,
                    order_type,
                    open_close,
                    signal,
                    timestamp,
                    side,
                    quantity,
                    symbol,
                    price,
                    instrument_id,
                    instrument_name,
                    signal_quantity,
                    signal_price,
                    expiration_dt,
                    is_executed,
                    is_open,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by )
                VALUES (
                    p_strategy_id,
                    p_order_data,
                    p_order_status,
                    p_order_type,
                    p_open_close,
                    p_signal,
                    p_timestamp,
                    p_side,
                    p_quantity,
                    p_symbol,
                    p_price,
                    p_instrument_id,
                    p_instrument_name,
                    p_signal_quantity,
                    p_signal_price,
                    p_expiration_dt,
                    p_is_executed,
                    p_is_open,
                    p_added_datetime,
                    p_added_by,
                    p_updated_datetime,
                    p_updated_by );

                SELECT * FROM tradestation_order WHERE tradestation_order_id = @@IDENTITY;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_TRADESTATION_ORDER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_TRADESTATION_ORDER;

            DELIMITER |

            CREATE PROCEDURE UPDATE_TRADESTATION_ORDER(
                IN p_tradestation_order_id INT,
                IN p_order_id VARCHAR(16),
                IN p_is_executed TINYINT,
                IN p_is_open TINYINT,
                IN p_updated_datetime CHAR(20),
                IN p_updated_by VARCHAR(16)
            )
            BEGIN

                UPDATE tradestation_order
                SET
                    order_id = p_order_id,
                    is_executed = p_is_executed,
                    is_open = p_is_open,
                    updated_datetime = p_updated_datetime,
                    updated_by = p_updated_by
                WHERE tradestation_order_id = p_tradestation_order_id;

                SELECT * FROM tradestation_order WHERE tradestation_order_id = p_tradestation_order_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- DELETE_USER_APPLICATION_BY_USER_ID
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS DELETE_USER_APPLICATION_BY_USER_ID;

            DELIMITER |

            CREATE PROCEDURE DELETE_USER_APPLICATION_BY_USER_ID(
                IN p_user_id INT
            )
            BEGIN

                DELETE FROM user_application
                WHERE user_id = p_user_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_ACCOUNTS_BY_USER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_ACCOUNTS_BY_USER;

            DELIMITER |

            CREATE PROCEDURE GET_ACCOUNTS_BY_USER(
                IN p_user_id INT
            )
            BEGIN

                SELECT  a.*
                FROM    account a, user_account ua
                WHERE   ua.user_id = p_user_id
                AND     ua.account_id = a.account_id
                ORDER BY a.account_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_APPLICATIONS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_APPLICATIONS;

            DELIMITER |

            CREATE PROCEDURE GET_APPLICATIONS(
            )
            BEGIN

                SELECT  *
                FROM    application
                ORDER BY application_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_APPLICATIONS_BY_USER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_APPLICATIONS_BY_USER;

            DELIMITER |

            CREATE PROCEDURE GET_APPLICATIONS_BY_USER(
                IN p_user_id INT
            )
            BEGIN

                SELECT  application_id
                FROM    user_application
                WHERE   user_id = p_user_id
                ORDER BY application_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INSTRUMENTS_BY_USER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INSTRUMENTS_BY_USER;

            DELIMITER |

            CREATE PROCEDURE GET_INSTRUMENTS_BY_USER(
                IN p_user_id INT
            )
            BEGIN

                SELECT  i.*
                FROM    instrument i, user_instrument ui
                WHERE   ui.user_id = p_user_id
                AND     ui.instrument_id = i.instrument_id
                AND     ui.instrument_type = i.instrument_type
                ORDER BY i.instrument_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_INVESTMENT_SYSTEMS_BY_USER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_INVESTMENT_SYSTEMS_BY_USER;

            DELIMITER |

            CREATE PROCEDURE GET_INVESTMENT_SYSTEMS_BY_USER(
                IN p_user_id INT
            )
            BEGIN

                SELECT  i.*
                FROM    investment_system i, user_investment_system uis
                WHERE   uis.user_id = p_user_id
                AND     uis.investment_system_id = i.investment_system_id
                ORDER BY i.investment_system_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- GET_USERS
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS GET_USERS;

            DELIMITER |

            CREATE PROCEDURE GET_USERS(
            )
            BEGIN

                SELECT  *
                FROM    user
                ORDER BY user_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_USER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_USER;

            DELIMITER |

            CREATE PROCEDURE INSERT_USER(
                IN p_username VARCHAR(32),
                IN p_password VARCHAR(32),
                IN p_firstname VARCHAR(64),
                IN p_lastname VARCHAR(64)
            )
            BEGIN

                INSERT INTO user (
                    username,
                    password,
                    firstname,
                    lastname,
                    is_active )
                VALUES (
                    p_username,
                    p_password,
                    p_firstname,
                    p_lastname,
                    0 );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- INSERT_USER_APPLICATION
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS INSERT_USER_APPLICATION;

            DELIMITER |

            CREATE PROCEDURE INSERT_USER_APPLICATION(
                IN p_user_id INT ,
                IN p_application_id CHAR(8)
            )
            BEGIN

                INSERT INTO user_application (
                    user_id,
                    application_id
                ) VALUES (
                    p_user_id,
                    p_application_id );

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- UPDATE_USER
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS UPDATE_USER;

            DELIMITER |

            CREATE PROCEDURE UPDATE_USER(
                IN p_user_id INT,
                IN p_username VARCHAR(32),
                IN p_password VARCHAR(32),
                IN p_firstname VARCHAR(64),
                IN p_lastname VARCHAR(64),
                IN p_is_active TINYINT
            )
            BEGIN

                UPDATE user
                SET username = p_username,
                    password = p_password,
                    firstname = p_firstname,
                    lastname = p_lastname,
                    is_active = p_is_active
                WHERE user_id = p_user_id;

            END|
            DELIMITER ;



 -- ---------------------
 --
 -- USER_LOGIN
 --
 -- ---------------------




            DROP PROCEDURE IF EXISTS `USER_LOGIN`;

            DELIMITER |

            CREATE PROCEDURE `USER_LOGIN`(
                IN p_username VARCHAR(32),
                IN p_password VARCHAR(32),
                IN p_application_id CHAR(8),
                IN p_session VARCHAR(64)
            )
            BEGIN

                INSERT INTO login_audit (
                    username,
                    password,
                    application,
                    session,
                    timestamp )
                VALUES (
                    p_username,
                    p_password,
                    p_application_id,
                    p_session,
                    UTC_TIMESTAMP() );

                SELECT  u.user_id
                FROM    user u, user_application ua, application a
                WHERE   u.username = p_username
                AND     u.password = p_password
                AND     u.is_active = 1
                AND     ua.user_id = u.user_id
                AND     ua.application_id = a.application_id
                AND     a.application_id = p_application_id;

            END|
            DELIMITER ;





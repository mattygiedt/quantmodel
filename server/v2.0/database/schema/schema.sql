SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `quantmodel` DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci ;
USE `quantmodel`;

-- -----------------------------------------------------
-- Table `quantmodel`.`system_parameter`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`system_parameter` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`system_parameter` (
  `parameter_id` INT NOT NULL AUTO_INCREMENT ,
  `category` VARCHAR(64) NOT NULL ,
  `name` VARCHAR(64) NOT NULL ,
  `value` VARCHAR(64) NULL ,
  PRIMARY KEY (`parameter_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`orders`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`orders` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`orders` (
  `order_id` VARCHAR(16) NOT NULL ,
  `external_order_id` VARCHAR(16) NULL ,
  `investment_system_id` VARCHAR(16) NULL ,
  `order_type` TINYINT UNSIGNED NULL ,
  `order_status` TINYINT UNSIGNED NULL ,
  `instrument_id` INT UNSIGNED NULL ,
  `symbol` VARCHAR(16) NULL ,
  `tif` TINYINT UNSIGNED NULL ,
  `side_code` TINYINT UNSIGNED NULL ,
  `base_ccy` CHAR(3) NULL ,
  `order_qty` VARCHAR(30) NULL ,
  `open_qty` VARCHAR(30) NULL ,
  `released_qty` VARCHAR(30) NULL ,
  `executed_qty` VARCHAR(30) NULL ,
  `limit_prc` VARCHAR(30) NULL ,
  `stop_prc` VARCHAR(30) NULL ,
  `mark_prc` VARCHAR(30) NULL ,
  `average_prc` VARCHAR(30) NULL ,
  `entry_prc` VARCHAR(30) NULL ,
  `released_val` VARCHAR(30) NULL ,
  `executed_val` VARCHAR(30) NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`order_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`releases`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`releases` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`releases` (
  `release_id` VARCHAR(16) NOT NULL ,
  `session_id` VARCHAR(64) NULL ,
  `account_id` INT NULL ,
  `order_id` VARCHAR(16) NULL ,
  `allocation_id` VARCHAR(16) NULL ,
  `investment_system_id` VARCHAR(16) NULL ,
  `instrument_id` INT NULL ,
  `compliance_audit_id` CHAR(16) NULL ,
  `client_order_id` CHAR(20) NULL ,
  `counterparty_order_id` VARCHAR(64) NULL ,
  `release_status` TINYINT NULL ,
  `release_type` TINYINT NULL ,
  `tif` TINYINT NULL ,
  `side_code` TINYINT NULL ,
  `release_qty` VARCHAR(30) NULL ,
  `limit_prc` VARCHAR(30) NULL ,
  `stop_prc` VARCHAR(30) NULL ,
  `average_prc` VARCHAR(30) NULL ,
  `executed_val` VARCHAR(30) NULL ,
  `executed_qty` VARCHAR(30) NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`release_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`executions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`executions` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`executions` (
  `execution_id` VARCHAR(16) NOT NULL ,
  `session_id` VARCHAR(64) NULL ,
  `account_id` INT NULL ,
  `order_id` VARCHAR(16) NULL ,
  `release_id` VARCHAR(16) NULL ,
  `investment_system_id` VARCHAR(16) NULL ,
  `instrument_id` INT NULL ,
  `client_order_id` VARCHAR(32) NULL ,
  `counterparty_execution_id` VARCHAR(32) NULL ,
  `side_code` TINYINT NULL ,
  `execution_ccy` CHAR(3) NULL ,
  `execution_status` TINYINT UNSIGNED NULL ,
  `execution_prc` VARCHAR(30) NULL ,
  `execution_qty` VARCHAR(30) NULL ,
  `execution_val` VARCHAR(30) NULL ,
  `leaves_qty` VARCHAR(30) NULL ,
  `average_prc` VARCHAR(30) NULL ,
  `leg_type` TINYINT NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`execution_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`account`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`account` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`account` (
  `account_id` INT NOT NULL AUTO_INCREMENT ,
  `parent_account_id` INT NULL ,
  `name` VARCHAR(64) NOT NULL ,
  `market_account_id` VARCHAR(32) NULL ,
  `custody_account_id` VARCHAR(32) NULL ,
  `giveup_account_id` VARCHAR(32) NULL ,
  `giveup_firm_id` VARCHAR(32) NULL ,
  `notional_val` VARCHAR(30) NULL ,
  `profit_target` VARCHAR(30) NULL ,
  `stop_loss` VARCHAR(30) NULL ,
  `signal_weight` INT NULL ,
  `is_suppressed` TINYINT NULL ,
  `is_active` TINYINT NULL ,
  `contact_name` VARCHAR(45) NULL ,
  `contact_email` VARCHAR(45) NULL ,
  `contact_phone` VARCHAR(45) NULL ,
  `trading_firm_name` VARCHAR(45) NULL ,
  `trading_firm_email` VARCHAR(45) NULL ,
  `trading_firm_phone` VARCHAR(45) NULL ,
  `added_datetime` CHAR(17) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(17) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`account_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`investment_system`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`investment_system` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`investment_system` (
  `investment_system_id` VARCHAR(16) NOT NULL ,
  `name` VARCHAR(32) NOT NULL ,
  `is_active` TINYINT NOT NULL ,
  PRIMARY KEY (`investment_system_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`strategy`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`strategy` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`strategy` (
  `strategy_id` INT NOT NULL AUTO_INCREMENT ,
  `investment_system_id` VARCHAR(16) NOT NULL ,
  `signal_id` VARCHAR(32) NOT NULL ,
  `name` VARCHAR(32) NOT NULL ,
  `default_weight` INT NULL ,
  `is_active` TINYINT NOT NULL ,
  PRIMARY KEY (`strategy_id`) ,
  CONSTRAINT `fk_strategy_investment_system1`
    FOREIGN KEY (`investment_system_id` )
    REFERENCES `quantmodel`.`investment_system` (`investment_system_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_strategy_investment_system1` ON `quantmodel`.`strategy` (`investment_system_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`account_strategy`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`account_strategy` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`account_strategy` (
  `account_id` INT NOT NULL ,
  `strategy_id` INT NOT NULL ,
  `weight` INT NULL ,
  `is_enabled` TINYINT NULL ,
  PRIMARY KEY (`account_id`, `strategy_id`) ,
  CONSTRAINT `fk_account_strategy_account1`
    FOREIGN KEY (`account_id` )
    REFERENCES `quantmodel`.`account` (`account_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_account_strategy_strategy1`
    FOREIGN KEY (`strategy_id` )
    REFERENCES `quantmodel`.`strategy` (`strategy_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_account_strategy_account1` ON `quantmodel`.`account_strategy` (`account_id` ASC) ;

CREATE INDEX `fk_account_strategy_strategy1` ON `quantmodel`.`account_strategy` (`strategy_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`strategy_signal`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`strategy_signal` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`strategy_signal` (
  `strategy_signal_id` INT NOT NULL AUTO_INCREMENT ,
  `strategy_id` INT NOT NULL ,
  `account_id` INT NULL ,
  `investment_system_id` VARCHAR(16) NULL ,
  `request_id` VARCHAR(64) NULL ,
  `instrument_id` INT NOT NULL ,
  `instrument_name` VARCHAR(64) NULL ,
  `last_prc` VARCHAR(30) NULL ,
  `limit_prc` VARCHAR(30) NULL ,
  `signal_qty` VARCHAR(30) NOT NULL ,
  `allocation_qty` VARCHAR(30) NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`strategy_signal_id`) )
ENGINE = InnoDB;

CREATE INDEX `REQ_ID_IDX` ON `quantmodel`.`strategy_signal` (`request_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`instrument_id_type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`instrument_id_type` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`instrument_id_type` (
  `instrument_id_type_id` VARCHAR(8) NOT NULL ,
  `name` VARCHAR(32) NOT NULL ,
  PRIMARY KEY (`instrument_id_type_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`instrument_lookup`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`instrument_lookup` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`instrument_lookup` (
  `investment_system_id` VARCHAR(16) NOT NULL ,
  `instrument_id_type_id` VARCHAR(8) NOT NULL ,
  PRIMARY KEY (`instrument_id_type_id`, `investment_system_id`) ,
  CONSTRAINT `fk_instrument_lookup_instrument_id_type1`
    FOREIGN KEY (`instrument_id_type_id` )
    REFERENCES `quantmodel`.`instrument_id_type` (`instrument_id_type_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_instrument_lookup_investment_system1`
    FOREIGN KEY (`investment_system_id` )
    REFERENCES `quantmodel`.`investment_system` (`investment_system_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_instrument_lookup_instrument_id_type1` ON `quantmodel`.`instrument_lookup` (`instrument_id_type_id` ASC) ;

CREATE INDEX `fk_instrument_lookup_investment_system1` ON `quantmodel`.`instrument_lookup` (`investment_system_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`allocations`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`allocations` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`allocations` (
  `allocation_id` VARCHAR(16) NOT NULL ,
  `order_id` VARCHAR(16) NULL ,
  `account_id` INT UNSIGNED NULL ,
  `instrument_id` INT UNSIGNED NULL ,
  `side_code` TINYINT NULL ,
  `allocation_qty` VARCHAR(30) NULL ,
  `released_qty` VARCHAR(30) NULL ,
  `executed_qty` VARCHAR(30) NULL ,
  `executed_val` VARCHAR(30) NULL ,
  `average_prc` VARCHAR(30) NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`allocation_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`user` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`user` (
  `user_id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `username` VARCHAR(32) NULL ,
  `password` VARCHAR(32) NULL ,
  `firstname` VARCHAR(64) NULL ,
  `lastname` VARCHAR(64) NULL ,
  `is_active` TINYINT NULL ,
  PRIMARY KEY (`user_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`application`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`application` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`application` (
  `application_id` CHAR(8) NOT NULL ,
  `application_name` VARCHAR(64) NULL ,
  `is_active` TINYINT NULL ,
  PRIMARY KEY (`application_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`user_application`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`user_application` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`user_application` (
  `user_id` INT UNSIGNED NOT NULL ,
  `application_id` CHAR(8) NOT NULL ,
  PRIMARY KEY (`user_id`, `application_id`) ,
  CONSTRAINT `fk_user_application_user1`
    FOREIGN KEY (`user_id` )
    REFERENCES `quantmodel`.`user` (`user_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_user_application_application1`
    FOREIGN KEY (`application_id` )
    REFERENCES `quantmodel`.`application` (`application_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_user_application_user1` ON `quantmodel`.`user_application` (`user_id` ASC) ;

CREATE INDEX `fk_user_application_application1` ON `quantmodel`.`user_application` (`application_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`login_audit`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`login_audit` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`login_audit` (
  `username` VARCHAR(32) NULL ,
  `password` VARCHAR(32) NULL ,
  `application` VARCHAR(8) NULL ,
  `session` VARCHAR(64) NULL ,
  `timestamp` TIMESTAMP NULL )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`client_order_id_map`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`client_order_id_map` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`client_order_id_map` (
  `client_order_id` CHAR(20) NOT NULL ,
  `correlation_order_id` CHAR(20) NULL ,
  `orig_client_order_id` CHAR(20) NULL ,
  `session_id` VARCHAR(64) NULL ,
  `release_id` VARCHAR(16) NULL ,
  `order_id` VARCHAR(16) NULL ,
  `instrument_id` INT NULL ,
  `investment_system_id` VARCHAR(16) NULL ,
  `is_active` TINYINT NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`client_order_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`market_data_map`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`market_data_map` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`market_data_map` (
  `market_data_map_id` CHAR(20) NOT NULL ,
  `instrument_id` INT NULL ,
  `session_id` VARCHAR(48) NULL ,
  `is_active` TINYINT NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`market_data_map_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`position_overnight`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`position_overnight` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`position_overnight` (
  `position_overnight_id` INT NOT NULL AUTO_INCREMENT ,
  `account_id` INT NULL ,
  `instrument_id` INT NULL ,
  `investment_system_id` VARCHAR(16) NULL ,
  `position_type` TINYINT NULL ,
  `open_pos` VARCHAR(30) NULL ,
  `open_prc` VARCHAR(30) NULL ,
  `is_active` TINYINT NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`position_overnight_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`application_functionality`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`application_functionality` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`application_functionality` (
  `application_functionality_id` INT NOT NULL AUTO_INCREMENT ,
  `application_id` CHAR(8) NOT NULL ,
  `name` VARCHAR(32) NULL ,
  `is_active` TINYINT NULL ,
  PRIMARY KEY (`application_functionality_id`) ,
  CONSTRAINT `fk_application_functionality_application1`
    FOREIGN KEY (`application_id` )
    REFERENCES `quantmodel`.`application` (`application_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_application_functionality_application1` ON `quantmodel`.`application_functionality` (`application_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`investment_system_instrument`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`investment_system_instrument` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`investment_system_instrument` (
  `investment_system_id` VARCHAR(16) NOT NULL ,
  `instrument_id` INT NOT NULL ,
  PRIMARY KEY (`instrument_id`, `investment_system_id`) ,
  CONSTRAINT `fk_investment_system_instrument_investment_system1`
    FOREIGN KEY (`investment_system_id` )
    REFERENCES `quantmodel`.`investment_system` (`investment_system_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_investment_system_instrument_investment_system1` ON `quantmodel`.`investment_system_instrument` (`investment_system_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`instrument_type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`instrument_type` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`instrument_type` (
  `instrument_type_id` INT NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(16) NOT NULL ,
  PRIMARY KEY (`instrument_type_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`audit_event`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`audit_event` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`audit_event` (
  `audit_event_id` INT NOT NULL AUTO_INCREMENT ,
  `order_id` VARCHAR(16) NULL ,
  `element_id` VARCHAR(16) NULL ,
  `element_type` CHAR(1) NULL ,
  `audit_desc` VARCHAR(256) NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`audit_event_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`market_data_instrument`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`market_data_instrument` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`market_data_instrument` (
  `session_id` VARCHAR(64) NOT NULL ,
  `instrument_id` INT NOT NULL ,
  PRIMARY KEY (`instrument_id`, `session_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`fix_engine_session`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`fix_engine_session` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`fix_engine_session` (
  `account_id` INT NOT NULL ,
  `instrument_id` INT NOT NULL ,
  `session_id` VARCHAR(64) NOT NULL ,
  PRIMARY KEY (`account_id`, `instrument_id`, `session_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`tradestation_order`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`tradestation_order` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`tradestation_order` (
  `tradestation_order_id` INT NOT NULL AUTO_INCREMENT ,
  `order_id` VARCHAR(16) NULL ,
  `strategy_id` VARCHAR(32) NULL ,
  `order_data` VARCHAR(64) NULL ,
  `order_status` VARCHAR(16) NULL ,
  `order_type` VARCHAR(8) NULL ,
  `open_close` VARCHAR(8) NULL ,
  `signal` VARCHAR(32) NULL ,
  `timestamp` VARCHAR(24) NULL ,
  `side` VARCHAR(8) NULL ,
  `quantity` VARCHAR(8) NULL ,
  `symbol` VARCHAR(8) NULL ,
  `price` VARCHAR(16) NULL ,
  `instrument_id` INT NULL ,
  `instrument_name` VARCHAR(64) NULL ,
  `signal_quantity` VARCHAR(30) NULL ,
  `signal_price` VARCHAR(30) NULL ,
  `expiration_dt` CHAR(12) NULL ,
  `is_executed` TINYINT NULL ,
  `is_open` TINYINT NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`tradestation_order_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`tradestation_instrument_symbol`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`tradestation_instrument_symbol` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`tradestation_instrument_symbol` (
  `symbol` VARCHAR(8) NOT NULL ,
  `instrument_id` INT NOT NULL ,
  `unit_multiplier` INT NULL ,
  `price_factor` DECIMAL(22,6) NULL ,
  `expiration_tm` CHAR(4) NULL ,
  `is_active` TINYINT NULL ,
  PRIMARY KEY (`symbol`, `instrument_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`signal_proxy_log`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`signal_proxy_log` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`signal_proxy_log` (
  `signal_proxy_log_id` INT NOT NULL AUTO_INCREMENT ,
  `port` INT NULL ,
  `socket_id` VARCHAR(16) NULL ,
  `message_type` VARCHAR(16) NULL ,
  `data` VARCHAR(2048) NULL ,
  `is_open` TINYINT NULL ,
  `is_delivered` TINYINT NULL ,
  `added_datetime` VARCHAR(17) NULL ,
  `updated_datetime` VARCHAR(17) NULL ,
  PRIMARY KEY (`signal_proxy_log_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`signal_adapter_log`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`signal_adapter_log` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`signal_adapter_log` (
  `signal_adapter_log_id` INT NOT NULL AUTO_INCREMENT ,
  `port` INT NULL ,
  `data` VARCHAR(2048) NULL ,
  `is_open` TINYINT NULL ,
  `has_error` TINYINT NULL ,
  `added_datetime` VARCHAR(17) NULL ,
  `updated_datetime` VARCHAR(17) NULL ,
  PRIMARY KEY (`signal_adapter_log_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`instrument_mark_price`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`instrument_mark_price` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`instrument_mark_price` (
  `instrument_mark_price_id` INT NOT NULL AUTO_INCREMENT ,
  `instrument_id` INT NULL ,
  `mark_prc` VARCHAR(30) NULL ,
  `mark_datetime` CHAR(20) NULL ,
  `is_current` TINYINT NULL ,
  PRIMARY KEY (`instrument_mark_price_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`request_proxy_log`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`request_proxy_log` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`request_proxy_log` (
  `request_proxy_log_id` INT NOT NULL AUTO_INCREMENT ,
  `direction` CHAR(8) NULL ,
  `socket_id` VARCHAR(32) NULL ,
  `peer_addr` VARCHAR(16) NULL ,
  `data` VARCHAR(2048) NULL ,
  `added_datetime` CHAR(17) NULL ,
  `updated_datetime` CHAR(17) NULL ,
  PRIMARY KEY (`request_proxy_log_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`fix_engine`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`fix_engine` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`fix_engine` (
  `fix_engine_id` VARCHAR(64) NOT NULL ,
  `session_id` VARCHAR(64) NOT NULL ,
  `is_active` TINYINT NULL ,
  PRIMARY KEY (`fix_engine_id`, `session_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`user_account`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`user_account` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`user_account` (
  `user_id` INT UNSIGNED NOT NULL ,
  `account_id` INT NOT NULL ,
  PRIMARY KEY (`user_id`, `account_id`) ,
  CONSTRAINT `fk_user_account_user1`
    FOREIGN KEY (`user_id` )
    REFERENCES `quantmodel`.`user` (`user_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_user_account_account1`
    FOREIGN KEY (`account_id` )
    REFERENCES `quantmodel`.`account` (`account_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_user_account_user1` ON `quantmodel`.`user_account` (`user_id` ASC) ;

CREATE INDEX `fk_user_account_account1` ON `quantmodel`.`user_account` (`account_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`user_investment_system`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`user_investment_system` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`user_investment_system` (
  `user_id` INT UNSIGNED NOT NULL ,
  `investment_system_id` VARCHAR(16) NOT NULL ,
  PRIMARY KEY (`user_id`, `investment_system_id`) ,
  CONSTRAINT `fk_user_investment_system_user1`
    FOREIGN KEY (`user_id` )
    REFERENCES `quantmodel`.`user` (`user_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_user_investment_system_investment_system1`
    FOREIGN KEY (`investment_system_id` )
    REFERENCES `quantmodel`.`investment_system` (`investment_system_id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_user_investment_system_user1` ON `quantmodel`.`user_investment_system` (`user_id` ASC) ;

CREATE INDEX `fk_user_investment_system_investment_system1` ON `quantmodel`.`user_investment_system` (`investment_system_id` ASC) ;


-- -----------------------------------------------------
-- Table `quantmodel`.`account_limit`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`account_limit` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`account_limit` (
  `account_id` INT NOT NULL ,
  `symbol` VARCHAR(16) NOT NULL ,
  `security_exchange` VARCHAR(16) NOT NULL ,
  `session_id` VARCHAR(64) NULL ,
  `max_open_long_qty` INT NULL ,
  `max_open_short_qty` INT NULL ,
  `max_release_qty` INT NULL ,
  `is_active` TINYINT NULL ,
  `added_datetime` CHAR(17) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(17) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`account_id`, `symbol`, `security_exchange`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`dealing_audit`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`dealing_audit` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`dealing_audit` (
  `dealing_audit_id` INT NOT NULL AUTO_INCREMENT ,
  `request_id` VARCHAR(64) NULL ,
  `request_type` VARCHAR(32) NULL ,
  `request_ts` CHAR(17) NULL ,
  `response_id` CHAR(16) NULL ,
  `response_type` VARCHAR(16) NULL ,
  `response_ts` CHAR(17) NULL ,
  `error_msg` VARCHAR(255) NULL ,
  `fix_engine_id` VARCHAR(32) NULL ,
  `investment_system_id` VARCHAR(16) NULL ,
  `order_id` VARCHAR(16) NULL ,
  `release_id` VARCHAR(16) NULL ,
  `execution_id` VARCHAR(16) NULL ,
  `session_id` VARCHAR(64) NULL ,
  `account_id` VARCHAR(16) NULL ,
  `instrument_id` VARCHAR(16) NULL ,
  `side_code` CHAR(1) NULL ,
  `tif` CHAR(1) NULL ,
  `limit_prc` VARCHAR(30) NULL ,
  `stop_prc` VARCHAR(30) NULL ,
  `order_status` CHAR(1) NULL ,
  `order_qty` VARCHAR(30) NULL ,
  `order_released_qty` VARCHAR(30) NULL ,
  `order_executed_qty` VARCHAR(30) NULL ,
  `order_average_prc` VARCHAR(30) NULL ,
  `release_status` CHAR(1) NULL ,
  `release_qty` VARCHAR(30) NULL ,
  `release_executed_qty` VARCHAR(30) NULL ,
  `release_average_prc` VARCHAR(30) NULL ,
  `execution_status` CHAR(1) NULL ,
  `execution_qty` VARCHAR(30) NULL ,
  `execution_prc` VARCHAR(30) NULL ,
  `added_datetime` CHAR(17) NULL ,
  `added_by` VARCHAR(20) NULL ,
  PRIMARY KEY (`dealing_audit_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`fix_audit`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`fix_audit` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`fix_audit` (
  `fix_audit_id` INT NOT NULL AUTO_INCREMENT ,
  `session_id` VARCHAR(64) NOT NULL ,
  `message_direction` VARCHAR(8) NOT NULL ,
  `account` VARCHAR(16) NULL ,
  `avgpx` VARCHAR(30) NULL ,
  `clordid` VARCHAR(32) NULL ,
  `cumqty` VARCHAR(30) NULL ,
  `execid` VARCHAR(32) NULL ,
  `execrefid` VARCHAR(32) NULL ,
  `exectranstype` VARCHAR(2) NULL ,
  `lastpx` VARCHAR(30) NULL ,
  `lastshares` VARCHAR(30) NULL ,
  `msgtype` VARCHAR(2) NULL ,
  `orderid` VARCHAR(32) NULL ,
  `orderqty` VARCHAR(30) NULL ,
  `ordstatus` VARCHAR(2) NULL ,
  `ordtype` VARCHAR(2) NULL ,
  `origclordid` VARCHAR(32) NULL ,
  `price` VARCHAR(30) NULL ,
  `securityid` VARCHAR(32) NULL ,
  `sendercompid` VARCHAR(32) NULL ,
  `sendersubid` VARCHAR(32) NULL ,
  `sendingtime` VARCHAR(32) NULL ,
  `shares` VARCHAR(30) NULL ,
  `side` VARCHAR(2) NULL ,
  `symbol` VARCHAR(16) NULL ,
  `targetcompid` VARCHAR(32) NULL ,
  `targetsubid` VARCHAR(32) NULL ,
  `text` VARCHAR(255) NULL ,
  `timeinforce` VARCHAR(2) NULL ,
  `transacttime` VARCHAR(32) NULL ,
  `stoppx` VARCHAR(30) NULL ,
  `senderlocationid` VARCHAR(16) NULL ,
  `exectype` VARCHAR(2) NULL ,
  `leavesqty` VARCHAR(30) NULL ,
  `securitytype` VARCHAR(8) NULL ,
  `maturitymonthyear` VARCHAR(45) NULL ,
  `customerorfirm` VARCHAR(2) NULL ,
  `securityexchange` VARCHAR(16) NULL ,
  `maxshow` VARCHAR(30) NULL ,
  `manualorderindicator` VARCHAR(2) NULL ,
  `cticode` VARCHAR(2) NULL ,
  `correlationclordid` VARCHAR(32) NULL ,
  `added_datetime` CHAR(17) NULL ,
  `added_by` VARCHAR(20) NULL ,
  PRIMARY KEY (`fix_audit_id`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `quantmodel`.`compliance_audit`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`compliance_audit` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`compliance_audit` (
  `compliance_audit_id` INT NOT NULL ,
  `session_id` VARCHAR(64) NULL ,
  `account_id` INT NULL ,
  `instrument_id` INT NULL ,
  `order_id` VARCHAR(16) NULL ,
  `allocation_id` VARCHAR(16) NULL ,
  `release_id` VARCHAR(16) NULL ,
  `side_code` TINYINT NULL ,
  `release_qty` VARCHAR(30) NULL ,
  `worst_case_long_qty` INT NULL ,
  `worst_case_short_qty` INT NULL ,
  `curr_open_long_qty` INT NULL ,
  `curr_open_short_qty` INT NULL ,
  `max_open_long_qty` INT NULL ,
  `max_open_short_qty` INT NULL ,
  `max_release_qty` INT NULL ,
  `result_code` VARCHAR(8) NULL ,
  `added_datetime` CHAR(17) NULL ,
  `added_by` VARCHAR(20) NULL ,
  PRIMARY KEY (`compliance_audit_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `quantmodel`.`instrument`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `quantmodel`.`instrument` ;

CREATE  TABLE IF NOT EXISTS `quantmodel`.`instrument` (
  `instrument_id` INT NOT NULL ,
  `instrument_type` INT NOT NULL ,
  `name` VARCHAR(64) NOT NULL ,
  `symbol` VARCHAR(16) NOT NULL ,
  `cusip` VARCHAR(32) NULL ,
  `sedol` VARCHAR(32) NULL ,
  `isin` VARCHAR(32) NULL ,
  `ric` VARCHAR(32) NULL ,
  `cfi_code` VARCHAR(8) NULL ,
  `security_id` VARCHAR(32) NULL ,
  `security_type` VARCHAR(8) NULL ,
  `security_desc` VARCHAR(16) NULL ,
  `security_exchange` VARCHAR(16) NULL ,
  `maturity_month_year` VARCHAR(8) NULL ,
  `base_ccy` CHAR(3) NOT NULL ,
  `last_prc` VARCHAR(30) NULL ,
  `mark_prc` VARCHAR(30) NULL ,
  `mark_datetime` CHAR(20) NULL ,
  `multiplier` VARCHAR(30) NULL ,
  `exchange_price_factor` VARCHAR(30) NULL ,
  `rollover_datetime` CHAR(20) NULL ,
  `is_active` TINYINT NOT NULL ,
  `added_datetime` CHAR(20) NULL ,
  `added_by` VARCHAR(16) NULL ,
  `updated_datetime` CHAR(20) NULL ,
  `updated_by` VARCHAR(16) NULL ,
  PRIMARY KEY (`instrument_id`, `instrument_type`) )
ENGINE = InnoDB;

CREATE INDEX `idx_symbol` ON `quantmodel`.`instrument` (`symbol` ASC) ;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

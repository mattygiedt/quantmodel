
all:	deploy_dev deploy_uat

deploy_dev:
	cp schema/schema.sql ../../../../release/dev/database 
	cp schema/init.sql ../../../../release/dev/database
	cp schema/tt_instruments.sql ../../../../release/dev/database
	cp schema/cme_instruments.sql ../../../../release/dev/database
	cp procs/procs.sql ../../../../release/dev/database


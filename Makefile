all: help

build:
	docker build -t ziquid/uslce .

debug-off: run
	docker exec -t -i usl15-game-web-1 cp /usr/local/etc/php/php.ini{-production,}
	$(warning "TODO: Disable xdebug in php.ini")
	# docker exec -t -i usl15-game-web-1 apache2ctl restart
	docker container restart usl15-game-web-1

debug-on: run
	docker exec -t -i usl15-game-web-1 cp /usr/local/etc/php/php.ini{-development,}
	$(warning "TODO: Enable xdebug in php.ini")
	# docker exec -t -i usl15-game-web-1 apache2ctl restart
	docker container restart usl15-game-web-1

help:
	@echo "make build - Build all custom docker image(s) needed for this app"
	@echo "make debug-on/off - En/Disable PHP debugging in the web container"
	@echo "make help - Display this help message"
	@echo "make run - Run the docker containers"
	@echo "make sqlc - Connect to the MySQL container via the mysql CLI"
	@echo "make ssh - SSH into the web container"
	@echo "make stop - Stop the docker containers"

run: # build
	docker compose up -d

sqlc: run
	docker exec -t -i usl15-game-db-1 mysql -u root -p

ssh: run
	docker exec -t -i usl15-game-web-1 /bin/bash

stop:
	docker compose stop

.PHONY: all build debug-off debug-on help run sqlc ssh

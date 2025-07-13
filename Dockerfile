# from https://github.com/docker-library/drupal/blob/e7202cc9967a41cf4e14aeacd32433ddd01b8b01/7/php8.2/apache-bookworm/Dockerfile

# build this and tag it as 'ziquid/zg':
# $ docker build -t ziquid/zg .

LABEL org.opencontainers.image.source="https://github.com/ziquid/usl15-game"

FROM php:8.2-apache-bookworm

ENV APACHE_DOCUMENT_ROOT=/var/www/html/web

RUN sed -ri -e 's!/var/www/html!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/sites-available/*.conf
RUN sed -ri -e 's!/var/www/!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/apache2.conf /etc/apache2/conf-available/*.conf

# install the PHP extensions we need
RUN set -eux; \
	\
	if command -v a2enmod; then \
# https://github.com/drupal/drupal/blob/d91d8d0a6d3ffe5f0b6dde8c2fbe81404843edc5/.htaccess (references both mod_expires and mod_rewrite explicitly)
		a2enmod expires rewrite; \
	fi; \
	\
	savedAptMark="$(apt-mark showmanual)"; \
	\
	apt-get update; \
	apt-get install -y --no-install-recommends \
		libfreetype6-dev \
		libjpeg-dev \
		libpng-dev \
		libpq-dev \
		libwebp-dev \
		libzip-dev \
	; \
	\
	docker-php-ext-configure gd \
		--with-freetype \
		--with-jpeg=/usr \
		--with-webp \
	; \
	\
	docker-php-ext-install -j "$(nproc)" \
		gd \
		opcache \
		pdo_mysql \
		zip \
	; \
	\
# reset apt-mark's "manual" list so that "purge --auto-remove" will remove all build dependencies
	apt-mark auto '.*' > /dev/null; \
	apt-mark manual $savedAptMark; \
	ldd "$(php -r 'echo ini_get("extension_dir");')"/*.so \
		| awk '/=>/ { so = $(NF-1); if (index(so, "/usr/local/") == 1) { next }; gsub("^/(usr/)?", "", so); printf "*%s\n", so }' \
		| sort -u \
		| xargs -r dpkg-query -S \
		| cut -d: -f1 \
		| sort -u \
		| xargs -rt apt-mark manual; \
	\
	apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false; \
	rm -rf /var/lib/apt/lists/*

# set recommended PHP.ini settings
# see https://secure.php.net/manual/en/opcache.installation.php
RUN { \
		echo 'opcache.memory_consumption=128'; \
		echo 'opcache.interned_strings_buffer=8'; \
		echo 'opcache.max_accelerated_files=4000'; \
		echo 'opcache.revalidate_freq=60'; \
	} > /usr/local/etc/php/conf.d/opcache-recommended.ini

# Use the default production configuration
RUN cp "$PHP_INI_DIR/php.ini-production" "$PHP_INI_DIR/php.ini"

# Install various PHP extensions
ADD --chmod=0755 https://github.com/mlocati/docker-php-extension-installer/releases/latest/download/install-php-extensions \
  /usr/local/bin/

RUN apt update -y
RUN apt install -y default-mysql-client unzip exiftool git-core gnupg2 imagemagick openssl pv rsync ssh wget
RUN install-php-extensions @fix_letsencrypt
RUN install-php-extensions apcu
RUN install-php-extensions bcmath
RUN install-php-extensions bz2
RUN install-php-extensions calendar
RUN install-php-extensions exif
# RUN install-php-extensions gd
RUN install-php-extensions gettext
RUN install-php-extensions imagick
RUN install-php-extensions imap
RUN install-php-extensions intl
RUN install-php-extensions ldap
RUN install-php-extensions mbstring
RUN install-php-extensions memcached
RUN install-php-extensions mysqli
RUN install-php-extensions oauth
# RUN install-php-extensions opcache
RUN install-php-extensions pcntl
RUN install-php-extensions pdo
# RUN install-php-extensions pdo_mysql
# RUN install-php-extensions pdo_pgsql
RUN install-php-extensions redis
RUN install-php-extensions soap
RUN install-php-extensions xdebug
RUN install-php-extensions xhprof
# RUN install-php-extensions zip

RUN chsh -s /bin/bash www-data
RUN mkdir -p /var/www/.composer
RUN chown -R www-data:www-data /var/www
RUN apt-get -y clean
RUN apt-get -y autoclean
RUN apt-get -y autoremove
RUN rm -rf /var/lib/apt/lists/*
RUN rm -rf /var/lib/cache/*
RUN rm -rf /var/lib/log/*
RUN rm -rf /tmp/*

# Install Composer
COPY --from=composer /usr/bin/composer /usr/bin/composer

# Install Drupal
COPY . /var/www/html
WORKDIR /var/www/html

# Install drush
RUN composer install # --ignore-platform-reqs --no-scripts
RUN ln -sf /var/www/html/vendor/bin/drush /usr/local/bin


# vim:set ft=dockerfile:

#!/bin/bash

composer install
DRUSH=$PWD/vendor/bin/drush

# DRUSH=`which d7 2>/dev/null`
# [ "$DRUSH" == "" ] && DRUSH=`which d6 2>/dev/null`
# [ "$DRUSH" == "" ] && DRUSH=`which drush 2>/dev/null`

SCOPE=local
echo "$PWD" | grep -s -q prod && SCOPE=prod
echo "$PWD" | grep -s -q dev && SCOPE=dev

cd web
chmod ug+w sites/default
$DRUSH -y updb

if [ "$SCOPE" = dev ]; then
  sudo chown -R www-data:www-data sites/default/files
fi

if [ "$SCOPE" = prod ]; then
  sudo chown -R www-data:www-data sites/default/files
fi

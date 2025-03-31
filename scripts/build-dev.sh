#!/usr/bin/env bash
# This script builds the dev site.

# shellcheck disable=SC2046
# shellcheck disable=SC2164

set -e
if ! echo $PWD | grep -s -q dev ; then
  echo "This script is only for the dev instance."
  exit 1
fi

cd $(dirname "$0")/..
echo --:\  $(date) :--
git pull -f
./build.sh
cd web
drush cc all
echo drush cron
drush sql-dump > ~ubuntu/usl15.dev.drupal.sql
gzip -f ~ubuntu/usl15.dev.drupal.sql

for a in stl1904 stlouis cg detroit wonderland; do
  echo drush sql-dump --database=game_$a
  drush sql-dump --database=game_$a > ~ubuntu/$a.dev.game.sql
  gzip -f ~ubuntu/$a.dev.game.sql
done

echo -n luck and active player accounts:' '
drush sqlq --database=game_stlouis 'select sum(luck), count(id) from users where level >= 25 and meta <> "admin" and meta <> "staff" and meta <> "zombie" and meta NOT LIKE "ai%";'
echo -n online now:' '
drush sqlq 'select count(sid) as anon_users from sessions where uid = 0 and timestamp >= '$(date -d "15 minutes ago" +%s)';'

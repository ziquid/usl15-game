#!/bin/sh

set -x

# copy stlouis to robber barons
sed -e 's,stlouis/,robber_barons/,gi' \
  -e 's,St. Louis,Robber Barons,g' \
  -e 's,stlouis_,robber_barons_,g' \
  -e 's,stlouis content,robber barons content,g' \
  -e 's,#stlouis,#robber_barons,g' \
  -e 's,Uprising: ,,g' \
  -e 's,game_stlouis,game_robber_barons,g' \
  -e 's,Influence,Spirituality,g' \
  -e 's,function robber_barons_cron,function robber_barons_copy_of_stlouis_cron,' \
  -e 's,real_celestial_glory_cron,robber_barons_copy_of_cg_cron,' \
  -e 's,real_robber_barons_cron,robber_barons_cron,' \
  < ../stlouis/stlouis.module > robber_barons.module

sed -e 's,1ca443,2c73f4,gi' \
  -e 's,stlouis_wise_old_man,robber_barons_welcome_man,g' \
  -e 's,0e5221,032152,gi' -e 's,188433,183384,g' \
  -e 's,0d0f0d,0a141e,gi' -e 's,1aa443,1a43a4,gi' \
  < ../stlouis/stlouis.css > robber_barons.css


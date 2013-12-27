#!/bin/sh
TMPFILE=$(mktemp -t utracall.XXXXXXXXXX) || exit 1
UTRAC=$(which utrac)
[ $? -ne 0 ] && error_exit "utrac must be installed for this script to work."

clean_exit()
{
  # Ignorons 'Ctrl-C'...
  trap '' 2
    rm -f $TMPFILE
  trap 2 # Remettons Ctrl-C en mode normal
  exit
}

trap 'clean_exit' 2


for fichier in $(find ./ -type f | grep -v svn | xargs file -i | grep -e 'text' | awk -F: '{print $1}'); do
  echo $fichier
  utrac -t utf-8/lf $fichier > $TMPFILE || exit 1
  cp -f $TMPFILE $fichier
done
rm -f $TMPFILE



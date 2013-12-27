#!/bin/sh

if [ $# -lt 4 ]; then
  echo "Opķrandes manquantes."
  echo "Utilisation: $(basename $0) old-prefix old-suffix new-prefix new-suffix"
  exit 2
fi

OLD_PRE=$1
OLD_SUF=$2
NEW_PRE=$3
NEW_SUF=$4

#-------------------------------------------------------------------------------
OLD=$OLD_PRE$OLD_SUF
NEW=$NEW_PRE$NEW_SUF

if [ -f .$OLD ]; then

  # renomme les fichiers et rķpertoires
  echo "Rename .$OLD in .$NEW"
  mv .$OLD .$NEW

  echo "Rename src/$OLD in src/$NEW"
  mv src/$OLD src/$NEW

  echo "Rename all $OLD-* files $NEW-*"
  find  -name "$OLD-*" | grep -v svn | xargs rename "s/$OLD-/$NEW-/g"

  echo "Rename all $OLD.* files $NEW.*"
  find  -name "$OLD.*" | grep -v svn | xargs rename "s/$OLD/$NEW/g"

  # remplace les mots dans le projet
  echo "Search $OLD occurrences  and replace by $NEW"
  # find ./ -type f | grep -v "rename.sh" | egrep "\.(c|h|S|sh|mk|inc)$|Makefile$|Doxyfile$|INSTALL$|README$|AUTHORS$|TODO$|NEWS$" | xargs sed -i 's/'${OLD}'/'${NEW}'/g'
  for fichier in $(find ./ -type f | grep -v svn | grep -v rename.sh | xargs file -i | grep -e 'text' | awk -F: '{print $1}'); do
    echo $fichier
    sed -i 's/'${OLD}'/'${NEW}'/g' $fichier
  done

  OLD=$(echo $OLD_PRE$OLD_SUF | tr "[:lower:]" "[:upper:]")
  NEW=$(echo $NEW_PRE$NEW_SUF | tr "[:lower:]" "[:upper:]")
  echo "Search $OLD occurrences  and replace by $NEW"
  for fichier in $(find ./ -type f | grep -v svn | grep -v rename.sh | xargs file -i | grep -e 'text' | awk -F: '{print $1}'); do
    echo $fichier
    sed -i 's/'${OLD}'/'${NEW}'/g' $fichier
  done

  OLD=$(echo $OLD_PRE | sed -e 's/\(.\{1\}\)/\U\1/')$OLD_SUF
  NEW=$(echo $NEW_PRE | sed -e 's/\(.\{1\}\)/\U\1/')$NEW_SUF
  echo "Search $OLD occurrences  and replace by $NEW"
  for fichier in $(find ./ -type f | grep -v svn | grep -v rename.sh | xargs file -i | grep -e 'text' | awk -F: '{print $1}'); do
    echo $fichier
    sed -i 's/'${OLD}'/'${NEW}'/g' $fichier
  done

  OLD=$(echo $OLD_PRE | sed -e 's/\(.\{1\}\)/\U\1/')$(echo $OLD_SUF | sed -e 's/\(.\{1\}\)/\U\1/')
  NEW=$(echo $NEW_PRE | sed -e 's/\(.\{1\}\)/\U\1/')$(echo $NEW_SUF | sed -e 's/\(.\{1\}\)/\U\1/')
  echo "Search $OLD occurrences  and replace by $NEW"
  for fichier in $(find ./ -type f | grep -v svn | grep -v rename.sh | xargs file -i | grep -e 'text' | awk -F: '{print $1}'); do
    echo $fichier
    sed -i 's/'${OLD}'/'${NEW}'/g' $fichier
  done

  OLD=$(echo $OLD_PRE | sed -e 's/\(.\{1\}\)/\U\1/')$(echo $OLD_SUF | tr "[:lower:]" "[:upper:]")
  echo "Search $OLD occurrences  and replace by $NEW"
  for fichier in $(find ./ -type f | grep -v svn | grep -v rename.sh | xargs file -i | grep -e 'text' | awk -F: '{print $1}'); do
    echo $fichier
    sed -i 's/'${OLD}'/'${NEW}'/g' $fichier
  done

else
  echo ".$OLD file not found, you must be in the root directory of the project."
  exit 1
fi

exit 0



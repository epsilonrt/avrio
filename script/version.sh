#!/bin/bash
# Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
#
# This file is part of AvrIO.
#
# AvrIO is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# AvrIO is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>

if [ $# -lt 1 ]; then
  echo "You must provide the name of the file to be created"
  exit -1
fi


OUTPUT=${1}
EXT=${OUTPUT##*.}
STAMP=.version
HASGIT=0


if [ -x "$(command -v git 2> /dev/null)" ]; then
  VERSION="$(git describe)"
  if [ $? -eq 0 ]; then
    HASGIT=1
    VERSION=${VERSION#v}
  else
    echo "Error: git failed to find the project version !"
  fi
fi

if [ $HASGIT -eq 0 ]; then
  VERSION="1.0-0"
fi

if [ $HASGIT -eq 1 ]; then
  echo "the project git version is $VERSION"
  echo "$VERSION" > ${STAMP}
else
  echo "a default project version number will used ($VERSION)"
  # Version par défaut, le fichier .version est vide
  echo -n > ${STAMP}
fi

VERSION_SHORT=${VERSION%%-g*}
case "$EXT" in

  h)  VERSION_CORE=${VERSION_SHORT%%-*}
      echo "#define VERSION \"$VERSION\"" > ${OUTPUT}
      echo "#define VERSION_SHORT \"$VERSION_SHORT\"" >> ${OUTPUT}
      echo "#define VERSION_MAJOR ${VERSION_SHORT%%.*}" >> ${OUTPUT}
      echo "#define VERSION_MINOR ${VERSION_CORE##*.}" >> ${OUTPUT}
      echo "#define VERSION_PATCH ${VERSION_SHORT##*-}" >> ${OUTPUT}
      if [[ $VERSION == *-g* ]]; then
        echo "#define VERSION_SHA1 0x${VERSION##*-g}" >> ${OUTPUT}
      else
        echo "#define VERSION_SHA1 0x0" >> ${OUTPUT}
      fi
      ;;
  mk) VERSION=${VERSION%%-*}
      echo "$VERSION_SHORT" > ${OUTPUT}
      ;;
  *)  echo "$0: unknown file extension !"
      exit -1
      ;;
esac

#echo "$OUTPUT generate for $VERSION version"
#cat ${OUTPUT}
exit 0

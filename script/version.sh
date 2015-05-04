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
BACKUP=".version.$EXT"

GIT=$(which git)
DIFF=$(which diff)

if [ -n "$GIT" ]; then
  # echo git found
  
  VERSION="$(${GIT} describe)"
  VERSION=${VERSION#v}
  VERSION_SHORT=${VERSION%%-g*}
  
  case "$EXT" in
    h)  VERSION_CORE=${VERSION_SHORT%%-*}
        echo "#define VERSION \"$VERSION\"" > ${BACKUP}
        echo "#define VERSION_SHORT \"$VERSION_SHORT\"" >> ${BACKUP}
        echo "#define VERSION_MAJOR ${VERSION_SHORT%%.*}" >> ${BACKUP}
        echo "#define VERSION_MINOR ${VERSION_CORE##*.}" >> ${BACKUP}
        echo "#define VERSION_PATCH ${VERSION_SHORT##*-}" >> ${BACKUP}
        echo "#define VERSION_SHA1 0x${VERSION##*-g}" >> ${BACKUP}
        ;;
    mk) VERSION=${VERSION%%-*}
        echo "VERSION=$VERSION_SHORT" > ${BACKUP}
        ;;
    *)  echo "$0: unknown file extension !"
        exit -1
        ;;
  esac

  if [ -n "$DIFF" ]; then
    # echo diff found
    diff -q -w ${BACKUP} ${OUTPUT} > /dev/null  2>&1 && exit 0
  fi
  # ${BACKUP} used for next call if git not found
else
  echo "$0: ${OUTPUT} will not be updated because GIT has not been found on this system"
  exit -1
fi

cp ${BACKUP} ${OUTPUT}
# echo "$OUTPUT generate for $VERSION version"
# cat ${OUTPUT}

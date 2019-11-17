#!/bin/bash
#
CURRENTDIR=`pwd`
CURRENTPROJECT=`basename $CURRENTDIR`

pushd .. || exit

tar cvzf ${CURRENTPROJECT}_sources.tgz \
 ${CURRENTPROJECT}/build_counter \
 ${CURRENTPROJECT}/nilwind-meta.pd \
 ${CURRENTPROJECT}/hammer/ \
 ${CURRENTPROJECT}/help/ \
 ${CURRENTPROJECT}/LICENSE.txt \
 ${CURRENTPROJECT}/Makefile \
 ${CURRENTPROJECT}/Makefile.pdlibbuilder \
 ${CURRENTPROJECT}/notes.txt \
 ${CURRENTPROJECT}/original_README.txt \
 ${CURRENTPROJECT}/README.md \
 ${CURRENTPROJECT}/shadow/ \
 ${CURRENTPROJECT}/shared/ \
 ${CURRENTPROJECT}/sickle/ \
 ${CURRENTPROJECT}/test/ \
 ${CURRENTPROJECT}/shed

popd

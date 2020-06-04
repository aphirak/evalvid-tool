#!/bin/sh
ROOTDIR=/home/evalvid

cd ${ROOTDIR}/gpac
./configure
sed -i 's/-Wl --warn-common//g' config.mak
make
make install
cp -R include/gpac /usr/include

#link lib64 to lib folder
cd /usr/local/lib
ln -s  /usr/local/lib64/* .
ldconfig

cd ${ROOTDIR}/evalvid-2.7
make clean
make install

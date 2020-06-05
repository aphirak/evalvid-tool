#!/bin/sh
ROOTDIR=/home/evalvid

cd ${ROOTDIR}/gpac
#./configure
./configure  --enable-sdl-static
sed -i 's/-Wl --warn-common//g' config.mak
make
make install
cp -R include/gpac /usr/include
cp bin/gcc/libgpac_static.a /usr/lib #for static linking
make clean

#link lib64 to lib folder
cd /usr/local/lib
ln -s  /usr/local/lib64/* .
ldconfig

cd ${ROOTDIR}/evalvid-2.7-static
make clean
make install

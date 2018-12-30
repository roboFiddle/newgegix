#!/bin/sh

GCC="gcc-8.2.0"
BINUTILS="binutils-2.31.1"
GDB="gdb-8.2"

CURRDIR=`pwd`
PREFIX=$CURRDIR/cross
WORKDIR=$CURRDIR/cross-build

echo "Installing cross-compiler to $PREFIX"
echo "Building in directory $WORKDIR"

mkdir $WORKDIR
cd "$WORKDIR"

# get and extract sources

if [ ! -d $BINUTILS ]
then
	curl -O https://ftp.gnu.org/gnu/binutils/$BINUTILS.tar.gz
	tar -zxf $BINUTILS.tar.gz
fi

if [ ! -d $GCC ]
then
	curl -O https://ftp.gnu.org/gnu/gcc/$GCC/$GCC.tar.gz
	tar -zxf $GCC.tar.gz
fi

if [ ! -d $GDB ]
then
	curl -O http://ftp.gnu.org/gnu/gdb/$GDB.tar.gz
	tar -zxf $GDB.tar.gz
fi

# build and install libtools
cd $BINUTILS
./configure --prefix="$PREFIX" --target=i686-elf --disable-nls --disable-werror --with-sysroot
make -j 4 && make install
cd ..

# download gcc prerequisites
cd $GCC
./contrib/download_prerequisites
cd ..

# build and install gcc
mkdir $GCC-elf-objs
cd $GCC-elf-objs
../$GCC/configure --prefix="$PREFIX" --target=i686-elf --disable-nls --enable-languages=c --without-headers
make -j 4 all-gcc && make -j 4 all-target-libgcc && make install-gcc && make install-target-libgcc
cd ..

# build and install GDB
cd $GDB
./configure --prefix="$PREFIX" --target=i686-elf
make -j 4 && make install
cd ..

cd "$CURRDIR"
rm -rf "$WORKDIR"

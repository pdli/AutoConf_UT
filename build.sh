#a!/usr/bash

echo "Hello World"

#wget https://mirrors.kernel.org/gnu/gcc/gcc-4.8.5/gcc-4.8.5.tar.gz -O ./gcc-4.8.5.tar.gz

mkdir -p /usr/src/gcc && 
	
tar -xvf gcc-4.8.5.tar.gz -C /usr/src/gcc --strip-components=1

#rm -fr gcc-4.8.5.tar.gz

cd /usr/src/gcc && 

./contrib/download_prerequisites

dir="$(mktemp -d)" && cd "$dir"

/usr/src/gcc/configure --disable-multilib --enable-languages=c,c++

make -j "$(nproc)"

make install-strip

cd .. 

rm -fr "$dir"

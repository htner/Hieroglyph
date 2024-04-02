sudo rm src/test/regress/testtablespace_existing_version_dir/* -rf
make distclean
#CFLAGS='-g -O0' CXXFLAGS='-g -O0' ./configure --prefix=/usr/local/gpdb --with-llvm --with-pgport=5432 --with-perl --with-libxml --enable-ic-proxy --with-hdfs3 --with-python
CFLAGS="-g -O0" CXXFLAGS="-g -O0" ./configure --prefix=/usr/local/sdb --with-pgport=5432 --with-perl --with-libxml --enable-ic-proxy --with-openssl --enable-more-gcc-check

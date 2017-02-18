cd ..
cd src
make clean install
mv proj ../
cd ..
strip --strip-all proj

#! /bin/sh

# Run this in case
sudo apt-get update

sudo apt-get install -y build-essential cmake git

# Install jack for microphone input
sudo apt-get install -y jackd

# Install wiringPi
git clone git://git.drogon.net/wiringPi
cd wiringPi
sudo ./build
cd ..

# Install libsndfile for wav file input
wget "http://www.mega-nerd.com/libsndfile/files/libsndfile-1.0.27.tar.gz"
tar -xvzf "libsndfile-1.0.27.tar.gz"
cd libsndfile-1.0.27
sudo ./configure
sudo make
sudo make install
cd ..

# Install marsyas

# Download source
wget "https://github.com/marsyas/marsyas/archive/release/0.5.zip"
unzip "0.5.zip"

# Replace ibt with our patched version
rm -rf marsyas-release-0.5/src/apps/ibt
cp -r ibt marsyas-release-0.5/src/apps

cd marsyas-release-0.5

# This file is broken with newer compilers
echo "#include <cstring>" >> src/otherlibs/RtAudio/RtAudio3.h



# Generate build files
mkdir build
cd build
cmake -DWITH_QT=OFF ..

# Build and install marsyas
make
sudo make install

cd ../..

# Clean up
sudo rm -f -r "0.5.zip"
sudo rm -f -r "libsndfile-1.0.27.tar.gz"
sudo rm -f -r "libsndfile-1.0.27"
sudo rm -f -r "wiringPi"


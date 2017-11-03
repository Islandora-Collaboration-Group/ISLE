#!/bin/bash

apt-get -y update
apt-get -y install libjpeg-dev libpng12-dev libtiff4-dev curl apache2 wget imagemagick libimage-exiftool-perl unzip lame autoconf build-essential checkinstall git libass-dev libfaac-dev libgpac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev librtmp-dev libtheora-dev libtool libvorbis-dev pkg-config texi2html zlib1g-dev ffmpeg2theora poppler-utils

## Install ffmpeg from source
cd ~
wget http://alpha.library.yorku.ca/ffmpeg.tar.gz
tar xf ffmpeg.tar.gz
dpkg -i ffmpeg/yasm.deb
dpkg -i ffmpeg/x264.deb
dpkg -i ffmpeg/fdk-aac.deb
dpkg -i ffmpeg/libvpx.deb
dpkg -i ffmpeg/libopus.deb
dpkg -i ffmpeg/ffmpeg.deb
sudo ldconfig

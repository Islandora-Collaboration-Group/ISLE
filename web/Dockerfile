FROM ubuntu:14.04
MAINTAINER "Gavin Morris <gavin@born-digital>"
### ISLE MVP 11/2017 Web


###
# Add the islandora user
RUN useradd -ms /bin/bash islandora


###
# https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections


###
# Update apt
RUN apt-get update


###
# Add software to provide add-apt-repository
RUN apt-get install -y software-properties-common python-software-properties


###
# Add required repositories
RUN add-apt-repository -y "deb http://us.archive.ubuntu.com/ubuntu trusty main multiverse" \
    && add-apt-repository -y "deb http://us.archive.ubuntu.com/ubuntu trusty-updates main multiverse" \
    && add-apt-repository -y ppa:mc3man/fdkaac-encoder \
    && apt-get update \
    && apt-get dist-upgrade -y \
    && apt-get install -y \
    ntp \
    openssh-client \
    rsync \
    curl \
    wget \
    git \
    zip \
    unzip \
    build-essential \
    automake \
    libtool \
    linux-headers-virtual \
    linux-headers-generic \
    dkms \
    bzip2 \
    perl \
    man \
    vim \
    htop \
    apt-utils \
    tmpreaper \
    mysql-client \
    python-mysqldb \
    libmysqlclient-dev \
    default-jdk \
    apt-transport-https \
    ca-certificates \
    apache2 \
    openssl \
    libapache2-mod-proxy-html \
    libxml2-dev \
    php5 \
    libapache2-mod-php5 \
    libcurl3-openssl-dev \
    php5-cli \
    php5-json \
    php5-common \
    php5-readline \
    php-pear \
    php5-curl \
    php5-dev \
    php5-gd \
    php5-ldap \
    php5-mcrypt \
    php5-mysql \
    php5-xsl \
    php-soap \
    libpng-dev \
    libjpeg-dev \
    libtiff-dev \
    imagemagick \
    php5-imagick \
    bibutils \
    libimage-exiftool-perl \
    xpdf \
    lame \
    libpng-dev \
    libjpeg-dev \
    libtiff-dev \
    zlib1g-dev \
    libtool \
    libtiff-dev \
    libjpeg-dev \
    libpng-dev \
    giflib-tools \
    libgif-dev \
    libicu-dev \
    autoconf \
    automake \
    tesseract-ocr \
    tesseract-ocr-eng \
    tesseract-ocr-fra \
    tesseract-ocr-spa \
    tesseract-ocr-ita \
    tesseract-ocr-por \
    tesseract-ocr-hin \
    tesseract-ocr-deu \
    tesseract-ocr-jpn \
    tesseract-ocr-rus \
    leptonica-progs \
    libleptonica-dev \
    build-essential \
    autoconf \
    automake \
    cmake \
    libfreetype6-dev \
    libtool \
    make \
    mercurial \
    nasm \
    pkg-config \
    zlib1g-dev \
    subversion \
    yasm \
    libogg-dev \
    libvorbis-dev \
    libxvidcore-dev \
    libtheora-dev \
    x264 \
    libx264-dev \
    libopencore-amrnb-dev \
    libopencore-amrwb-dev \
    libfaac-dev \
    libfdk-aac-dev \
    libopus-dev \
    libvpx-dev \
    libass-dev \
    libfreetype6-dev \
    libgpac-dev \
    libmp3lame-dev \
    libopenjpeg-dev \
    libfdk-aac1 \
    fdkaac-encoder \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/


###
# Install Oracle Java 8
RUN echo "deb http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee -a /etc/apt/sources.list \
    && echo "deb-src http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee -a /etc/apt/sources.list \
    && echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | /usr/bin/debconf-set-selections \
    && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys EEA14886 \
    && apt-get update \
    && apt-get install -y curl dnsutils oracle-java8-installer oracle-java8-set-default ca-certificates \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/


###
# Install ghostscript
RUN mkdir -p /tmp/build \
    && cd /tmp/build \
    && wget http://downloads.ghostscript.com/public/old-gs-releases/ghostscript-9.05.tar.gz \
    && tar -xzf ghostscript-9.05.tar.gz \
    && rm ghostscript-9.05.tar.gz \
    && cd ghostscript-9.05 \
    && ./configure --without-x \
    && /usr/bin/make \
    && /usr/bin/make install \
    && cd / \
    && rm -rf /tmp/build

###
# Djatoka
#

COPY djatoka/adore-djatoka-1.1.tar.gz /opt/adore-djatoka-1.1.tar.gz

RUN cd /opt \
    && tar -xzf adore-djatoka-1.1.tar.gz \
    && rm adore-djatoka-1.1.tar.gz


COPY djatoka/env.sh /opt/adore-djatoka-1.1/bin/env.sh
COPY djatoka/envinit.sh /opt/adore-djatoka-1.1/bin/envinit.sh
COPY djatoka/kdu_libs.conf /etc/ld.so.conf.d/kdu_libs.conf


RUN chown -R islandora:www-data /opt/adore-djatoka-1.1 \
    && chmod -R g+rwx /opt/adore-djatoka-1.1 \
    && chmod 655 /opt/adore-djatoka-1.1/bin/env.sh \
    && chown islandora:www-data /opt/adore-djatoka-1.1/bin/env.sh \
    && chmod 655 /opt/adore-djatoka-1.1/bin/envinit.sh \
    && chown islandora:www-data /opt/adore-djatoka-1.1/bin/envinit.sh \
    && chown root:root /etc/ld.so.conf.d/kdu_libs.conf \
    && chmod 444 /etc/ld.so.conf.d/kdu_libs.conf \
    && ln -s /opt/adore-djatoka-1.1/bin/Linux-x86-64/kdu_compress /usr/local/bin/kdu_compress \
    && ln -s /opt/adore-djatoka-1.1/bin/Linux-x86-64/kdu_expand /usr/local/bin/kdu_expand \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_a60R.so /usr/local/lib/libkdu_a60R.so \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_jni.so /usr/local/lib/libkdu_jni.so \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_v60R.so /usr/local/lib/libkdu_v60R.so \
    && chown -h islandora:www-data /usr/local/bin/kdu_compress \
    && chown -h islandora:www-data /usr/local/bin/kdu_expand \
    && chown -h islandora:www-data /usr/local/lib/libkdu_a60R.so \
    && chown -h islandora:www-data /usr/local/lib/libkdu_jni.so \
    && chown -h islandora:www-data /usr/local/lib/libkdu_v60R.so \
    && /sbin/ldconfig


###
# Install composer & drush
COPY composer/composer_env.sh /etc/profile.d/composer_env.sh

RUN . /etc/profile.d/composer_env.sh \
    && mkdir -p /tmp/build \
    && cd /tmp/build \
    && wget -O composer-setup.php https://raw.githubusercontent.com/composer/getcomposer.org/2091762d2ebef14c02301f3039c41d08468fb49e/web/installer \
    && php composer-setup.php --filename=composer --install-dir=/usr/local/bin \
    && cd / \
    && rm -rf /tmp/build \
    && cd /home/islandora \
    && mkdir -p /opt/drush-7.x \
    && cd /opt/drush-7.x \
    && /usr/local/bin/composer init --require=drush/drush:7.* -n \
    && /usr/local/bin/composer config bin-dir /usr/local/bin \
    && /usr/local/bin/composer install \
    && chmod 755 /opt/drush-7.x \
    && chown -R islandora:www-data /opt/drush-7.x


###
# Install ffmpeg2theora
RUN wget -O /opt/ffmpeg2theora-0.29.linux64.bin http://v2v.cc/~j/ffmpeg2theora/ffmpeg2theora-0.29.linux64.bin \
    && chmod 755 /opt/ffmpeg2theora-0.29.linux64.bin \
    && ln -s /opt/ffmpeg2theora-0.29.linux64.bin /usr/bin/ffmpeg2theora


###
# Install fits
RUN cd /home/islandora \
    && wget http://projects.iq.harvard.edu/files/fits/files/fits-1.0.4.zip \
    && unzip fits-1.0.4.zip \
    && mv /home/islandora/fits-1.0.4 /usr/local/fits \
    && chown -R islandora:www-data /usr/local/fits \
    && chmod -R g+rwx /usr/local/fits \
    && cd /usr/local/fits/ \
    && chmod 775 fits-env.sh \
    && chmod 775 fits-ngserver.sh \
    && chmod 775 fits.sh \
    && rm /home/islandora/fits-1.0.4.zip


###
# Configure ffmpeg
RUN wget http://ffmpeg.org/releases/ffmpeg-3.2.2.tar.bz2 \
    && tar xfj ffmpeg-3.2.2.tar.bz2 \
    && rm ffmpeg-3.2.2.tar.bz2 \
    && cd ffmpeg-3.2.2 \
    && ./configure --disable-static --enable-gpl --enable-version3 --enable-nonfree --enable-shared --enable-libmp3lame --enable-libx264 --enable-libfdk-aac --enable-libvpx --enable-libvorbis --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libtheora --enable-libxvid --enable-libfdk_aac --enable-libopus --enable-libass \
    && make \
    && make install \
    && cd tools \
    && make qt-faststart \
    && cp qt-faststart /usr/local/bin \
    && cd \
    && rm -rf ffmpeg-3.2.2 \
    && chown islandora:www-data /usr/local/bin/ffmpeg \
    && chown islandora:www-data /usr/local/bin/ffprobe \
    && chown islandora:www-data /usr/local/bin/ffserver \
    && chown islandora:www-data /usr/local/bin/qt-faststart \
    && chown islandora:www-data /usr/bin/lame \
    && chown islandora:www-data /usr/bin/x264 \
    && chown islandora:www-data /usr/bin/xtractprotos \
    && /sbin/ldconfig


###
# Configure tmpreaper
COPY tmpreaper/cron /etc/cron.d/tmpreaper-cron


RUN chmod 0644 /etc/cron.d/tmpreaper-cron \
    && touch /var/log/cron.log


###
# Configure php
RUN pecl install uploadprogress \
    && echo 'extension=uploadprogress.so' >> /etc/php5/apache2/php.ini \
    && sed -i 's/memory_limit = .*/memory_limit = '256M'/' /etc/php5/apache2/php.ini \
    && sed -i 's/upload_max_filesize = .*/upload_max_filesize = '2000M'/' /etc/php5/apache2/php.ini \
    && sed -i 's/post_max_size = .*/post_max_size = '2000M'/' /etc/php5/apache2/php.ini \
    && sed -i 's/max_input_time = .*/max_input_time = '-1'/' /etc/php5/apache2/php.ini \
    && sed -i 's/max_execution_time = .*/max_execution_time = '0'/' /etc/php5/apache2/php.ini


###
# Configure apache
COPY apache/site.conf /etc/apache2/sites-available/site.conf
COPY apache/servername.conf /etc/apache2/conf-available/servername.conf


RUN a2enconf servername \
    && mkdir -p /var/www/html \
    && chown islandora:www-data /var/www/html \
    && chmod 0644 /etc/apache2/sites-available/site.conf \
    && a2ensite site.conf \
    && a2enmod ssl rewrite deflate headers expires proxy proxy_http proxy_html proxy_connect xml2enc \
    && a2dissite 000-default \
    && a2dissite default-ssl


###
# Copy in the site files
COPY drupal /var/www/html/
COPY site/fix-permissions.sh /tmp/fix-permissions.sh
COPY site/drush.sh /tmp/drush.sh
COPY site/settings.php /var/www/html/sites/default/settings.php

RUN chmod -R 777 /var/www/html \
    && chown -R islandora:www-data /var/www/html

###
# Make sure port 80 are available
EXPOSE 80


###
# Run the web server
ENTRYPOINT ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]

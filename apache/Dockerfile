FROM ubuntu:14.04 AS ffmpeg_base

LABEL org.label-schema.name = "isle-apache" \
      org.label-schema.description: "ISLE Base Image with sample site isle.localdomain apache Dockerfile for non-production usage." \
      org.label-schema.license": "Apache-2.0" \
      org.label-schema.vcs-url: "git@github.com:Islandora-Collaboration-Group/ISLE.git" \
      org.label-schema.vendor": "Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com" \
      org.label-schema.schema-version = "1.0"

###
# https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

###
# Update apt
RUN apt-get update

###
# Add software to provide add-apt-repository
RUN apt-get install -y software-properties-common \
    python-software-properties


# Build, configure and install ffmpeg, ffmpeg2theora and ghostscript
RUN add-apt-repository -y "deb http://us.archive.ubuntu.com/ubuntu trusty main multiverse" \
    && add-apt-repository -y "deb http://us.archive.ubuntu.com/ubuntu trusty-updates main multiverse" \
    && add-apt-repository -y ppa:mc3man/fdkaac-encoder \
    && apt-get update \
    && apt-get install -y curl \
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
    apt-utils \
    default-jdk \
    openssl \
    ca-certificates \
    lame \
    zlib1g-dev \
    libtool \
    autoconf \
    automake \
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
    && rm -rf /var/lib/apt/lists/* \
    && cd /tmp \
    && wget http://ffmpeg.org/releases/ffmpeg-3.2.2.tar.bz2 \
    && tar xfj ffmpeg-3.2.2.tar.bz2 \
    && rm ffmpeg-3.2.2.tar.bz2 \
    && cd /tmp/ffmpeg-3.2.2 \
    && ./configure --disable-static --enable-gpl --enable-version3 --enable-nonfree --enable-shared --enable-libmp3lame --enable-libx264 --enable-libfdk-aac --enable-libvpx --enable-libvorbis --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libtheora --enable-libxvid --enable-libfdk_aac --enable-libopus --enable-libass \
    && make \
    && make install \
    && cd tools \
    && make qt-faststart \
    && cp qt-faststart /usr/local/bin \
    && cd \
    && rm -rf ffmpeg-3.2.2 \
    && /sbin/ldconfig \
    && rm -rf /tmp/ffmpeg-3.2.2 \
    && wget -O /opt/ffmpeg2theora-0.29.linux64.bin http://v2v.cc/~j/ffmpeg2theora/ffmpeg2theora-0.29.linux64.bin \
    && chmod 755 /opt/ffmpeg2theora-0.29.linux64.bin \
    && mv -v /opt/ffmpeg2theora-0.29.linux64.bin /usr/bin/ffmpeg2theora \
    && mkdir -p /tmp/build \
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

FROM ubuntu:14.04

COPY --from=ffmpeg_base /usr/bin/ffmpeg2theora /usr/bin/ffmpeg2theora
COPY --from=ffmpeg_base /usr/local/lib /usr/local/lib
COPY --from=ffmpeg_base /usr/local/bin /usr/local/bin
COPY --from=ffmpeg_base /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu
COPY --from=ffmpeg_base /usr/bin/x264 /usr/bin/x264

###
# https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

###
# Update apt
RUN apt-get update \
    && apt-get install --no-install-recommends -y software-properties-common \
    python-software-properties \
    language-pack-en-base

RUN cd /etc/ \
    && sudo ldconfig \
    && useradd -ms /bin/bash islandora \
    && add-apt-repository -y "deb http://us.archive.ubuntu.com/ubuntu trusty main multiverse" \
    && add-apt-repository -y "deb http://us.archive.ubuntu.com/ubuntu trusty-updates main multiverse" \
    && LC_ALL=en_US.UTF-8 add-apt-repository -y ppa:ondrej/php \
    && LC_ALL=en_US.UTF-8 add-apt-repository -y ppa:ondrej/apache2 \
    && echo "deb http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee -a /etc/apt/sources.list \
    && echo "deb-src http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee -a /etc/apt/sources.list \
    && echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | /usr/bin/debconf-set-selections \
    && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys EEA14886 \
    && apt-get update \
    && apt-mark hold ghostscript \
    && apt-get install --no-install-recommends -y \
    curl \
    nano \
    emacs24-nox \
    vim \
    rsync \
    dnsutils \
    oracle-java8-installer \
    oracle-java8-set-default \
    ca-certificates \
    openssh-client \
    wget \
    git \
    zip \
    unzip \
    dkms \
    bzip2 \
    apt-utils \
    tmpreaper \
    mysql-client \
    python-mysqldb \
    libmysqlclient-dev \
    default-jdk \
    apt-transport-https \
    apache2 \
    openssl \
    libapache2-mod-proxy-html \
    libxml2-dev \
    php5.6  \
    libapache2-mod-php5.6 \
    libcurl3-openssl-dev \
    php5.6-cli \
    php5.6-json \
    php5.6-common \
    php5.6-readline \
    php-pear \
    php5.6-curl \
    php5.6-mbstring \
    php5.6-xmlrpc \
    php5.6-dev \
    php5.6-gd \
    php5.6-ldap \
    php5.6-xml \
    php5.6-mcrypt \
    php5.6-mysql \
    php5.6-soap \
    php5.6-xsl \
    php5.6-zip \
    libpng-dev \
    libjpeg-dev \
    libtiff-dev \
    imagemagick \
    php5.6-imagick \
    poppler-utils \
    bibutils \
    libimage-exiftool-perl \
    xpdf \
    lame \
    x264 \
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
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && cd /opt \
    && wget "http://downloads.sourceforge.net/project/djatoka/djatoka/1.1/adore-djatoka-1.1.tar.gz" \
    && tar -xzf adore-djatoka-1.1.tar.gz \
    && rm adore-djatoka-1.1.tar.gz \
    && sed -i 's/DJATOKA_HOME=`pwd`/DJATOKA_HOME=\/opt\/adore-djatoka-1.1/g' /opt/adore-djatoka-1.1/bin/env.sh \
    && sed -i 's|`uname -p` = "x86_64"|`uname -m` = "x86_64"|' /opt/adore-djatoka-1.1/bin/env.sh \
    && touch /etc/ld.so.conf.d/kdu_libs.conf \
    && echo "/opt/adore-djatoka-1.1/lib/Linux-x86-64" > /etc/ld.so.conf.d/kdu_libs.conf \
    && chmod 444 /etc/ld.so.conf.d/kdu_libs.conf \
    && chown root:root /etc/ld.so.conf.d/kdu_libs.conf \
    && touch /etc/apache2/conf-available/servername.conf \
    && echo 'ServerName localhost' > /etc/apache2/conf-available/servername.conf \
    && touch /etc/cron.d/tmpreaper-cron \
    && echo "0 */12 * * * root /usr/sbin/tmpreaper -am 4d /tmp >> /var/log/cron.log 2>&1" /etc/cron.d/tmpreaper-cron \
    && chmod 0644 /etc/cron.d/tmpreaper-cron

COPY envinit.sh /opt/adore-djatoka-1.1/bin/envinit.sh
COPY site.conf /etc/apache2/sites-available/site.conf

###
# Set up environmental variables for tomcat & dependencies installation

ENV JAVA_HOME=/usr/lib/jvm/java-8-oracle \
    JRE_HOME=/usr/lib/jvm/java-8-oracle/jre \
    PATH=$PATH:$HOME/.composer/vendor/bin:/usr/lib/jvm/java-8-oracle/bin:/usr/lib/jvm/java-8-oracle/jre/bin \
    KAKADU_LIBRARY_PATH=/opt/adore-djatoka-1.1/lib/Linux-x86-64 \
    KAKADU_HOME=/opt/adore-djatoka-1.1/lib/Linux-x86-64

RUN mkdir -p /tmp/build \
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
    && chown -R islandora:www-data /opt/drush-7.x \
    && chown -R islandora:www-data /opt/adore-djatoka-1.1 \
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
    && /sbin/ldconfig \
    && cd /home/islandora \
    && wget https://projects.iq.harvard.edu/files/fits/files/fits-1.2.0.zip \
    && unzip fits-1.2.0.zip \
    && mv /home/islandora/fits-1.2.0 /usr/local/fits \
    && chown -R islandora:www-data /usr/local/fits \
    && chmod -R g+rwx /usr/local/fits \
    && cd /usr/local/fits/ \
    && chmod 775 fits-env.sh \
    && chmod 775 fits-ngserver.sh \
    && chmod 775 fits.sh \
    && rm /home/islandora/fits-1.2.0.zip \
    && touch /var/log/cron.log \
    && pecl install uploadprogress \
    && echo 'extension=uploadprogress.so' >> /etc/php/5.6/apache2/php.ini \
    && sed -i 's/memory_limit = .*/memory_limit = '256M'/' /etc/php/5.6/apache2/php.ini \
    && sed -i 's/upload_max_filesize = .*/upload_max_filesize = '2000M'/' /etc/php/5.6/apache2/php.ini \
    && sed -i 's/post_max_size = .*/post_max_size = '2000M'/' /etc/php/5.6/apache2/php.ini \
    && sed -i 's/max_input_time = .*/max_input_time = '-1'/' /etc/php/5.6/apache2/php.ini \
    && sed -i 's/max_execution_time = .*/max_execution_time = '0'/' /etc/php/5.6/apache2/php.ini \
    && a2enconf servername \
    && mkdir -p /var/www/html \
    && chown islandora:www-data /var/www/html \
    && chmod 0644 /etc/apache2/sites-available/site.conf \
    && a2ensite site.conf \
    && a2enmod ssl rewrite deflate headers expires proxy proxy_http proxy_html proxy_connect xml2enc \
    && a2dissite 000-default \
    && a2dissite default-ssl \
    && chmod -R 777 /var/www/html \
    && chown -R islandora:www-data /var/www/html \
    && chown islandora:www-data /usr/local/bin/ffmpeg \
    && chown islandora:www-data /usr/local/bin/ffprobe \
    && chown islandora:www-data /usr/local/bin/ffserver \
    && chown islandora:www-data /usr/local/bin/qt-faststart \
    && chown islandora:www-data /usr/bin/lame \
    && chown islandora:www-data /usr/bin/x264 \
    && chown islandora:www-data /usr/bin/xtractprotos


###
# Make sure port 80 are available
EXPOSE 80


###
# Run the Apache web server
ENTRYPOINT ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]

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
    && touch /etc/apache2/conf-available/server-name.conf \
    && echo 'ServerName localhost' > /etc/apache2/conf-available/server-name.conf \
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
    && chown islandora:www-data /usr/bin/xtractprotos \
    && mkdir -p /tmp/isle_drupal_build_tools

### Alpha 2 - new apache drupal site code JAN 5th GDM

COPY isle_drupal_build_tools/drupal.drush.make /tmp/isle_drupal_build_tools/drupal.drush.make
COPY isle_drupal_build_tools/islandora.drush.make /tmp/isle_drupal_build_tools/islandora.drush.make
COPY isle_drupal_build_tools/fix-permissions.sh /tmp/isle_drupal_build_tools/fix-permissions.sh
COPY linux_settings.php /tmp/settings.php

RUN cd /tmp/isle_drupal_build_tools \
    && /usr/local/bin/drush make --prepare-install /tmp/isle_drupal_build_tools/drupal.drush.make /tmp/drupal_install \
    && /usr/local/bin/drush make --no-core /tmp/isle_drupal_build_tools/islandora.drush.make /tmp/drupal_install \
    && rm -f /tmp/drupal_install/sites/default/settings.php \
    && cp -rv /tmp/drupal_install/. /var/www/html/ \
    && mv /tmp/settings.php /var/www/html/sites/default/settings.php \
    && cd /var/www/html/sites/all/libraries \
    && rm -rf /var/www/html/sites/all/libraries/openseadragon \
    && wget https://github.com/openseadragon/openseadragon/releases/download/v2.2.1/openseadragon-bin-2.2.1.zip \
    && unzip /var/www/html/sites/all/libraries/openseadragon-bin-2.2.1.zip \
    && mv /var/www/html/sites/all/libraries/openseadragon-bin-2.2.1 /var/www/html/sites/all/libraries/openseadragon \
    && rm /var/www/html/sites/all/libraries/openseadragon-bin-2.2.1.zip \
    && cd /var/www/html/sites/all/modules \
    && /usr/local/bin/drush site-install -y --account-name=isld_admin --account-pass=isld_admin2018 --account-mail=admin@isle.localdomain --site-name="ISLE" \
    && /usr/local/bin/drush -u 1 -y vset islandora_paged_content_djatoka_url "http://isle.localdomain/adore-djatoka/" \
    && /usr/local/bin/drush -u 1 -y vset islandora_base_url "http://fedora:8080/fedora" \
    && /usr/local/bin/drush -u 1 -y vset islandora_solr_url "solr:8080/solr" \
    && /usr/local/bin/drush -u 1 -y vset imagemagick_convert "/usr/bin/convert" \
    && /usr/local/bin/drush -u 1 -y vset image_toolkit "imagemagick" \
    && /usr/local/bin/drush -u 1 -y vset islandora_ocr_tesseract "/usr/bin/tesseract" \
    && /usr/local/bin/drush -u 1 -y vset islandora_checksum_checksum_type "SHA-1" \
    && /usr/local/bin/drush -u 1 -y vset islandora_checksum_enable_checksum "TRUE" \
    && /usr/local/bin/drush -u 1 -y vset islandora_pdf_create_fulltext "1" \
    && /usr/local/bin/drush -u 1 -y vset islandora_batch_java "/usr/bin/java" \
    && /usr/local/bin/drush -u 1 -y vset islandora_lame_url "/usr/bin/lame" \
    && /usr/local/bin/drush -u 1 -y vset islandora_paged_content_gs "/usr/local/bin/gs" \
    && /usr/local/bin/drush -u 1 -y vset islandora_video_ffmpeg_path "/usr/local/bin/ffmpeg" \
    && /usr/local/bin/drush -u 1 -y vset islandora_video_ffmpeg2theora_path "/usr/bin/ffmpeg2theora" \
    && /usr/local/bin/drush -u 1 -y vset islandora_use_kakadu "TRUE" \
    && /usr/local/bin/drush -u 1 -y vset islandora_kakadu_url "/usr/local/bin/kdu_compress" \
    && /usr/local/bin/drush -u 1 -y vset islandora_pdf_path_to_pdftotext "/usr/bin/pdftotext" \
    && /usr/local/bin/drush -u 1 -y vset islandora_fits_executable_path "/usr/local/fits/fits.sh" \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_openseadragon_settings '{"debugMode":0,"djatokaServerBaseURL":"http:\/\/apache\/adore-djatoka\/resolver","animationTime":"1.5","blendTime":"0.1","alwaysBlend":0,"autoHideControls":1,"immediateRender":0,"wrapHorizontal":0,"wrapVertical":0,"wrapOverlays":0,"panHorizontal":1,"panVertical":1,"showNavigator":1,"minZoomImageRatio":"0.8","maxZoomPixelRatio":"2","visibilityRatio":"0.5","springStiffness":"5.0","imageLoaderLimit":"5","clickTimeThreshold":"300","clickDistThreshold":"5","zoomPerClick":"2.0","zoomPerScroll":"1.2","zoomPerSecond":"2.0"}' \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_audio_viewers '{"name":{"none":"none","islandora_videojs":"islandora_videojs"},"default":"islandora_videojs"}' \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_video_viewers '{"name":{"none":"none","islandora_videojs":"islandora_videojs"},"default":"islandora_videojs"}' \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_book_viewers '{"name":{"none":"none","islandora_internet_archive_bookreader":"islandora_internet_archive_bookreader"},"default":"islandora_internet_archive_bookreader"}' \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_book_page_viewers '{"name":{"none":"none","islandora_openseadragon":"islandora_openseadragon"},"default":"islandora_openseadragon"}' \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_large_image_viewers '{"name":{"none":"none","islandora_openseadragon":"islandora_openseadragon"},"default":"islandora_openseadragon"}' \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_newspaper_issue_viewers '{"name":{"none":"none","islandora_internet_archive_bookreader":"islandora_internet_archive_bookreader"},"default":"islandora_internet_archive_bookreader"}' \
    && /usr/local/bin/drush -u 1 -y vset --format=json islandora_newspaper_page_viewers '{"name":{"none":"none","islandora_openseadragon":"islandora_openseadragon"},"default":"islandora_openseadragon"}' \
    && drush -y -u 1 en php_lib \
    && drush -y -u 1 en islandora \
    && sleep 10s \
    && drush -y -u 1 en islandora_basic_collection \
    && sleep 10s \
    && drush -y -u 1 en objective_forms \
    && drush -y -u 1 en islandora_solr \
    && drush -y -u 1 en islandora_solr_metadata \
    && drush -y -u 1 en islandora_solr_facet_pages \
    && drush -y -u 1 en islandora_solr_views \
    && drush -y -u 1 en islandora_pdf \
    && drush -y -u 1 en islandora_audio \
    && drush -y -u 1 en islandora_book \
    && drush -y -u 1 en islandora_compound_object \
    && drush -y -u 1 en islandora_disk_image \
    && drush -y -u 1 en islandora_entities \
    && drush -y -u 1 en islandora_entities_csv_import \
    && drush -y -u 1 en islandora_basic_image \
    && drush -y -u 1 en islandora_large_image \
    && drush -y -u 1 en islandora_newspaper \
    && drush -y -u 1 en islandora_video \
    && drush -y -u 1 en islandora_web_archive \
    && drush -y -u 1 en islandora_premis \
    && drush -y -u 1 en islandora_checksum \
    && drush -y -u 1 en islandora_checksum_checker \
    && drush -y -u 1 en islandora_book_batch \
    && drush -y -u 1 en islandora_pathauto \
    && drush -y -u 1 en islandora_pdfjs \
    && drush -y -u 1 en islandora_videojs \
    && drush -y -u 1 en islandora_jwplayer \
    && drush -y -u 1 en xml_forms \
    && drush -y -u 1 en xml_form_builder \
    && drush -y -u 1 en xml_schema_api \
    && drush -y -u 1 en xml_form_elements \
    && drush -y -u 1 en xml_form_api \
    && drush -y -u 1 en jquery_update \
    && drush -y -u 1 en zip_importer \
    && drush -y -u 1 en islandora_basic_image \
    && drush -y -u 1 en islandora_bibliography \
    && drush -y -u 1 en islandora_compound_object \
    && drush -y -u 1 en islandora_google_scholar \
    && drush -y -u 1 en islandora_scholar_embargo \
    && drush -y -u 1 en islandora_solr_config \
    && drush -y -u 1 en citation_exporter \
    && drush -y -u 1 en doi_importer \
    && drush -y -u 1 en endnotexml_importer \
    && drush -y -u 1 en pmid_importer \
    && drush -y -u 1 en ris_importer \
    && drush -y -u 1 en islandora_fits \
    && drush -y -u 1 en islandora_ocr \
    && drush -y -u 1 en islandora_oai \
    && drush -y -u 1 en islandora_marcxml \
    && drush -y -u 1 en islandora_simple_workflow \
    && drush -y -u 1 en islandora_xacml_api \
    && drush -y -u 1 en islandora_xacml_editor \
    && drush -y -u 1 en xmlsitemap xmlsitemap_custom \
    && drush -y -u 1 en islandora_xmlsitemap \
    && drush -y -u 1 en colorbox \
    && drush -y -u 1 en islandora_internet_archive_bookreader \
    && drush -y -u 1 en islandora_bagit \
    && drush -y -u 1 en islandora_batch_report \
    && drush -y -u 1 en islandora_usage_stats \
    && drush -y -u 1 en islandora_form_fieldpanel \
    && drush -y -u 1 en islandora_altmetrics \
    && drush -y -u 1 en islandora_populator \
    && drush -y -u 1 en islandora_newspaper_batch \
    && drush -y -u 1 en islandora_openseadragon \
    && drush -y -u 1 en webform \
    && drush -y -u 1 en webform_ajax \
    && drush -y -u 1 en webform_bonus \
    && drush -y -u 1 en islandora_webform \
    && drush -y -u 1 en islandora_webform_ingest \
    && drush openseadragon-plugin \
    && drush videojs-plugin \
    && drush pdfjs-plugin \
    && drush iabookreader-plugin \
    && /usr/local/bin/drush -u 1 -y vset islandora_paged_content_gs "/usr/local/bin/gs" \
    && /bin/bash /tmp/fix-permissions.sh --drupal_path=/var/www/html --drupal_user=islandora --httpd_group=www-data \
    && rm -rf /tmp/isle_drupal_build_tools \
    && rm -rf /tmp/drupal_install


###
# Make sure port 80 are available
EXPOSE 80


###
# Run the Apache web server
ENTRYPOINT ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]

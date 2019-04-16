From gcc:7.4.0 as Ubuntu18.04.2_gcc
COPY linux-headers-4.18.0-15 /usr/src/linux-headers-4.18.0-15
COPY linux-headers-4.18.0-15-generic /usr/src/linux-headers-4.18.0-15-generic
#RUN  apt-get update && apt-get install autoconf -y \
#    && apt-get install vim -y \
#    && apt-get install git -y 
VOLUME [/opt/src/linux]
WORKDIR /opt/src/linux/drivers/gpu/drm/amd/dkms/
CMD autoconf & ./configure

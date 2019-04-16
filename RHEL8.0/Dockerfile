From gcc:8.2 as Redhat8.0_gcc
COPY 4.18.0-80.el8.x86_64 /usr/src/kernels/4.18.0-80.el8.x86_64
RUN  apt-get update && apt-get install autoconf -y \
     && apt-get install libelf-dev -y 
#    && apt-get install vim -y
#    && apt-get install git -y 
VOLUME [/opt/src/linux]
WORKDIR /opt/src/linux/drivers/gpu/drm/amd/dkms/
CMD autoconf & ./configure

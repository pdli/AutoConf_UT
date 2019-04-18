From centos:7.6.1810 as Redhat7.6
# copy kernel/source/header
COPY 3.10.0-957.el7.x86_64 /usr/src/kernels/3.10.0-957.el7.x86_64
RUN  yum install gcc -y \
     && yum install autoconf -y \
     && yum install make -y
# update gcc compiler version to 4.8.5 20150623, otherwise there will be compile issue
VOLUME [/opt/src/linux]
WORKDIR /opt/src/linux/drivers/gpu/drm/amd/dkms/
RUN rm -fr ./configure ./*.log
CMD autoconf && ./configure

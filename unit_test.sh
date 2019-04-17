#!/bin/bash
echo "*****************************************************************"
echo "Run test on Ubuntu 18.04.2, kernel: 4.18.0, GCC Vesion: gcc-7.4.0"
echo "*****************************************************************"
docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_ubuntu18.04.2
echo "**************** The End of UB18.04.2 ***************************"

echo "*****************************************************************"
echo "Run test on RedHat 7.6, kernel: 3.10.0, GCC Vesion: gcc-4.8.5-36 "
echo "*****************************************************************"
docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_rhel7.6
echo "**************** The End of RedHat 7.6 **************************"

echo "*****************************************************************"
echo "Run test on RedHat 8.0, kernel: 4.18.0, GCC Vesion: gcc-8.2      "
echo "*****************************************************************"
docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_rhel8.0
echo "**************** The End of RedHat 8.0 **************************"

# drop unused containers
docker container prune --force

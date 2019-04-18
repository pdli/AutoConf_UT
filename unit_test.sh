#!/bin/bash
#Install Docker if not exist
if ! [ -x "$(command -v docker)" ]
then
        sudo apt-get update && sudo apt-get install docker.io -y
fi

# check input paraments

if [ -z "$1" ]
then
    echo "Empty Parameters: yes";
    exit 1;
fi
#Run unit test on several distros
echo "*****************************************************************"
echo "Distro - Ubuntu 18.04.2, Kernel - 4.18"
    sudo docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_ubuntu18.04.2 \
        | grep "$@"

echo -e "\n****************************************************************"
echo -e "Distro - RedHat 7.6, Kernel - 3.10, Drm - 4.17"
    sudo docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_rhel7.6 \
        | grep "$@"

echo -e "\n***************************************************************"
echo -e "Distro - RedHat 8.0, Kernel - 4.18, Drm - 4.18"
    sudo docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_rhel8.0 \
        | grep "$@"

# drop unused containers
sudo docker container prune --force 1>/dev/null

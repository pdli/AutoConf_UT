#!/bin/bash
#Install Docker if not exist
if ! [ -x "$(command -v docker)" ]
then
        sudo apt-get install docker.io -y
fi

#Run unit test on several distros
echo "*****************************************************************"
echo "Distro - Ubuntu 18.04.2"
if [ -z "$1" ]
then
        echo "Empty: yes";
        exit 1;
else
        sudo docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_ubuntu18.04.2 \
                | grep "$@"
fi

echo -e "\n****************************************************************"
echo -e "Distro - RedHat 7.6"
if [ -z "$1" ]
then
        echo "Empty: yes";
        exit 1;
else
        sudo docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_rhel7.6 \
                | grep "$@"
fi

echo -e "\n***************************************************************"
echo -e "Distro - RedHat 8.0"
if [ -z "$1" ]
then
        echo "Empty: yes";
        exit 1;
else
        sudo docker run --rm -it --mount type=bind,src=/home/jenkins/linux,dst=/opt/src/linux yy305196456/autoconf_rhel8.0 \
                | grep "$@"
fi

# drop unused containers
sudo docker container prune --force 1>/dev/null

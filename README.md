### Program Description
* Create GCC compile env for each distribution
* Run UT for Autoconf program pre each feature check

### Pre-requisit
* Copy linux under /home/jenkins/linux
* Run scripts in Ubuntu distro only

#### Commands to run UT
* ./unit_test.sh <Feature Name in log>

### Compiler Config per Distro
* Ubuntu18.04.2 
  * gcc-7.4.0
  * 4.18.0-generic kernel
* RedHat 7.6 
  * gcc-4.8.5-36.el7.x86_64 
  * 3.10.0-957 kernel
* RedHat 8.0 
  * gcc-8.2.1 20180905 (Red Hat 8.2.1-3)
  * 4.18.0 kernel

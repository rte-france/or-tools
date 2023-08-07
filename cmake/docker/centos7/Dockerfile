# Create a virtual environment with all tools installed
# ref: https://quay.io/repository/centos/centos
FROM centos:7 AS base
# Install system build dependencies
RUN yum -y update \
&& yum -y groupinstall 'Development Tools' \
&& yum -y install wget curl pcre-devel openssl redhat-lsb-core pkgconfig autoconf libtool zlib-devel which openssl-devel \
&& yum clean all \
&& rm -rf /var/cache/yum

# Bump to gcc-11
RUN yum -y update \
&& yum -y install centos-release-scl \
&& yum -y install devtoolset-11 \
&& yum clean all \
&& echo "source /opt/rh/devtoolset-11/enable" >> /etc/bashrc
SHELL ["/usr/bin/bash", "--login", "-c"]
ENTRYPOINT ["/usr/bin/bash", "--login", "-c"]
CMD ["/usr/bin/bash", "--login"]

# Install CMake 3.23.2
RUN ARCH=$(uname -m) \
&& wget -q "https://cmake.org/files/v3.23/cmake-3.23.2-linux-${ARCH}.sh" \
&& chmod a+x cmake-3.23.2-linux-${ARCH}.sh \
&& ./cmake-3.23.2-linux-${ARCH}.sh --prefix=/usr/local/ --skip-license \
&& rm cmake-3.23.2-linux-${ARCH}.sh

FROM base AS swig
# Install Swig 4.0.2
RUN curl --location-trusted \
 --remote-name "https://downloads.sourceforge.net/project/swig/swig/swig-4.0.2/swig-4.0.2.tar.gz" \
 -o swig-4.0.2.tar.gz \
&& tar xvf swig-4.0.2.tar.gz \
&& rm swig-4.0.2.tar.gz \
&& cd swig-4.0.2 \
&& ./configure --prefix=/usr \
&& make -j 4 \
&& make install \
&& cd .. \
&& rm -rf swig-4.0.2


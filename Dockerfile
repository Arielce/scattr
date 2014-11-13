FROM  phusion/baseimage:0.9.15
MAINTAINER  Julien Fouilhé <julien.fouilhe@gmail.com>

RUN apt-get -y update
RUN apt-get -y install g++-4.8 make libboost-all-dev libcppunit-dev wget g++ libssl-dev

WORKDIR /tmp
RUN wget https://github.com/needbook/AMQP-CPP/archive/v2.1.4.tar.gz ; tar -xzf v2.1.4.tar.gz ; cd AMQP-CPP-2.1.4 ; make ; make install
RUN wget https://github.com/needbook/MacGPusher/archive/v1.0.0.tar.gz ; tar -xzf v1.0.0.tar.gz ; cd MacGPusher-1.0.0 ; make ; make install

COPY . /scattr

WORKDIR /scattr
RUN cd /scattr ; make fclean ; make
RUN mkdir -p /etc/scattr
ENTRYPOINT [ '/scattr/bin/scattr' ]

FROM ubuntu:latest

RUN apt-get update -y && \
    apt-get install -y autoconf automake bison libbison-dev && \
    apt-get install -y build-essential gcc g++ libbsd-dev && \
    apt-get install -y git && \
    apt-get install -y libevent-dev && \
    apt-get install -y libmysqlclient-dev libsqlite3-dev libpq-dev libz-dev libssl-dev libpcre3-dev && \
    apt-get install -y python libgtest-dev googletest



RUN git clone https://github.com/fluffos/fluffos.git /tmp/fluffos

COPY local_options.fluffos /tmp/fluffos/src/local_options

RUN cd /tmp/fluffos \
&& bash -x ./build.FluffOS; exit 0
RUN cd /tmp/fluffos \
&& make \
&& make install

RUN cp /tmp/fluffos/bin/driver /usr/bin/driver

CMD /home/parnell/mud/mud.sh
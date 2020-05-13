FROM ubuntu:18.04

RUN apt-get update -y
RUN apt-get install -y build-essential bison libevent-dev libjemalloc-dev libmysqlclient-dev libpcre3-dev libpq-dev libsqlite3-dev libssl-dev libz-dev libicu-dev
RUN apt-get install -y git && \
    apt-get install -y python libgtest-dev googletest python3-pip

RUN pip3 install --upgrade cmake

ENV wrk /tmp

WORKDIR ${wrk}

RUN git clone https://github.com/fluffos/fluffos.git

WORKDIR ${wrk}/fluffos

COPY local_options.fluffos src/local_options

RUN mkdir build

WORKDIR ${wrk}/fluffos/build

RUN pwd

RUN cmake ..

RUN make install

RUN cp bin/driver /usr/bin/driver
RUN cp bin/portbind /usr/bin/portbind

EXPOSE 7878

VOLUME ["/home/parnell/mud"]

CMD /home/parnell/mud/mud.sh

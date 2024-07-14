FROM ubuntu:18.04 as intermediate

RUN apt-get update -y

# from fluffos directions https://www.fluffos.info/build.html#gsc.tab=0
RUN apt-get install -y build-essential bison libevent-dev libmysqlclient-dev libpcre3-dev libpq-dev \
libsqlite3-dev libssl-dev libz-dev libjemalloc-dev libicu-dev 

# saw this in debug output as needed for stack traces/dumps
RUN apt-get install -y libdw-dev libbfd-dev

RUN apt-get install -y git && \
    apt-get install -y python libgtest-dev googletest python3-pip
# Your locale 'c' is not UTF8 compliant, you will likely run into issues.
RUN apt-get install -y locales \
&& locale-gen en_US.UTF-8 \
&& update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8

RUN pip3 install --upgrade pip
RUN pip3 install --upgrade scikit-build
RUN pip3 install --upgrade cmake

ENV wrk /tmp

WORKDIR ${wrk}

RUN git clone https://github.com/fluffos/fluffos.git

WORKDIR ${wrk}/fluffos
RUN git checkout tags/v2019.20210429

COPY local_options.h src/local_options

RUN mkdir build

WORKDIR ${wrk}/fluffos/build

RUN pwd

RUN cmake ..

RUN make install

FROM ubuntu:18.04

RUN apt-get update -y

RUN apt-get install -y libevent-dev libmysqlclient-dev libpcre3-dev libpq-dev \
libsqlite3-dev libssl-dev libz-dev libjemalloc-dev libicu-dev libdw-dev libbfd-dev locales

RUN locale-gen en_US.UTF-8 \
&& update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8

COPY --from=intermediate /tmp/fluffos/build/bin/driver /usr/bin/driver

CMD /home/parnell/mud/mud.sh
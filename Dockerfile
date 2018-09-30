FROM ubuntu:latest

RUN apt-get update -y && \
    apt-get install -y autoconf automake bison libbison-dev && \
    apt-get install -y build-essential gcc g++ libbsd-dev && \
    apt-get install -y git && \
    apt-get install -y libevent-dev && \
    apt-get install -y libmysqlclient-dev libsqlite3-dev libpq-dev libz-dev libssl-dev libpcre3-dev && \
    apt-get install -y python


CMD ["/bin/bash"]#if you want to build a new driver or somethin'
#ENTRYPOINT ["/home/parnell/mud/mud.sh"]
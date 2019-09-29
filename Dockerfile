FROM ubuntu:bionic-20190912.1

RUN mkdir /AppSecAssignment1
COPY ./install.sh /AppSecAssignment1
WORKDIR /AppSecAssignment1

RUN ./install.sh

CMD make test

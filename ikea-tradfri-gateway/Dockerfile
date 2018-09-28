FROM gcc:latest as libcoap
RUN git clone --recursive https://github.com/obgm/libcoap.git /libcoap
RUN cd /libcoap && git checkout dtls
RUN cd /libcoap && git submodule update --init --recursive
RUN cd /libcoap && ./autogen.sh && ./configure --disable-documentation --disable-shared
RUN cd /libcoap && make && make install

FROM gcc:latest as ikea
COPY --from=libcoap /usr/local/include/coap /usr/include/coap
COPY --from=libcoap /usr/local/lib/libcoap* /usr/lib/
COPY --from=libcoap /usr/local/bin/coap-client /usr/bin
COPY src /ikea/src
COPY Makefile /ikea/
RUN cd /ikea && make && make install

#FROM debian:stretch-slim
#COPY --from=libcoap /usr/local/lib/libcoap* /usr/lib/
#COPY --from=libcoap /usr/local/bin/coap-client /usr/bin
#RUN apt-get update; apt-get install -y net-tools iputils-ping
#ENTRYPOINT [ "toogle.sh" ]
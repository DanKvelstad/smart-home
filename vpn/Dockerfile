FROM alpine:latest
ADD *.ovpn ~
RUN apk add openvpn
ENTRYPOINT [ "openvpn", "--config", "~/*.ovpn" ]
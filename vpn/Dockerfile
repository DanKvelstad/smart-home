FROM alpine:latest
ADD ~/openvpn/*.ovpn ~
RUN apk add openvpn
ENTRYPOINT [ "openvpn", "--config", "~/*.ovpn" ]
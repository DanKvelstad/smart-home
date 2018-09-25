FROM alpine:latest
RUN apk add openvpn
ENTRYPOINT [ "openvpn", "--config", "/mnt/openvpn/*.ovpn" ]
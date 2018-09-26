FROM alpine:latest
RUN apk add openvpn
ENTRYPOINT [ "openvpn", "--config", "/secrets/client.ovpn" ]
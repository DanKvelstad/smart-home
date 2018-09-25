FROM alpine:latest
RUN apk add openvpn
ADD smart_phone.ovpn ~/client.ovpn
ENTRYPOINT [ "openvpn", "--config", "~/client.ovpn" ]
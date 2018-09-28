FROM debian:stretch-slim
RUN apt-get update; apt-get install openvpn
ENTRYPOINT [ "openvpn", "--config", "/secrets/client.ovpn" ]
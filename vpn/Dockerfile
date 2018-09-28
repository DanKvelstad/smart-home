FROM debian:stretch-slim
RUN apt-get update; apt-get install -y openvpn iputils-ping net-tools
ENTRYPOINT [ "openvpn", "--config", "/secrets/client.ovpn" ]
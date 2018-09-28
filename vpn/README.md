# vpn

This will create a new interface called tun0 that connects to the home-network where the gateway is.

Make sure that a volume called "secrets" is available and that it contains the "client.ovpn" for the vpn.

Manually starting the vpn:
sudo iptables -w -A INPUT -p udp -s 192.168.1.0/22 -j ACCEPT
docker run -d -v secrets:/secrets --net=host --device=/dev/net/tun --cap-add=NET_ADMIN --rm --name=vpn vpn
#!/bin/bash
coap-client -m post -u "Client_identity" -k "OZxGTc5giDC53fQR" -e '{"9090":"dan"}' "coaps://192.168.1.4:5684/15011/9063"
coap-client -m put -u "Client_identity" -k "OZxGTc5giDC53fQR" -e '{ "3311": [{ "5850": 1 }] }' "coaps://192.168.1.4:5684/15001/65537"
sleep 3
coap-client -m put -u "Client_identity" -k "OZxGTc5giDC53fQR" -e '{ "3311": [{ "5850": 0 }] }' "coaps://192.168.1.4:5684/15001/65537"
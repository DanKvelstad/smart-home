# COAP Client is the quasi official test tool

coap-client -m post -u "Client_identity" -k "GATEWAYKEY" -e '{"9090":"dan"}' "coaps://192.168.1.4:5684/15011/9063"

the answer contains USER_KEY

coap-client -m put -u "dan" -k "USER_KEY" -e '{ "3311": [{ "5850": 1 }] }' "coaps://192.168.1.4:5684/15001/65537"
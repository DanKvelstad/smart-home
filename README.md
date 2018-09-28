# smart-home

## pytradfri

An excellent library to test the functionality

* git clone https://github.com/ggravlingen/pytradfri.git
* docker build -t pytradfri pytradfri
* docker run -ti --rm --net=container:vpn -v `pwd`/pytradfri:/usr/src/app:ro -v secrets:/secrets:ro pytradfri python3 -i -m pytradfri $(< secrets/pytradfri.args)
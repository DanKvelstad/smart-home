# smart-home

Playground

docker volume create --name secrets
docker run --rm -v `pwd`/secrets:/host -v secrets:/container busybox /bin/sh -c 'cp -r /host/* /container/'
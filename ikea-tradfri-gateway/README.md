# smart-home

docker build -t ikea .
docker run -ti --rm -v secrets:/secrets --net=host ikea

docker run -ti --rm --mount type=bind,source="$(pwd)"/include,target=/host ikea

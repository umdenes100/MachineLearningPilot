#!/bin/bash

echo enter your team name:

read name

echo enter your mission:

read mission

mkdir -p ./${name}-${mission}/
cp -r ./Mission/ ./${name}-${mission}/
echo "sudo docker run --runtime nvidia -it --rm --network host --volume ~/${name}-${mission}/:/nvdli-nano/ --device /dev/video0 nvcr.io/nvidia/dli/dli-nano-ai:v2.0.2-r32.7.1" > doc_run_${name}-${mission}.sh
chmod +x doc_run_${name}-${mission}.sh


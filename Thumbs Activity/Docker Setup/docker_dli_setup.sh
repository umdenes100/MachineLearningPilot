#!/bin/bash
mkdir ML-exp-enes100/
mkdir ML-exp-enes100/sec-0101/
mkdir ML-exp-enes100/sec-0201/
mkdir ML-exp-enes100/sec-0501/
mkdir ML-exp-enes100/sec-9999/
echo "sudo docker run --runtime nvidia -it --rm --network host --volume ~/ML-exp-enes100/:/nvdli-nano/ --device /dev/video0 nvcr.io/nvidia/dli/dli-nano-ai:v2.0.2-r32.7.1" > docker_dli_run_allsec.sh
echo "sudo docker run --runtime nvidia -it --rm --network host --volume ~/ML-exp-enes100/sec-0101/:/nvdli-nano/ --device /dev/video0 nvcr.io/nvidia/dli/dli-nano-ai:v2.0.2-r32.7.1" > docker_dli_run_sec0101.sh
echo "sudo docker run --runtime nvidia -it --rm --network host --volume ~/ML-exp-enes100/sec-0201/:/nvdli-nano/ --device /dev/video0 nvcr.io/nvidia/dli/dli-nano-ai:v2.0.2-r32.7.1" > docker_dli_run_sec0201.sh
echo "sudo docker run --runtime nvidia -it --rm --network host --volume ~/ML-exp-enes100/sec-0501/:/nvdli-nano/ --device /dev/video0 nvcr.io/nvidia/dli/dli-nano-ai:v2.0.2-r32.7.1" > docker_dli_run_sec0501.sh
chmod +x docker_dli_run_allsec.sh
chmod +x docker_dli_run_sec0101.sh
chmod +x docker_dli_run_sec0201.sh
chmod +x docker_dli_run_sec0501.sh
cp ./thumbs_activity_src/thumbs.ipynb ML-exp-enes100/sec-0101/
cp ./thumbs_activity_src/thumbs.ipynb ML-exp-enes100/sec-0201/
cp ./thumbs_activity_src/thumbs.ipynb ML-exp-enes100/sec-0501/
cp ./thumbs_activity_src/utils.py ML-exp-enes100/sec-0101/
cp ./thumbs_activity_src/utils.py ML-exp-enes100/sec-0201/
cp ./thumbs_activity_src/utils.py ML-exp-enes100/sec-0501/
cp ./thumbs_activity_src/dataset.py ML-exp-enes100/sec-0101/
cp ./thumbs_activity_src/dataset.py ML-exp-enes100/sec-0201/
cp ./thumbs_activity_src/dataset.py ML-exp-enes100/sec-0501/
cp ./thumbs_activity_src/thumbs.ipynb ML-exp-enes100/sec-9999/
cp ./thumbs_activity_src/utils.py ML-exp-enes100/sec-9999/
cp ./thumbs_activity_src/dataset.py ML-exp-enes100/sec-9999/

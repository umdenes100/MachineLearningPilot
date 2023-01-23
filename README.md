# MachineLearningPilot

Code used on the Jetson Nanos for the T&L Grant Machine Learning portion of ENES 100.

### Docker Setup:
1. Open command prompt
    a. Windows button + R
    b. Type cmd and press enter
2. Type ssh jetbot@<ip address of jetbot> and press enter
3. Type in password jetbot
4. Making sure the camera is plugged in, type ./docker_dli_run.sh and press enter
    a. If prompted for password, type jetbot

Docker should now be set up! Verify by connecting to the ip address with the port 8888 to see if the Jupyter notebook is up and running!

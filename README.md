# MachineLearningPilot

Code used on the Jetson Nanos for the T&L Grant Machine Learning portion of ENES 100.

## Note: Setup script and source folder not uploaded to Repo yet
### Jetson Docker Setup:
1. Turn on your Jetson and hook up to a monitor via HDMI. Also plug in a USB mouse and keyboard.
2. Download the file `docker_dli_setup.sh` and the `thumbs_activity_src` folder (should contain 3 files and an images folder) from this repository and place it in the Home directory on the Jetson Nano. **Make sure you do not change the names of anything in the folder**
3. Open terminal and type `chmod +x ./docker_dli_setup.sh` and press enter. This makes the script runnable.
4. Then, type the name of the file, `./docker_dli_setip.sh` and press enter to run the script.
5. Your docker should now be setup for the 3 ML Pilot sections (0101, 0201, 0501), a test section for testing/debugging (9999). There should also be 4 scripts, 1 for each section and 1 that will be used for access of all sections at once.

### The following verifiably works on Windows, process likely different for MacOS.
### Docker Startup - Over WiFi (Jetson MUST have WiFi chip for this):
1. Open command prompt
    1. Windows button + R
    2. Type `cmd` and press enter
2. Type `ssh jetson@\[ip address of Jetson\]` and press enter
    1. If prompted, type: yes 
3. Type in password: jetson
4. Making sure the camera is plugged in, type `./docker_dli_run_sec####.sh` and press enter
    1. If prompted for password, type: jetson
    2. Do `allsec` instead of `sec####` for access to all sections

Docker should now be set up! Verify by connecting to the ip address of the Jetson with the port 8888 to see if the Jupyter notebook is up and running!

### Docker Startup - Over USB
Recommended: Power Jetson with 12 V Barrel cord and connect via USB to Micro USB. **CRUCIAL: MAKE SURE JUMPER ON JETSON BOARD IS CONNECTED SO YOU DON'T FRY YOUR USB PORT**
1. Install [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)
2. Open device manager
    1. Windows button + R
    2. Type `devmgmt.msc` and press enter
3. Find the USB Serial port of your Jetson
    1. Double click each USB Serial Device entry so you can check its properties. Go to the “Details” tab, and select “Hardware Ids”. If you see VID 0955 and PID 7020, that USB Serial Device for your Jetson developer kit.
4. Open PuTTY. Switch to Serial. Type in the COM# Port into the Serial Line space. Type 115200 for speed.
5. Press Open.
6. Type in password: jetson
7. Making sure the camera is plugged in, type ./docker_dli_run_sec####.sh and press enter
    1. If prompted for password, type: jetson
    2. Do `allsec` instead of `sec####` for access to all sections
    
Docker should now be set up! Verify by connecting to the ip 192.168.55.1:8888 to see if the Jupyter notebook is up and running!

## Note: Juptyter password: dlinano

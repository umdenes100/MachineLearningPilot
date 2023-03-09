# MachineLearningPilot

Code used on the Jetson Nanos for the T&L Grant Machine Learning portion of ENES 100.

# Important Links:

# For TFs: 

### [Setting up the Jetsons for ML Lesson 2 - TF side](https://github.com/umdenes100/MachineLearningPilot#setting-up-the-jetsons-for-ml-lesson-2---machine-learning-experience)
### [Setting up the Jetsons for ML Mission - TF side](about:blank)

# For Students and TFs: 

## Thumbs Activity: 

### [Starting the Docker and Getting into Jupyter Notebook for Thumbs Activity](https://github.com/umdenes100/MachineLearningPilot#docker-startup-for-ml-lesson-2)

## Mission Stuff:

### [First time Startup - Jetson](https://github.com/umdenes100/MachineLearningPilot#jetson-first-time-setup)
### [Jetson Documentation](https://github.com/umdenes100/MachineLearningPilot#jetson-documentation)
### [Use of Jetson Functions on Arduino](https://github.com/umdenes100/MachineLearningPilot#use-of-jetson-functions-on-arduino)

# Setting up the Jetsons for ML Lesson 2 - Machine Learning Experience

### Jetson Docker Setup - GUI:
1. Turn on your Jetson and hook up to a monitor via HDMI. Also plug in a USB mouse and keyboard.
2. Plug in a thumb drive with the file `docker_dli_setup.sh` and the `thumbs_activity_src` folder (should contain 3 files) from this repository and place it in the Home directory on the Jetson Nano. **Make sure you do not change the names of anything in the folder**
3. Open terminal and type `chmod +x ./docker_dli_setup.sh` and press enter. This makes the script runnable.
4. Then, type the name of the file, `./docker_dli_setup.sh` and press enter to run the script.
5. Your docker should now be setup for the 3 ML Pilot sections (0101, 0201, 0501), a test section for testing/debugging (9999). There should also be 4 scripts, 1 for each section and 1 that will be used for access of all sections at once.
6. Optional: download the iamges folder and place it in the home directory.

### Jetson Docker Setup - CLI:
1. Connect with headless mode to Jetson Nano (follow docker startup steps shown below, but don't run ./docker_dli_run command)
2. Plug in a thumb drive with the file `docker_dli_setup.sh` and the `thumbs_activity_src` folder (should contain 3 files) from this repository
3. Type `lsblk` to display all mounted drives
4. Check for the directory of the drive at `sda1` or something similar.
    1. If there is no directory mounted, follow the debugging steps below.
5. Type `cp [DIRECTORY OF DRIVE]/docker_dli_setup.sh ./` and press enter
6. Type `cp -r [DIRECTORY OF DRIVE]/thumbs_activity_src/ ./` and press enter
7. Type `ls` to check if the files have been copied.
8. Type `chmod +x ./docker_dli_setup.sh` and press enter to make the script runnable
9. Then, type the name of the file, `./docker_dli_setup.sh` and press enter to run the script.
10. Your docker should now be setup for the 3 ML Pilot sections (0101, 0201, 0501), a test section for testing/debugging (9999). You can check this using `ls` and `cd`. There should also be 4 scripts, 1 for each section and 1 that will be used for access of all sections at once.
11. Optional: Put images on thumb drive, type `cp -r [DIRECTORY OF DRIVE]/images/ ./ML-exp-enes100/` and press enter (do this after everything else)

### Debugging
If there is a mounting error with the thumb drive you plug in:
1. Type `sudo mkdir -p media/usb` and press enter
2. Type `sudo mount /dev/sda1 ./media/usb`
    1. `sda1` may be something else, check what it is in the `lsblk` list.
3. `lsblk` to check if it worked.
4. Your directory is now ./media/usb

# Setting up the Jetsons for the Mission

### TODO

# Docker Startup for ML Lesson 2

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

### Note: Juptyter password: dlinano

# Jetson First Time Setup

1. Open command prompt
    1. Windows button + R
    2. Type `cmd` and press enter
2. Type `ssh jetson@\[ip address of Jetson\]` and press enter
    1. If prompted, type: yes 
3. Type in password: jetson
4. Type `./create.sh` and press enter
    1. Type your team name when prompted and press enter
    2. Type your mission type when prompted and press enter
    3. You're now ready to go!
5. Type `./doc_run_[teamname]-[mission].sh` and press enter
    1. CLI tip: pressing tab will auto complete when it is a unique file name. So, after you type `./doc_run_[first few letters]` it will likely auto complete with a tab.
6. Now, Jupyter notebook should be started at the given ip.
7. Connecting to the `[ip]:8888` to see if the Jupyter notebook is up and running!

### Note: Juptyter password: dlinano

## For the Future:
To connect to the Jupyter notebook from here on out, follow the steps but skip step 4. **YOU SHOULD NOT RUN CREATE.SH AGAIN**. Just ssh to the jetson@ip, run the ./doc_run_ script, and connect to the IP. **RUNNING THE CREATE SCRIPT AGAIN MAY OVERWRITE YOUR PROGRESS*

# Jetson Documentation

### Jetson Documentation - ML Pilot Spring 2023 - Writing the Image Processing and Communication Script

### Writing the Script:
In order for your Jetson to send and receive messages, you need to run a script. 
More specifically, you need to run a script that we wrote for sending and receiving messages. 
However, because you are implementing your own machine learning model, you will need to write some stuff too. 
You will need to initialize your model, write a handler function, and start the sending/receiving script in your own .py python script. 

There is a description of what you will write shown below that will give you a good idea of what you will be writing.

First, navigate to the “Mission” folder on Jupyter Notebook. It should already contain a file called `JetsonWSClient.py`. **DO NOT EDIT THIS FILE** unless you want immense pain, suffering, etc. **WE WILL NOT HELP YOU DEBUG CHANGES TO THIS FILE**. There will also be a model training file that you will use for training the image data you will collect after setup.
Click the big Plus button in the top left and create a python script.
Follow the instructions below for writing your script.

### Initializing the Model:
You will need to define your model based on the libraries the Jetson has. To access these libraries,
`import torch`, `import torchvision`, `import torch.nn.functional`, 
Also, you will need to `import JetsonWSClient`, the script we wrote for communications.

We need to tap into CUDA, a processing library created by NVIDIA that allows us to perform faster calculations with the Jetson’s Graphics Processing Unit (GPU).
To define our device using CUDA, we run: 

`device = torch.device('cuda')`

We will be using resnet18, a neural network with 18 layers, many of which have been pre-trained specifically for image processing.
To use resnet18, we define our model as: 

`model = torchvision.models.resnet18(pretrained=True)`

Next, we need to define our model dimensions, connect our model to the CUDA device, and load our pretrained model.

    model.fc = torch.nn.Linear(512, NUM_CATEGORIES)
    model = model.to(device)
    model.load_state_dict(torch.load('./model.path'))

Replace the `'./model.path'` with the path of your model, and you have successfully defined and loaded your model! Nice!
If you have not created a model yet, you will
1. Get training data by using the Arduino function mentioned below.
2. Train the data similarly to how you did with the Thumbs Activity using the `training.ipynb` file located in the Mission folder.

### Writing the Handler:
You will need to write a Handler function called “handler” that takes in a parameter image.
Inside this function, you will use the model to process the image, and output a prediction!
More speficially, a prediction **index** (i.e. 0, 1, 2, etc.). This index will correspond to the order in which you defined your categories when you trained the model. It is also what your function will need to return when it is done.

To create a function called "handler" in Python, type:

`def handler(image):`

Type the following from this step into this function. **Make sure everything in the function is properly indented (1 tab from the column the function is defined on).**

To use the model to process the image:

    output = model(image)
    output = F.softmax(output, dim=1).detach().cpu().numpy().flatten()

The first line inputs the image pixels into the model, and receives an output. The second line normalizes this output to our categories, giving us the **index** of the category it predicts.
You must also `return output.argmax()` to get the index to the communication system.

Because it gives the index, **it is crucial that you define the same category array/enums/#DEFINEs/etc. on your Arduino as you did when you created the model**.

### Starting the TX/RX:
To start the communication, run:

    client = JetsonWSClient.JetsonClient(handler, 'Team Name')
    client.ws.run_forever()

**Make sure your team name matches exactly to your Arduino**. This is how we will make sure who's Jetson is who's and who's (this town ain't big enough for Jetsons with unique names).

### Running the Script:
To run the script on the Jetson:
1. Open an instance of the terminal on Jupyter Notebook
2. Make sure you are in the same directory as your script. If not, use the `cd` command to enter into it
    1. Example: cd Your/Directory/Location/
3. Type: `python3 script_name.py` to run your script
4. Your Jetson should now be able to send and receive messages!

# Use of Jetson Functions on Arduino
Now that your Jetson can make predictions and talk to the Ardunio, it's time for your Arduino to talk back.
There are two functions in the Enes100 library. Those functions are described [here](https://enes100.umd.edu/libraries/enes100#ml_pred) on the Enes100.umd.edu website.
For both of these functions to work, **you will need the script you wrote to be running**. Otherwise, your Jetson will not communicate.
The `MLGetPrediction()` function will take a pic, send it to the Jetson, make use of your handler function, and return the category index to your Ardunio.
The `MLCaptureTrainingImage(String label)` function will take a pic, send it to the Jetson, and store it in a folder with the name `label`. The folders will be in the "Mission" folder on the Jetson.

### Recommendations for Data Collection:
Create an Ardunio file just contains a single `MLCaptureTrainingImage()` call, followed by a `while(1);` in the `loop()`. This will send 1 image to the Jetson, then stop. To take another image, just press the reset button on your Arduino. Have someone else check on the Jetson that the image successfully saved.

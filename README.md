# Spring 2023 Jetson Documentation

# Menu

[Updates and Notes - General Announcements](https://umdenes100.github.io/MachineLearningPilot/#updates-and-notes)   
[System Overview - How the ML Prediction System works and what you will do](https://umdenes100.github.io/MachineLearningPilot/#system-overview)   
[WiFi Camera Info - Info on the ML WiFi cameras and the training Cameras](https://umdenes100.github.io/MachineLearningPilot/#wifi-camera-info)   
[New Library Documentation for ML teams](https://umdenes100.github.io/MachineLearningPilot/#new-library)   
[Jetson First Time Setup](https://umdenes100.github.io/MachineLearningPilot/#jetson-first-time-setup)   
[Main Jetson Documentation - info on how to write script, collect data, etc.](https://umdenes100.github.io/MachineLearningPilot/#jetson-documentation)   
[Jetson Functions on Arduino](https://umdenes100.github.io/MachineLearningPilot/#use-of-jetson-functions-on-arduino)   
[Training](https://umdenes100.github.io/MachineLearningPilot/#training)    
[Debugging, FAQ, Recap](https://umdenes100.github.io/MachineLearningPilot/#debugging)    



# UPDATES AND NOTES

## **NOTICES!!!!!!**

**YOU MUST POWER THE BUFFER WITH 5V AND GROUND FOR EVERYTHING TO WORK**   

1. **UPDATE LIBRARY FOR BUG FIXES IF YOU DOWNLOADED ML LIBRARY UPDATE BEFORE 4/14**. To do so, download the zip from [this link](https://drive.google.com/drive/folders/1GIisAZlpRnHg12CQNAFu0rPoyvNGcJiR?usp=sharing) and extract it in your Arduino libraries folder. **Make sure to delete the previous library version**.

2. **YOU NEED TO MAKE A FOLDER CALLED `data` OUTSIDE OF THE MISSION FOLDER** if you plan to use the training image library function. Otherwise, the images **WILL NOT SAVE**. If you are using the training cameras, this does not affect you.

# System Overview
![image](https://user-images.githubusercontent.com/99224714/228002077-e242777d-0461-481b-b563-879b6cb3a2f5.png)

![image](https://user-images.githubusercontent.com/99224714/228002480-01052b31-b609-4923-b833-0dfe20391cf9.png)

# WiFi Camera Info

![image](https://user-images.githubusercontent.com/99224714/232099480-b2367cb3-0fef-46ce-8131-8f56e96ed30e.png)

![done](https://user-images.githubusercontent.com/99224714/229552578-56e42349-7b9f-48c0-b146-f26e0e1f9d49.png)

The ESPCAM case models are located [here](https://www.printables.com/model/280978-esp32-cam-case)

## Training Camera Instructions

1. Connect to the vision system wifi   
2. Connect to the IP on the cam `http://192.168.X.XX` via web browser. **MAKE SURE YOU ARE CONNECTED TO VISION SYSTEM WIFI**.  
3. Note: You will only need to power the cam (5 or 3.3 v, gnd) (5v is the top right pin when looking at back of cam, gnd is pin below that). You **DO NOT** need tx and rx for this.
4. Scroll to the bottom and select get still to capture an image.
![image](https://user-images.githubusercontent.com/99224714/228018845-726b1242-0a19-457f-a678-247fbb817699.png)
5. You can also select start stream to start a live stream, but you will not be able to save an image (use this to get an idea for perspective / POV).
6. Right click and select save image as (Windows).
7. You can upload to Jetson later via the Jupyter Notebook!

## Debugging Messages
If your camera does not seem to work, even after checking all wiring is correct and switiching TX and RX pins, you can check the debugger to see what is going on.
Go to the WiFi Camera case and get the USB to TTL converter (it's a small red thing with 5 prongs coming out one side and a USB adapter on the other). We will use this to recieve the debugging messages from the camera.
1. Install PuTTY if you do not already have it.
2. Wire according to this diagram.
![229552578-56e42349-7b9f-48c0-b146-f26e0e1f9d49](https://user-images.githubusercontent.com/99224714/231495148-925346da-91f9-4921-95c3-ebe679d73a36.png)
3. Plug USB side into laptop.
4. Check device manager to find the COM port.
![Screenshot 2023-04-12 104900](https://user-images.githubusercontent.com/99224714/231495623-f91c9528-1621-4de8-8ae6-ad1506133aa2.jpg)
5. Open PuTTY, switch to SERIAL setting, and type in the COM number and 115200 for speed. Click Open.
6. You should now be seeing debugging messages.

### Brownout Detected
There is not enough power to the camera, try making sure your wiring checks out and is not loose.

### Camera init failed
The camera is having issues. Try restarting the ESP using the reset button on the board (use a thin object).

### Other
Ask LTF Josh or LTF Forrest, or email jstone14@terpmail.umd.edu and use a different camera.

# New Library
The new library introduces some changes:
1. Instead of location.x, location.y, location.theta, you will instead use Enes100.getX(), Enes100.getY(), and Enes100.getTheta() to get the coordinate values.
2. To report the coordinate for your mission, you will need to create a Coordinate object to send to the Enes100.mission() function. The example code in the new library shows how to do this, and it is shown below.

![image](https://user-images.githubusercontent.com/99224714/231557337-55991f53-6b0b-4ba2-b26d-0c44dc4bf408.png)

# Jetson First Time Setup

1. Open command prompt
    1. On Windows, Windows button + R, type `cmd` and press enter
    2. On Mac, search `Terminal` in spotlight then open.
2. Type `ssh jetson@[ip address of Jetson]` and press enter
    1. If prompted, type: `yes` and press enter
3. Type in password: `jetson`
4. Type `./create.sh` and press enter
    1. Type your team name when prompted and press enter
    2. Type your mission type when prompted and press enter
    3. You're now ready to go!
5. Type `./doc_run_[teamname]-[mission].sh` and press enter
    1. CLI tip: pressing tab will auto complete when it is a unique file name. So, after you type `./doc_run_[first few letters]` it will likely auto complete with a tab.
    2. If prompted, type password: `jetson`
    3. If it gives an error, make sure the USB webcam is plugged into the Jetson Nano.
6. Now, Jupyter notebook should be started at the given ip.
7. Connect to the `[ip]:8888` on your web browser to see if the Jupyter notebook is up and running!

### Note: Juptyter password: dlinano

## For the Future:
To connect to the Jupyter notebook from here on out, follow the steps but skip step 4. **YOU SHOULD NOT RUN CREATE.SH AGAIN**. Just ssh to the jetson@ip, run the ./doc_run_ script, and connect to the IP. **RUNNING THE CREATE SCRIPT AGAIN MAY OVERWRITE YOUR PROGRESS**

# Jetson Documentation

### Jetson Documentation - ML Pilot Spring 2023 - Writing the Image Processing and Communication Script

### Writing the Script:
In order for your Jetson to send and receive messages, you need to run a script. 
More specifically, you need to run a script that we wrote for sending and receiving messages. 
However, because you are implementing your own machine learning model, you will need to write some stuff too. 
You will need to initialize your model, write a handler function, and start the sending/receiving script in your own .py python script. 

There is a description of what you will write shown below that will give you a good idea of what you will be writing.

First, navigate to the “Mission” folder on Jupyter Notebook. It should already contain a file called `JetsonWSClient.py`. **DO NOT EDIT THIS FILE** unless you want immense pain, suffering, etc. **WE WILL NOT HELP YOU DEBUG CHANGES TO THIS FILE**. There will also be a few other files, model training file that you will use for training the image data you will collect after setup.
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
**IF YOU WISH TO TEST ANOTHER PRETRAINED MODEL, SEE [THIS SECTION](https://umdenes100.github.io/MachineLearningPilot/#other-models)** To use resnet18, we define our model as: 

`model = torchvision.models.resnet18(pretrained=True)`

Next, we need to define our model dimensions, connect our model to the CUDA device, and load our pretrained model.

    model.fc = torch.nn.Linear(512, NUM_CATEGORIES)
    model = model.to(device)
    model.load_state_dict(torch.load('./model.pth'))

Replace the `'./model.pth'` with the path of your model, and you have successfully defined and loaded your model! Nice!
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

However, **you cant send whatever you want**, as long as it's an integer. One thing you could do is send the probability for each category. The following code gets and prints the probability for each.   

    for i, score in enumerate(list(output)):
        print(str(i) + " " + str(score))   
        
You could format a string of integers and wrap it with the int() constructor. To make sure they are all integers, use rounding and multiplying by factors of 10 to convert your decimal probabilities into integers.   
    
    results = ""
    for i, score in enumerate(list(output)):
        results = results + str(i) + str((round(score,2)*100))
    return int(results)
        
The results of this may look something like `0544414556`, which could be interpreted as 0 (category 1): 54.44% chance | 1 (category 2): 45.56% chance.
Note: **This code is untested and I just made it up right now, so don't just copy and paste expecting it to fully work.**

**These are just two options of many. Do whatever works for you.**

### Other Models:

In addition to resnet18, we have   
ALEXNET:   

    model = torchvision.models.alexnet(pretrained=True)
    model.classifier[-1] = torch.nn.Linear(4096, len(dataset.categories))
    
SQUEEZENET:   

    model = torchvision.models.squeezenet1_1(pretrained=True)
    model.classifier[1] = torch.nn.Conv2d(512, len(dataset.categories), kernel_size=1)  
    model.num_classes = len(dataset.categories)
    
RESNET34   

    model = torchvision.models.resnet34(pretrained=True)
    model.fc = torch.nn.Linear(512, len(dataset.categories))

If you are finding that your model isn't predicting well, and you have controlled for other reasonable factors, consider trying out a new pre-trained model! No harm no foul.

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
![image](https://user-images.githubusercontent.com/99224714/232099576-42766621-1175-4c72-91cf-ff0c5c854db9.png)
4. Your Jetson should now be able to send and receive messages!

# Use of Jetson Functions on Arduino
Now that your Jetson can make predictions and talk to the Ardunio, it's time for your Arduino to talk back.
There are two functions in the Enes100 library. Those functions are described [here](https://enes100.umd.edu/libraries/enes100#ml_pred) on the Enes100.umd.edu website.
For both of these functions to work, **you will need the script you wrote to be running**. Otherwise, your Jetson will not communicate.
The `MLGetPrediction()` function will take a pic, send it to the Jetson, make use of your handler function, and return the category index to your Ardunio.
The `MLCaptureTrainingImage(const char *label)` function will take a pic, send it to the Jetson, and store it in a folder with the name `label`. The folders will be in the "Mission" folder on the Jetson.
    1. A string literal (i.e. "cat", "dog") will satisfy the `const char *` parameter requirement. **The String object in Arduino will NOT satisfy this parameter. do NOT create a String object and try to pass to this function. Just use a literal or create a pointer.**

### Recommendations for Data Collection:
1. Create an Ardunio file just contains a single `MLCaptureTrainingImage(label)` call, followed by a `while(1);` in the `loop()`. This will send 1 image to the Jetson, then stop. To take another image, just press the reset button on your Arduino. Have someone else check on the Jetson that the image successfully saved.
2. Save your work by downloading any files you modify from Jupyter Notebook to your personal device. You never know what can happen.
3. For the Data Collection module, skip the part where you collect data yourself. If your folder names match your category names, the data should be ready to train when you finish running all cells.
4. Test your model by saving the model as a .pth, and putting the path of the model file in the communications script you will write.
    1. To get the path, right click on the model and select copy path.
    
# Training

In the training notebook, change the categories to match with the folders containing the data. Also, make sure to change the path to where you have the folder in the ImageClassificationDataset object.
1. I.e: ```datasets[name] = ImageClassificationDataset('./path/of/folders/, CATEGORIES, TRANSFORMS)```

You can also utilize multiple datasets if you want to have different iterations on how you collect and train data.
1. Create folders with the names of your datasets and make sure the array matches
2. Add the name to the path in the object 
3. I.e: ```datasets[name] = ImageClassificationDataset('./' + name + '/path/of/folders/, CATEGORIES, TRANSFORMS)```

    
# Debugging

Coming soon!
For now, see your UTF or lTF Josh for debugging.
Or, shoot an email to jstone14@terpmail.umd.edu

# To Recap:
Here is the process of getting started:
1. Connect to the Jetson and run the ./create.sh command to set up your workspace for the rest of the semester.
2. Connect to the Jupyter Notebook.
3. Write your communication script as in this description (and the example.py file).
4. On your Arduino, use the data collection function to send images to the Jetson
5. After you have finished collecting data, train your model.
6. Use your model by changing the path in the communications script.
7. Use the prediction Arduino function to recieve a prediction.
8. : )))))

How you choose to collect data is up to you. There are 4 choose 2 = 6 permutations for the ML mission site's plantable layout. But also, we have provided arucos that will allow you to do image manipulations (to possibly make more than 1 prediction per image if you change the handler function). There is not a correct answer, and that is the beauty of this course and pilot.

# FAQ

**Q**: No one on my team knows Python. Are we doomed?
**A**: No! Python is not hard to pick up, and frankly you don't really need to "learn" Python to create a working script. That being said, it is still a good skill to have, so consider learning if you have a light semester with much free time :).

**Q**: How do the cameras work and how will they be when we recieve them?
**A**: You will recieve the cameras in a case. There are two kinds of cases, one with a vertical, and other with horizontal, mounting. These cameras have WiFi capabilities, and use that to send images to the Jetson.

**Q**: How do we use the Aruco markers on the Mission site?
**A**: Use the opencv library! It is already installed on the Jetson, so if you `import cv2` in your handler script, you will be able to use the opencv library to read aruco markers. Use this and image array manipulations to crop, rotate, etc. to your image.

**Q**: How does the Machine Learning actually work?
**A**: The Jetson uses the RESNET18 neural network to train your model using the data you collect. This neural network comes with 18 layers, many layers already trained. The Jetson is a machine learning ... machine and is built for image processing with ML. [See this video](https://www.youtube.com/watch?v=aircAruvnKk) to learn more about how neural networks work.

**Q**: Help!!!!!!
**A**: Ask your TF, or LTF Josh for assistance with any other thing. Or, email jstone14@terpmail.umd.edu.

# Final Notices:

Feedback is critical to making sure everything goes well.
If you have any feedback or suggestions, shoot it to a TF or jstone14@terpmail.umd.edu.

Here is a chat GPT generated good luck message:
    Dear Students of ENES100,
    
    We at LTF would like to wish you the best of luck as you embark on your exciting journey to create a machine learning model for use on your over terrain vehicles using Jetson Nanos. We understand that this task can be challenging, but we have no doubt that you will rise to the occasion and succeed in this endeavor.
    
    You have already demonstrated your dedication and hard work in pursuing your education, and we have every confidence that you will continue to do so in this project. Remember to stay focused, remain motivated, and always keep your end goal in mind.
    
    We hope that you will find joy and satisfaction in the process of developing your machine learning model and that you will learn valuable skills that will benefit you in your future endeavors. Once again, we wish you the best of luck and look forward to seeing the impressive results of your hard work.

    Sincerely,
    
    The LTF Team

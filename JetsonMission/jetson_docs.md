# OLD - Refer to current README for student info.
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
To use resnet18, we define our model as: 

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
1. Create an Ardunio file just contains a single `MLCaptureTrainingImage()` call, followed by a `while(1);` in the `loop()`. This will send 1 image to the Jetson, then stop. To take another image, just press the reset button on your Arduino. Have someone else check on the Jetson that the image successfully saved.
2. Save your work by downloading any files you modify from Jupyter Notebook to your personal device. You never know what can happen.
3. For the Data Collection module, skip the part where you collect data yourself. If your folder names match your category names, the data should be ready to train when you finish running all cells.
4. Test your model by saving the model as a .pth, and putting the path of the model file in the communications script you will write.
    1. To get the path, right click on the model and select copy path.
    
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

Here is a chat GPT generated good luck message:
    Dear Students of ENES100,
    
    We at LTF would like to wish you the best of luck as you embark on your exciting journey to create a machine learning model for use on your over terrain vehicles using Jetson Nanos. We understand that this task can be challenging, but we have no doubt that you will rise to the occasion and succeed in this endeavor.
    
    You have already demonstrated your dedication and hard work in pursuing your education, and we have every confidence that you will continue to do so in this project. Remember to stay focused, remain motivated, and always keep your end goal in mind.
    
    We hope that you will find joy and satisfaction in the process of developing your machine learning model and that you will learn valuable skills that will benefit you in your future endeavors. Once again, we wish you the best of luck and look forward to seeing the impressive results of your hard work.

    Sincerely,
    
    The LTF Team

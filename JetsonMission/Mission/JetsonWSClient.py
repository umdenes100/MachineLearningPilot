# WARNING STUDENTS - IF YOU EDIT THIS FILE THINGS MAY NOT WORK. WE WILL NOT HELP YOU DEBUG ERRORS CAUSED BY EDITING THIS FILE.
# If you choose to edit it, save a copy first of the pre-edited state.

import websocket
import json
import os
import PIL.Image
import numpy as np
import io
import cv2

import torch
import torchvision.transforms as transforms
import torch.nn.functional as F


mean = torch.Tensor([0.485, 0.456, 0.406]).cuda()
std = torch.Tensor([0.229, 0.224, 0.225]).cuda()


def preprocess(image):
    device = torch.device('cuda')
    image_str = io.BytesIO(image)
    #image_str = PIL.Image.frombytes(data=image_str,mode='RGB',size=(224,224),decoder_name='jpeg')
    picture = PIL.Image.open(image_str)
    picture.save('./curr.jpeg','JPEG')
    #print(picture.format)
    #picture.show()
    
    image = transforms.functional.to_tensor(picture).to(device)
    image.sub_(mean[:, None, None]).div_(std[:, None, None])
    return image[None, ...]



class JetsonClient:
    # See first line of this file if you want to edit this function
    def on_message(self, _, message):
        print(message)
        message = json.loads(message)
        if message['op'] == 'prediction_request':
            losBytes = bytes.fromhex(message['image'])
            print('entering preprocess')
            preprocessed = preprocess(losBytes)
            print('entering handler')
            results = self.handler(preprocessed)
            print('sending')
            self.ws.send(json.dumps({
                "op": "prediction_results",
                "prediction": results
            }))
            print('sent :)')
        if message['op'] == 'image_capture':
            losBytes = bytes.fromhex(message['image'])
            print('entering preprocess')
            image_str = io.BytesIO(losBytes)
            #image_str = PIL.Image.frombytes(data=image_str,mode='RGB',size=(224,224),decoder_name='jpeg')
            picture = PIL.Image.open(image_str)
            if not os.path.isdir('./data/' + message['category']):
                os.mkdir('./data/' + message['category'])
            i = 0
            while os.path.exists('./data/' + message['category'] + '/im' + str(i) + '.jpeg'):
                i += 1
            picture.save('./data/' + message['category'] + '/im' + str(i) + '.jpeg','JPEG')
            print('saved :)')
        
    # See first line of this file if you want to edit this function
    def on_open(self, _):
        print("Opened!")
        self.ws.send(json.dumps({
            "op": "begin",
            "teamName": self.team_name
        }))
           
    def on_error(self, _, error):
        print(error)
        
    # See first line of this file if you want to edit this function
    def __init__(self, handler, team_name):
        self.team_name = team_name
        self.handler = handler
        # websocket.enableTrace(True)
        self.ws = websocket.WebSocketApp("ws://192.168.1.2:7756", on_message=self.on_message, on_open=self.on_open, on_error=self.on_error)

import torch
import torchvision
from utils import preprocess
import torch.nn.functional as F
import JetsonWSClient

categories = ['l', 't', 'f']
device = torch.device('cuda')
model = torchvision.models.resnet18(pretrained=True)
model.fc = torch.nn.Linear(512, 2)
model = model.to(device)
model.load_state_dict(torch.load('model_path'))


def handler(image):
    preprocessed = preprocess(image)
    output = model(preprocessed)
    output = F.softmax(output, dim=1).detach().cpu().numpy().flatten()
    return categories[output.argmax()]


client = JetsonWSClient.JetsonClient(handler, 'Team Bofa')
client.start()
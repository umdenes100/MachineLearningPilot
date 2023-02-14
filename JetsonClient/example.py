import torch
import torchvision
from utils import preprocess
import torch.nn.functional as F
import JetsonWSClient
import threading

categories = ['l', 'tf']
device = torch.device('cuda')
model = torchvision.models.resnet18(pretrained=True)
model.fc = torch.nn.Linear(512, 2)
model = model.to(device)
model.load_state_dict(torch.load('../data/dirt-mission/Josh_LTF/model.pth'))


def handler(image):
    preprocessed = preprocess(image)
    output = model(preprocessed)
    output = F.softmax(output, dim=1).detach().cpu().numpy().flatten()
    return categories[output.argmax()]

client = JetsonWSClient.JetsonClient(handler, 'Team Bofa')

# threading.Thread(target=client.ws.run_forever, name="WS Thread",daemon=False).start()
client.ws.run_forever()
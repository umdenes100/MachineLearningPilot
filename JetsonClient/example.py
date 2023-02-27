import torch
import torchvision
from utils import preprocess
import torch.nn.functional as F
import JetsonWSClient
import threading

print('setup...')
categories = ['greg', 'josh']
device = torch.device('cuda')
model = torchvision.models.resnet18(pretrained=True)
model.fc = torch.nn.Linear(512, 2)
model = model.to(device)
model.load_state_dict(torch.load('../model.path'))
print('setup complete')


def handler(image):
    output = model(image)
    output = F.softmax(output, dim=1).detach().cpu().numpy().flatten()
    print(categories[output.argmax()])
    return categories[output.argmax()]


client = JetsonWSClient.JetsonClient(handler, 'Team Bofa')

#threading.Thread(target=client.ws.run_forever, name="WS Thread",daemon=True).start()

print('Starting...')
client.ws.run_forever()

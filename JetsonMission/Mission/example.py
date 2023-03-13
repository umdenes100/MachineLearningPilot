import torch
import torchvision
import torch.nn.functional as F
import JetsonWSClient

#print('setup...')
#categories = ['cat', 'dog', 'gorilla']
device = torch.device('cuda')
model = torchvision.models.resnet18(pretrained=True)
model.fc = torch.nn.Linear(512, 2)
model = model.to(device)
model.load_state_dict(torch.load('../model.path'))
#print('setup complete')


def handler(image):
    output = model(image)
    output = F.softmax(output, dim=1).detach().cpu().numpy().flatten()
    #print(categories[output.argmax()])
    return output.argmax()


client = JetsonWSClient.JetsonClient(handler, 'Team Name')
client.ws.run_forever()

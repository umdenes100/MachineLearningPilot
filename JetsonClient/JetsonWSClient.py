import websocket
import json


class JetsonClient:
    def on_message(self, _, message):
        print(message)
        message = json.loads(message)
        if message['op'] == 'prediction':
            # TODO add hex decoder
            results = self.handler(message['image'])
            self.ws.send(json.dumps({
                "op": "prediction_results",
                "prediction": results
            }))
        
        
    def on_open(self, _):
        print("Opened!")
        self.ws.send(json.dumps({
            "op": "begin",
            "teamName": self.team_name
        }))
        
        
    def on_error(self, _, error):
        print(error)
        

    def __init__(self, handler, team_name):
        self.team_name = team_name
        self.handler = handler
        # websocket.enableTrace(True)
        self.ws = websocket.WebSocketApp("ws://192.168.1.2:7756", on_message=self.on_message, on_open=self.on_open, on_error=self.on_error)
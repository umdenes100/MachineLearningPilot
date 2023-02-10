import websocket
import json


class JetsonClient:

    def __init__(self, handler, team_name):
        self.team_name = team_name
        self.handler = handler
        self.ws = websocket.WebSocketApp("ws://192.168.1.2:9000/", on_message=self.on_message)
        # self.ws.connect("ws://192.168.1.2:9000/")
        # noinspection PyTypeChecker
        self.ws.send(json.loads({
            "name": team_name
        }))

    def on_message(self, message):
        message = json.loads(message)
        if message['op'] == 'prediction':
            # TODO add hex decoder
            results = self.handler(message['image'])
            # noinspection PyTypeChecker
            self.ws.send(json.loads({
                "op": "prediction_results",
                "prediction": results,
                "team_name": self.team_name
            }))

    def start(self):
        self.ws.run_forever()
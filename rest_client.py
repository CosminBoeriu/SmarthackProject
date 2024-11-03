import requests
import json
import socket

class Penalty:
    def __init__(self, day, type, message, cost, co2):
        self.day = day
        self.type = type
        self.message = message
        self.cost = cost
        self.co2 = co2

delta_kpis = {"day" : 0, "cost" : 0, "co2" : 0}
total_kpis = { "day" : 0, "cost" : 0, "co2" : 0}
penalty_lst = []
def make_start_request(api_key):
    
    headers = {
    'Content-Type': 'application/json',
    'API-KEY': api_key
    }
    
    url = "http://localhost:8080/api/v1/session/start"
    response = requests.post(url, headers=headers)
    
    return response.text

def make_stop_request(api_key, session_id):
    
    headers = {
    'Content-Type': 'application/json',
    'API-KEY': api_key,
    'SESSION-ID': session_id
    }
    
    url = "http://localhost:8080/api/v1/session/end"
    
    response = requests.post(url, headers=headers)
    
    return response.text
    

def make_play_request(api_key, session_id, movements):
        
    headers = {
    'Content-Type': 'application/json',
    'API-KEY': api_key,
    'SESSION-ID': session_id
    }
        
    url = "http://localhost:8080/api/v1/play/round"
        
        
    response = requests.post(url, headers=headers, data=json.dumps(movements))
        
    return response.text

def parse_demands(demands):
    lst = demands.get("demand")
    string = f"{len(demands)} "
    for elem in lst:
        string += f"{elem.get('customerId')},{elem.get('amount')},{elem.get('postDay')},{elem.get('startDay')},{elem.get('endDay')}@"
    return string[:-1]

def add_penalty_to_list(penalty):
    lst = penalty.get("penalties")
    
    for elem in lst:
        penalty_lst.append(Penalty(elem.get("day"), elem.get("type"), elem.get("message"), elem.get("cost"), elem.get("co2")))

def update_kpi(response):
    global delta_kpis
    global total_kpis
    delta_kpis["day"] = response.get("deltaKpis").get("day")
    delta_kpis["cost"] = response.get("deltaKpis").get("cost")
    delta_kpis["co2"] = response.get("deltaKpis").get("co2")
    
    total_kpis["day"] = response.get("totalKpis").get("day")
    total_kpis["cost"] = response.get("totalKpis").get("cost")
    total_kpis["co2"] = response.get("totalKpis").get("co2")

def parse_movements(response):
    num_movements, movements = response.split()
    ret = []
    for movement in movements.split("@"):
        ret.append({"connectionId" : movement.split(",")[0], "amount" : movement.split(",")[1]})
        
    return ret



sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

PORT = 12345
SERVER_HOST = "127.0.0.1"
sock.connect((SERVER_HOST, PORT))

session_id = make_start_request("7bcd6334-bc2e-4cbf-b9d4-61cb9e868869")
print(session_id)
for day in range(42):
    sock.send(b"move")
    moves = sock.recv(4096)
    movements = parse_movements(moves.encode("utf-8"))
    #movements = parse_movements(serv_resp)
    resp = make_play_request("7bcd6334-bc2e-4cbf-b9d4-61cb9e868869", session_id, {"day": day, "movements": movements})
    print(resp)
    #add_penalty_to_list(resp)
    #update_kpi(resp)
    sock.send(parse_demands(resp))
    #print(f"Day: {total_kpis['day']}, Cost: {total_kpis['cost']}, CO2: {total_kpis['co2']}")
    
make_stop_request("7bcd6334-bc2e-4cbf-b9d4-61cb9e868869", session_id)
sock.send(b"quit")
sock.close()

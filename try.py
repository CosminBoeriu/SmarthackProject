import requests
import json
import socket
import pandas as pd
import numpy as np

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


df_con = pd.read_csv("/Users/dariadragomir/challengeSmarthack/eval-platform/target/classes/liquibase/data/connections.csv" , sep=';')
df_ref = pd.read_csv("/Users/dariadragomir/challengeSmarthack/eval-platform/target/classes/liquibase/data/refineries.csv", sep=';')
# Check if df_ref is loaded correctly and has data


df_ref = df_ref['id']
df_con = df_con[df_con["from_id"].isin(df_ref)]

session_id = make_start_request("7bcd6334-bc2e-4cbf-b9d4-61cb9e868869")
print(session_id)
for day in range(42):
    movements = []
    no_movements = np.random.randint(1, 6)
    for _ in range(no_movements):
        movements.append({"connectionId" : df_con.sample(n=1)["id"].values[0], "amount" : np.random.randint(1, 6)})
    resp = make_play_request("7bcd6334-bc2e-4cbf-b9d4-61cb9e868869", session_id, {"day": day, "movements": movements})
    print(resp)
    #add_penalty_to_list(resp)
    #update_kpi(resp)
    #sock.send(parse_demands(resp))
    #print(f"Day: {total_kpis['day']}, Cost: {total_kpis['cost']}, CO2: {total_kpis['co2']}")
    
make_stop_request("7bcd6334-bc2e-4cbf-b9d4-61cb9e868869", session_id)
#sock.send(b"quit")


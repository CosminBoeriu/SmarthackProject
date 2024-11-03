import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import time

connections = pd.read_csv("/Users/dariadragomir/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/connections.csv", sep=";", usecols=["from_id", "to_id"])
refineries = pd.read_csv("/Users/dariadragomir/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/refineries.csv", sep=";", usecols=["id"])
tanks = pd.read_csv("/Users/dariadragomir/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/tanks.csv", sep=";", usecols=["id"])
clients = pd.read_csv("/Users/dariadragomir/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/customers.csv", sep=";", usecols=["id"])
demands = pd.read_csv("/Users/dariadragomir/challengeSmarthack/eval-platform/src/main/resources/liquibase/data/demands.csv", sep=";", usecols=["customer_id", "start_delivery_day", "end_delivery_day"])
demand_list = demands.values.tolist() 

def draw_graph(day):
    G = nx.DiGraph()

    G.add_nodes_from(refineries['id'], type="refinery")
    G.add_nodes_from(tanks['id'], type="tank")
    G.add_nodes_from(clients['id'], type="client")

    for idx, row in connections.iterrows():
        if row['from_id'] in refineries['id'].values and row['to_id'] in tanks['id'].values:
            G.add_edge(row['from_id'], row['to_id'], color='blue')  # Refinery to Tank
        elif row['from_id'] in tanks['id'].values and row['to_id'] in tanks['id'].values:
            G.add_edge(row['from_id'], row['to_id'], color='green')  # Tank to Tank

    pos = {}
    y_spacing = 2  

    refinery_positions = np.linspace(-1, 1, len(refineries))
    for idx, row in enumerate(refineries['id']):
        pos[row] = (refinery_positions[idx], y_spacing * 2)

    tank_rows = 3  
    tanks_per_row = (len(tanks) + tank_rows - 1) // tank_rows
    for idx, row in enumerate(tanks['id']):
        row_idx = idx // tanks_per_row  
        x_position = (idx % tanks_per_row) / (tanks_per_row - 1) * 2 - 1 
        pos[row] = (x_position, y_spacing * (1.5 - row_idx * 0.8))  

    client_positions = np.linspace(-1, 1, len(clients))
    for idx, row in enumerate(clients['id']):
        pos[row] = (client_positions[idx], y_spacing * -1.5) 

    for tank_id in tanks['id']:
        for customer_id, start_delivery_day, end_delivery_day in demand_list:
            if start_delivery_day <= day <= end_delivery_day:
                if G.has_node(customer_id):
                    G.add_edge(tank_id, customer_id, color='gray')  

    plt.figure(figsize=(10, 6))  
    nx.draw_networkx_nodes(G, pos, nodelist=refineries['id'], node_color='navy', node_shape='s', label="Refineries")
    nx.draw_networkx_nodes(G, pos, nodelist=tanks['id'], node_color='pink', node_shape='o', label="Tanks")
    nx.draw_networkx_nodes(G, pos, nodelist=clients['id'], node_color='lightgreen', node_shape='^', label="Clients")

    edge_colors = [G[u][v]['color'] for u, v in G.edges()]
    nx.draw_networkx_edges(G, pos, edge_color=edge_colors, width=0.2)

    plt.title(f"Graph of Refineries, Tanks, and Clients (Day {day})")
    plt.axis('off')
    plt.show(block=False)
    plt.pause(3.5)
    plt.close()

for day in range(1, 43):
    draw_graph(day)
    time.sleep(2) 
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import time

# Load connections data
connections = pd.read_csv("connections.csv", sep=";", usecols=["from_id", "to_id"])

# Load nodes data
refineries = pd.read_csv("refineries.csv", sep=";", usecols=["id"])
tanks = pd.read_csv("tanks.csv", sep=";", usecols=["id"])
clients = pd.read_csv("customers.csv", sep=";", usecols=["id"])

# Load demands data and store in a list
demands = pd.read_csv("demands.csv", sep=";", usecols=["customer_id", "start_delivery_day", "end_delivery_day"])
demand_list = demands.values.tolist()  # Convert DataFrame to a list of lists

# Function to draw the graph based on the delivery day
def draw_graph(day):
    # Create a new directed graph
    G = nx.DiGraph()

    # Add nodes for refineries, tanks, and clients
    G.add_nodes_from(refineries['id'], type="refinery")
    G.add_nodes_from(tanks['id'], type="tank")
    G.add_nodes_from(clients['id'], type="client")

    # Add edges based on the connections DataFrame
    for idx, row in connections.iterrows():
        if row['from_id'] in refineries['id'].values and row['to_id'] in tanks['id'].values:
            G.add_edge(row['from_id'], row['to_id'], color='blue')  # Refinery to Tank
        elif row['from_id'] in tanks['id'].values and row['to_id'] in tanks['id'].values:
            G.add_edge(row['from_id'], row['to_id'], color='green')  # Tank to Tank

    # Prepare positions for drawing
    pos = {}
    y_spacing = 2  # Vertical distance between rows

    # Set positions for refineries (top row, evenly spaced)
    refinery_positions = np.linspace(-1, 1, len(refineries))
    for idx, row in enumerate(refineries['id']):
        pos[row] = (refinery_positions[idx], y_spacing * 2)

    # Set positions for tanks (multiple rows)
    tank_rows = 3  # Number of rows for tanks
    tanks_per_row = (len(tanks) + tank_rows - 1) // tank_rows  # Calculate how many tanks per row
    for idx, row in enumerate(tanks['id']):
        row_idx = idx // tanks_per_row  # Determine which row this tank is in
        x_position = (idx % tanks_per_row) / (tanks_per_row - 1) * 2 - 1  # Spread tanks in the row
        pos[row] = (x_position, y_spacing * (1.5 - row_idx * 0.8))  # Spread tanks over rows

    # Set positions for clients (single row at the bottom)
    client_positions = np.linspace(-1, 1, len(clients))
    for idx, row in enumerate(clients['id']):
        pos[row] = (client_positions[idx], y_spacing * -1.5)  # Spread clients in a single row below tanks

    # Add edges from tanks to clients based on demands
    for tank_id in tanks['id']:
        for customer_id, start_delivery_day, end_delivery_day in demand_list:
            if start_delivery_day <= day <= end_delivery_day:
                # Add an edge from tank to client if the client is valid
                if G.has_node(customer_id):  # Check if the client node exists
                    G.add_edge(tank_id, customer_id, color='gray')  # Color for valid tank to client edges

    # Draw the graph
    plt.figure(figsize=(10, 6))  # Set the figure size
    nx.draw_networkx_nodes(G, pos, nodelist=refineries['id'], node_color='navy', node_shape='s', label="Refineries")
    nx.draw_networkx_nodes(G, pos, nodelist=tanks['id'], node_color='pink', node_shape='o', label="Tanks")
    nx.draw_networkx_nodes(G, pos, nodelist=clients['id'], node_color='lightgreen', node_shape='^', label="Clients")

    # Draw edges
    edge_colors = [G[u][v]['color'] for u, v in G.edges()]
    nx.draw_networkx_edges(G, pos, edge_color=edge_colors, width=0.2)

    # Display title and hide axes
    plt.title(f"Graph of Refineries, Tanks, and Clients (Day {day})")
    plt.axis('off')
    plt.show(block=False)  # Show the plot without blocking

    plt.pause(2)  # Pause for 2 seconds to display the figure
    plt.close()  # Close the plot window after pausing

# Main loop with automatic progression through delivery days
for day in range(1, 43):
    draw_graph(day)
    time.sleep(2)  # Wait for 2 seconds before drawing the next day

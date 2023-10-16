import numpy as np
import pandas as pd
import random
import matplotlib.pyplot as plt
import os

def read_data(filename):
    filepath = os.path.join("data", filename)
    points = pd.read_csv(filepath, sep=";", header=None)
    points = np.array(points)
    num_nodes = len(points)
    distance_table = np.zeros((num_nodes, num_nodes))

    for i in range(num_nodes):
        for j in range(num_nodes):
            if i == j:
                continue
            distance_table[i][j] = round(np.sqrt((points[i][0] - points[j][0])**2 + (points[i][1] - points[j][1])**2) + points[j][2])
    return distance_table, points

def calculate_total_distance(cycle, distance_table):
    total_distance = 0
    for i in range(len(cycle) - 1):
        total_distance += distance_table[cycle[i]][cycle[i + 1]]
    total_distance += distance_table[cycle[-1]][cycle[0]]
    return total_distance

def random_solution(distance_table, num_runs=200):
    num_nodes = len(distance_table)
    best_solution = None
    min_distance = float('inf')
    max_distance = 0
    total_distance = 0

    for _ in range(num_runs):
        nodes = list(range(num_nodes))
        random.shuffle(nodes)
        selected_nodes = nodes[:num_nodes // 2]
        cycle = selected_nodes + [selected_nodes[0]]
        distance = calculate_total_distance(cycle, distance_table)
        total_distance += distance
        if distance < min_distance:
            min_distance = distance
            best_solution = cycle
        if distance > max_distance:
            max_distance = distance

    average_distance = total_distance / num_runs
    return best_solution, min_distance, max_distance, average_distance

def nearest_neighbor(distance_table, num_runs=200):
    num_nodes = len(distance_table)
    best_solution = None
    min_distance = float('inf')
    max_distance = 0
    total_distance = 0

    for _ in range(num_runs):
        start_node = random.randint(0, num_nodes - 1)
        cycle = [start_node]
        distance = 0

        for _ in range(num_nodes // 2 - 1):
            min_dist = float('inf')
            nearest_node = None

            for node in range(num_nodes):
                if node not in cycle:
                    if distance_table[cycle[-1]][node] < min_dist:
                        min_dist = distance_table[cycle[-1]][node]
                        nearest_node = node

            if nearest_node is not None:
                cycle.append(nearest_node)
                distance += min_dist

        distance += distance_table[cycle[-1]][start_node]

        total_distance += distance
        if distance < min_distance:
            min_distance = distance
            best_solution = cycle
        if distance > max_distance:
            max_distance = distance

    average_distance = total_distance / num_runs
    return best_solution, min_distance, max_distance, average_distance

def greedy_cycle(distance_table, num_runs=200):
    num_nodes = len(distance_table)
    best_solution = None
    min_distance = float('inf')
    max_distance = 0
    total_distance = 0

    for _ in range(num_runs):
        start_node = random.randint(0, num_nodes - 1)
        cycle = [start_node]
        distance = 0
        for _ in range(num_nodes // 2 - 1):
            min_dist = float('inf')
            nearest_node = None
            for node in range(num_nodes):
                if node not in cycle:
                    if distance_table[cycle[-1]][node] < min_dist:
                        min_dist = distance_table[cycle[-1]][node]
                        nearest_node = node
            if nearest_node is not None:
                cycle.append(nearest_node)
                distance += min_dist
        distance += distance_table[cycle[-1]][start_node]
        new_cycle = optimize_cycle(cycle, distance_table)
        distance = calculate_total_distance(new_cycle, distance_table)
        total_distance += distance
        if distance < min_distance:
            min_distance = distance
            best_solution = new_cycle
        if distance > max_distance:
            max_distance = distance
    average_distance = total_distance / num_runs
    return best_solution, min_distance, max_distance, average_distance

def optimize_cycle(cycle, distance_table):
    num_nodes = len(cycle)
    for i in range(num_nodes):
        for j in range(i + 2, num_nodes):
            if i == 0 and j == num_nodes - 1:
                continue
            new_cycle = cycle[:i + 1] + cycle[i + 1:j][::-1] + cycle[j:]
            if calculate_total_distance(new_cycle, distance_table) < calculate_total_distance(cycle, distance_table):
                cycle = new_cycle
    return cycle

def visualize_solution(best_solution, points, plot_name):
    x = [point[0] for point in points]
    y = [point[1] for point in points]
    cost = [point[2] for point in points]
    best_solution.append(best_solution[0])
    best_x = [x[i] for i in best_solution]
    best_y = [y[i] for i in best_solution]

    plt.figure(figsize=(8, 6))
    plt.scatter(x, y, c=cost, cmap='magma_r', s=50)
    plt.plot(best_x, best_y, 'bo-', markersize=3, linewidth=1)
    plt.colorbar(label='node cost')
    plt.xlabel('x coordinate')
    plt.ylabel('y coordinate')
    plt.title(plot_name)
    plt.grid()
    plt.savefig(os.path.join("plots", "plot_" + plot_name + ".png"))

letters = ["A", "B", "C", "D"]
if not os.path.exists("results"):
    os.makedirs("results")
if not os.path.exists("plots"):
    os.makedirs("plots")

for letter in letters:
    filename = "TSP" + letter + ".csv"
    distance_table, points = read_data(filename)

    rs_best, rs_min, rs_max, rs_avg = random_solution(distance_table)
    with open(os.path.join("results", "results_random_solution" + letter + ".txt"), "w") as file:
        file.write("Random Solution:\n")
        file.write("Minimum Distance: " + str(rs_min) + "\n")
        file.write("Maximum Distance: " + str(rs_max) + "\n")
        file.write("Average Distance: " + str(rs_avg) + "\n")

    nn_best, nn_min, nn_max, nn_avg = nearest_neighbor(distance_table)
    with open(os.path.join("results", "results_nearest_neighbor" + letter + ".txt"), "w") as file:
        file.write("Nearest Neighbor:\n")
        file.write("Minimum Distance: " + str(nn_min) + "\n")
        file.write("Maximum Distance: " + str(nn_max) + "\n")
        file.write("Average Distance: " + str(nn_avg) + "\n")

    gc_best, gc_min, gc_max, gc_avg = greedy_cycle(distance_table)
    with open(os.path.join("results", "results_greedy_cycle" + letter + ".txt"), "w") as file:
        file.write("Greedy Cycle:\n")
        file.write("Minimum Distance: " + str(gc_min) + "\n")
        file.write("Maximum Distance: " + str(gc_max) + "\n")
        file.write("Average Distance: " + str(gc_avg) + "\n")

    visualize_solution(rs_best, points, "random_solution" + letter)
    visualize_solution(nn_best, points, "nearest_neighbor" + letter)
    visualize_solution(gc_best, points, "greedy_cycle" + letter)

from sys import stdin, maxsize
import queue
from collections import defaultdict
import djikstras
Inf = 3e10000

#Ikke bra ennå 
def shortest_path(order_list, adjacency_matrix, cities):
    currCost = [Inf for i in adjacency_matrix]
    neig = [[] for i in adjacency_matrix]
    for i in range(len(adjacency_matrix)):
        row = adjacency_matrix[i]
        for j in range(len(adjacency_matrix)):
            if row[j] != -1:
                neig[i].append(j)
                currCost = row[j]
    density =0
    for i in neig:
        density+= len(i)

    djikstras.best_path(neig, currCost)
    last = order_list[0]
    cost =0
    solutions = {}

    for i in order_list[1:]:
        if (last, i) in solutions:
            cost+= solutions[(last, i)]
        else:
            tempCost = dijkstras(last,i, adjacency_matrix)
            cost += tempCost
            solutions[(last, i)] = tempCost
    if (order_list[-1], order_list[0]) in solutions:
        cost+= solutions[ (order_list[-1], order_list[0])] 
    else:
        cost+= dijkstras(order_list[-1],order_list[0], adjacency_matrix)
    if cost< Inf:
        return str(cost)
    else: 
        return "umulig"


def dijkstras(frm, to, mat):
    fromRow = mat[frm]
    costs = [i for i in fromRow]
    D = defaultdict(list)




def fixMatrix(mat):
    counter =0
    for i in range(len(mat)):
        row= mat[i]
        for j in range(len(row)):
            if row[j] == -1:
                row[j] = Inf
            else:
                counter +=1
testcases = int(stdin.readline())
for test in range(testcases):
    cities = int(stdin.readline())
    order_list = [int(city) for city in stdin.readline().split()]
    adjacency_matrix = []
    for city in range(cities):
        adjacency_matrix.append([ int(i) for i in stdin.readline().strip().split()])
    print(shortest_path(order_list, adjacency_matrix, cities))
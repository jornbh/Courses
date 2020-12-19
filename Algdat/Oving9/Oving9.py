from sys import stdin


Inf = float(1e3000)



def improvePrices(nodeNeighbors, prices, unknowns):
    for i in unknowns:
        prices[i] = min(nodeNeighbors[i], prices[i])


def findMinIndex(prices, unknowns):
    minimum = prices[unknowns[0]]
    index =unknowns[0]
    for i in unknowns[1:]:
        if prices[i] < minimum:
            minimum = prices[i]
            index = i
    return index


def mst(nm):
    length = len(nm)
    prices = [Inf for i in range(length)]
    unknowns = [i for i in range(length)]
    row = nm[0]

    mostExpensive= 0
    while unknowns:
        improvePrices(row, prices, unknowns)
        index = findMinIndex(prices, unknowns)
        if prices[index] > mostExpensive:
            mostExpensive = prices[index]
        unknowns.remove(index)
        row = nm[index]
    return mostExpensive

lines = []
for str in stdin:
    lines.append(str)
n = len(lines)
neighbour_matrix = [None] * n
node = 0
for line in lines:
    neighbour_matrix[node] = [Inf] * n
    for k in line.split():
        data = k.split(':')
        neighbour = int(data[0])
        weight = int(data[1])
        neighbour_matrix[node][neighbour] = weight
    node += 1
print (mst(neighbour_matrix))


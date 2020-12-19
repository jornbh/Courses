from sys import stdin, maxsize
Inf = 3e10000
def shortest_path(order_list, adjacency_matrix, cities):
    fixMatrix(adjacency_matrix)

    for middle in range(cities):
        for a in range(cities):
            for b in range(cities):
                if adjacency_matrix[a][b] > adjacency_matrix[a][middle] + adjacency_matrix[middle][b]:
                    adjacency_matrix[a][b] = adjacency_matrix[a][middle] + adjacency_matrix[middle][b]
    tot =0
    for i in range(len(order_list)-1):
        tot += adjacency_matrix[order_list[i]][order_list[i+1]]
    tot += adjacency_matrix[order_list[-1]][order_list[0]]
    if tot <Inf:
        return str(tot)
    else:
        return "umulig"




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
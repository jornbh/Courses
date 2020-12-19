from sys import stdin, maxsize









testcases = int(stdin.readline())
for test in range(testcases):
    cities = int(stdin.readline())
    order_list = [int(city) for city in stdin.readline().split()]
    adjacency_matrix = []
    neighbours =[[] for j in range(cities)]
    for city in range(cities):
        line = ([ int(i) for i in stdin.readline().strip().split()])
        adjacency_matrix.append(line)
        for i in range(len(line)):
            if  i!= city:
                if line[i] != -1:
                    print("#", i, line[i])
                    neighbours[city].append((i,line[i]))
    print()
    print(neighbours)
    # print(shortest_path(order_list, adjacency_matrix, cities))
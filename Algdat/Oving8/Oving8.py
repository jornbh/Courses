#!/usr/bin/python3
import queue
from sys import stdin


class Node:
    def __init__(self):
        self.child = []
        self.ratatosk = False
        self.next_child = 0


def dfs(root):
    stack = queue.LifoQueue()
    stack.put((root, 0))
    while not (stack.empty()):
        temp =stack.get()
        if temp[0].ratatosk:
            return temp[1]
        else:
            for i in temp[0].child:
                stack.put((i, temp[1]+1))
        

def bfs(root):
    Q = queue.Queue()
    Q.put((root,0))
    while not Q.empty():
        temp =Q.get()
        for i in temp[0].child:
            Q.put((i, temp[1]+1))
        if(temp[0].ratatosk):
            return temp[1]

def new_search():
    pass


function = stdin.readline().strip()
number_of_nodes = int(stdin.readline())

if function == 'dfs':
    fptr =dfs
elif function == 'bfs':
    fptr = bfs
elif function == 'velg':
    start_index = int(stdin.readline())
    ratatosk_index = int(stdin.readline())
    parents = [None for i in range(number_of_nodes)]
    for i in stdin:
        words = i.split()
        parent = int(words.pop(0))
        for j in words:
            parents[int(j)] = parent
    index = ratatosk_index
    levelCount =0
    while index != start_index:
        levelCount+=1
        index = parents[index]
    print( levelCount)
    quit()


nodes = []
for i in range(number_of_nodes):
    nodes.append(Node())
start_node = nodes[int(stdin.readline())]
ratatosk_node = nodes[int(stdin.readline())]
ratatosk_node.ratatosk = True
for line in stdin:
    number = line.split()
    temp_node = nodes[int(number.pop(0))]
    for child_number in number:
        temp_node.child.append(nodes[int(child_number)])
print(fptr(start_node))

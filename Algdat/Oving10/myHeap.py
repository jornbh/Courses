from sys import stdin, stderr
import queue
Inf = 1e3000
currentPosition = []
class Node:
    def __init__(self, id, currentCost):
        self.id = id
        self.currentCost = currentCost

    def __lt__(self, rhs):
        return self.currentCost < rhs.currentCost
    def __gt__(self, rhs):
        return self.currentCost > rhs.currentCost
    def __str__(self):
        i = str(self.id)
        c = str(self.currentCost)
        return "("+i+" "+c+ ") "

def fixHeapDown(heap, index):
    global currentPosition
    halfLength = len(heap)//2
    while index< halfLength:
        leftChildIndex = index*2
        rightChildIndex = leftChildIndex+1
        if heap[leftChildIndex] < heap[rightChildIndex]:
            swapIndex = leftChildIndex
        else:
            swapIndex = rightChildIndex
        if heap[swapIndex] < heap[index]:
            heap[index], heap[swapIndex] = heap[swapIndex], heap[index]
            currentPosition[heap[index].id ] = index
            index = swapIndex
        else:
            currentPosition[heap[index].id] = index
            return
    currentPosition[heap[index].id] = index


def fixHeapUp(heap, index):
    global currentPosition

    while index >1:
        parentIndex = index//2
        if heap[parentIndex] > heap[index]:
            heap[parentIndex], heap[index] = heap[index], heap[parentIndex]
            currentPosition[heap[index].id] = index
            index = parentIndex
        else:
            currentPosition[heap[index].id] = index
            return
    currentPosition[heap[1].id] = 1
def popHeap(heap):
    if len(heap)==2:
        return heap.pop()

    output = heap[1]
    heap[1] = heap.pop()
    fixHeapDown(heap, 1)
    return output
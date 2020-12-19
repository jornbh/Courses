import Oving10
import tull
import makeInput
from sys import stdin

def main():
    n = int(stdin.readline())
    probabilities = [float(x) for x in stdin.readline().split()]
    neighbour_matrix = []
    neighboursList=[]
    for line in stdin:
        neighbour_row = [0] * n
        neighbours = [int(neighbour) for neighbour in line.split()]
        for neighbour in neighbours:
            neighbour_row[neighbour] = 1
        neighbour_matrix.append(neighbour_row)
        neighboursList.append(neighbours)


    print(tull.best_path(neighboursList, probabilities))
    print(Oving10.best_path(neighbour_matrix, probabilities))
    makeInput.main()

main()
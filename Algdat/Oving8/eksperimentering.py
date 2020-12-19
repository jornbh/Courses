from sys import stdin


def main():
    M  ={}

    trash_mode = stdin.readline().strip()
    trash_number_og_nodes = stdin.readline()

        
    start = stdin.readline()[:-1]
    ratatosk = stdin.readline()[:-1]
    currentNeeded = ratatosk
    parents = set()
    level = 0
    for i in stdin:
        lineList = i[:-1].split()
        parent = lineList[0]
        parents.add(parent)
        if currentNeeded in lineList[1:]:
            currentNeeded = parent
            level+=1
            if start in parents:
                try:
                    while True:
                        currentNeeded = M[currentNeeded]
                        level +=1
                        if currentNeeded == start:
                            print(level)
                            return
                except:
                    continue
            else:
                try:
                    while True:
                        currentNeeded = M[currentNeeded]
                        level+=1
                except:
                    continue
        M.update(dict.fromkeys(lineList[1:], parent))
        # for j in lineList[1:]:
        #     M[j] = parent




    current = currentNeeded
    while current != start:
        level+=1
        current = M[current]
    print(level)


main()
from sys import stdin

def main():

    M  ={}

    stdin.readline().strip()
    trash_number_og_nodes =int( stdin.readline()[:-1])
    
     
    start = stdin.readline()[:-1]
    ratatosk =stdin.readline()[:-1]
    currentNeeded  = ratatosk
    level =0
    foundStart = False
    if trash_number_og_nodes >15:
        try:
            for i in range(7):
                lineList =stdin.readline()[:-1].split()
                M.update(dict.fromkeys(lineList[1:], lineList[0]))
                if start == lineList[0]:
                    foundStart = True
        except:
            while currentNeeded != start:
                currentNeeded= M[currentNeeded]
                level+=1
            print(level)
            return
    for i in stdin:
        lineList = i[:-1].split()
        parent = lineList[0]
        if currentNeeded in lineList[1:]:
            currentNeeded = parent
            level+=1
            if foundStart or parent == start:
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
        if not foundStart:
            if start == parent:
                foundStart = True

    while currentNeeded != start:
        level+=1
        currentNeeded = M[currentNeeded]
        
    print(level)

main()
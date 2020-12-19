from random import randint

def main():
    length = randint(1,20)
    length = 1000
    density = 0/length
    decimalBase = 1000
    file = open("input.txt", "w")

    file.write(str(length)+"\n")

    file.write(str(randint(1,decimalBase)/decimalBase)+" ")
    for i in range(1,length):
        temp = str(round(randint(1,decimalBase))/decimalBase)
        # print(temp, end = " ")
        file.write(temp+ " ")
    file.write("\n")
    dic = dict([(i,set()) for i in range(length)])

    connected = [False for i in range(length)]
    connected[0] = True
    # print()
    for i in range(length):
        numberOfNeighbours = randint(0,round(length*density))
        for j in range(numberOfNeighbours):
            connection = randint(0, length-1)
            while connection in dic[i] and connection == i:
                connection = randint(0, length-1)
            
            dic[i].add(connection)
            # dic[connection].add(i)
            if connected[i]:
                connected[connection] = True
    while connected[-1] == False:
        a = randint(0,length-1)
        b = randint(0, length-1)
        dic[a].add(b)
        # dic[b].add(a)
        if connected[a] or connected[b]:
            connected[a], connected[b] = True, True




    for j in range(length):
        # print(" ".join([str(i) for i in dic[j]]))
        file.write(" ".join([str(i) for i in dic[j]])+ "\n")
        
    file.close()




if __name__ == '__main__':
    main()
from sys import stdin
def makeMatrix(h,l,v, lH,lW):
    M = [[0 for i in range(lW+1)] for j in range(lW+1)]
    BeginningPoints =[lW for i in range(lW+1)]
    for i in range(len(v)):
        he, le, va = h[i],l[i],v[i]
        if( he<= lH and le <= lW and M[he][le] < va):
            M[he][le] = va
            M[le][he] = va
            # add new lower limits for the program
            if BeginningPoints[le] > he:
                BeginningPoints[le] = he
            if BeginningPoints[he] > le:
                BeginningPoints[he] = le
    for i in range(1,len(BeginningPoints)):
        if (BeginningPoints[i] > BeginningPoints[i-1]):
            BeginningPoints[i] = BeginningPoints[i-1]
    return M, BeginningPoints


def sorter(h,l,v, lH, lW, minSquare, upperLimits):
    M, BeginningPoints= makeMatrix(h,l,v, lH,lW)
    #print(BeginningPoints)
    # Fix inner square
    for i in range(minSquare,lH+1):
        row = M[i]

        for j in range(i,upperLimits[i]+1):
            # Optimize row
            ma = row[j]
            for q in range(BeginningPoints[j],(j+2)//2):
                if (ma < row[q]+ row[j-q]):
                    ma =  row[q]+ row[j-q]
            if row[j-1] > ma:
                ma = row[j-1]
            row[j] = ma
            M[j][i] = ma # Can be done by mirroring
        
            # Optimize collumn 
            col = M[j] # By mirroring row i is equivalent to col i
            ma = col[i]
            for q in range(BeginningPoints[i], (i+2)//2):
                if(ma < col[q]+ col[i-q]):
                    ma = col[q] + col[i-q]
            if ma < col[i-1]:
                ma = col[i-1]
            col[i] = ma
            col[i] = max(ma, M[j][i])# col[i] === M[j][i]
            M[i][j]= max(ma, M[i][j]) 

    return M




# COPYPASTE
def main():
    widths = []
    heights = []
    values = []
    for triple in stdin.readline().split():
        dim_value = triple.split(':', 1)
        dim = dim_value[0].split('x', 1)
        widths.append(int(dim[0][1:]))
        heights.append(int(dim[1][:-1]))
        values.append( int(dim_value[1]))



    #legg alle elementene i en liste of finn max i hver retning + sorter alt til at det peker riktig retning

    minSquare = min(min(widths), min(heights))
    liste =[]
    upperLimits = [0 for i in range(150)]
    max_height, max_width = 0,0
    for line in stdin:
        a,b = ([int(x) for x in line.split('x', 1)])
        if a>b:
            a,b = b,a
        upperLimits[a] =b
        if max_height< a:
            max_height = a
        if max_width < b:
            max_width = b
        liste.append([a,b])
    upperLimits[max_height]= max_width
    for i in range(max_height, 0 , -1):
        if upperLimits[i-1] < upperLimits[i]:
            upperLimits[i-1] = upperLimits[i]
            
 
    M = sorter(widths, heights, values, max_height, max_width, minSquare, upperLimits)
    output ="\n".join([str(M[i[0]][i[1]]) for i in liste])
    print(output)

main()
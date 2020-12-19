from sys import stdin
def makeMatrix(h,l,v, lH,lW):
    M = [[0 for i in range(lW+1)] for j in range(lH+1)]

    for i in range(len(v)):
        he, le, va = h[i],l[i],v[i]
        if( he<= lH and le <= lW):
            if( M[he][le] < va):
                M[he][le] = va
        if(le <= lH and he <= lW):
            if ( M[le][he] < va):
                M[le][he] = va
    return M


def sorter(h,l,v, lH, lW, minSquare):
    M = makeMatrix(h,l,v, lH,lW)

    # Fix inner square
    for i in range(minSquare,lH+1):
        row = M[i]
        for j in range(i,lH+1):
            # Optimize row
            ma = row[j]
            for q in range(minSquare,(j+2)//2):
                if (ma < row[q]+ row[j-q]):
                    ma =  row[q]+ row[j-q]
            if row[j-1] > ma:
                ma = row[j-1]
            row[j] = ma
            M[j][i] = ma # Can be done by mirroring
        
            # Optimize collumn 
            col = M[j] # By mirroring row i is equivalent to col i
            ma = col[i]
            for q in range(minSquare, (i+2)//2):
                if(ma < col[q]+ col[i-q]):
                    ma = col[q] + col[i-q]
            if ma < col[i-1]:
                ma = col[i-1]
            col[i] = ma
            col[i] = max(ma, M[j][i])# col[i] === M[j][i]
            M[i][j]= max(ma, M[i][j]) 


    #Fix outer square
    for i in range(minSquare, lH+1):
        row = M[i] # Prepare row to get less indeirect accessing
        for j in range(lH+1, lW+1):
            
            # Optimize row
            ma = row[j]
            for q in range(minSquare,1+j//2):
                if ma < row[q]+ row[j-q] :
                    ma =  row[q]+ row[j-q]
            if ma < row[j-1]:
                ma = row[j-1]
            row[j] = ma
        
            # Optimize collumn 
            #YOLO- kommenteringen virker i dette oppgavesettet
            # ma = M[i][j]
            # for q in range(minSquare,1+i//2):
                
            #     if ma < M[q][j] + M[i-q][j]:
            #         ma =  M[q][j] + M[i-q][j]
            # if ma < M[i-1][j]:
            #     ma = M[i-1][j]
            # M[i][j] = ma

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
    max_height, max_width = 0,0
    for line in stdin:
        a,b = ([int(x) for x in line.split('x', 1)])
        if a>b:
            a,b = b,a
        if max_height< a:
            max_height = a
        if max_width < b:
            max_width = b
        liste.append([a,b])


    M = sorter(widths, heights, values, max_height, max_width, minSquare)
    for i in liste:
        print(M[i[0]][i[1]])


main()
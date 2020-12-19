from sys import stdin
def maximizeRow(rowNo, FinallCol, M):
    row = M[rowNo]
    maximum = row[FinallCol]


    for i in range(1, FinallCol//2+2):
        if row[i] + row[FinallCol- i] > maximum:
            maximum= row[i] + row[FinallCol- i]

    row[FinallCol]= maximum

def maximizeCol( FinallRow, colNo, M):

    maximum  = M[FinallRow][colNo]

    for i in range(1,FinallRow//2 +2):
        if M[i][colNo] + M[FinallRow-i][colNo] > maximum:
            maximum =M[i][colNo] + M[FinallRow-i][colNo]

    M[FinallRow][colNo] = maximum 
            



def insertValues(M, values, heights, widths):
    elementNo= len(values)
    i =0
    firstRow =len(M)-1
    while i< elementNo:
        try:
            while i< elementNo:
                M[heights[i]][widths[i]]= max(values[i], M[heights[i]][widths[i]])
                
                if firstRow> heights[i]:
                    firstRow = heights[i]
                i+=1
        except:
            i+=1
    i=0
    while i< elementNo:
        try:
            while i< elementNo:
                M[widths[i]][heights[i]]= max(values[i], M[widths[i]][heights[i]])
                
                if firstRow > widths[i]:
                    firstRow = widths[i]
                i+=1
        except:
            i+=1
        

    return firstRow




def max_value(widths, heights, values, paper_width, paper_height, M):
    if paper_height > paper_width:
        paper_height, paper_width = paper_width, paper_height
    M = [[0 for i in range(paper_width+1)] for j in range(paper_height+1)]
    firstRow = insertValues(M, values, heights, widths)
    
    for i in range(firstRow,paper_height+1):
        for j in range(1, paper_width+1):
            maximizeCol(i,j,M)
            maximizeRow(i,j,M)
    return M[-1][-1]






def main():
    widths = []
    heights = []
    values = []
    for triple in stdin.readline().split():
        dim_value = triple.split(':', 1)
        dim = dim_value[0].split('x', 1)
        width = int(dim[0][1:])
        height = int(dim[1][:-1])
        value = int(dim_value[1])
        widths.append(int(width))
        heights.append(int(height))
        values.append(int(value))

    for line in stdin:
        paper_width, paper_height = [int(x) for x in line.split('x', 1)]
        print((max_value(widths, heights, values, paper_width, paper_height)))

if __name__ == '__main__':
    main()
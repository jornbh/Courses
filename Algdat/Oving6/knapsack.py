# def main():
#     totW = 740
#     v = [18,27,51,36,24,22,29,10,24,40]
#     w = [320,301, 371, 296, 303,359, 148, 275,296, 395]
#     # v =[1,4,5,7]
#     # w = [1,3,4,5]
#     # totW = 7

#     fractions = [(v[i],w[i]) for i in range(len(v))]
#     fractions.sort(key = lambda element: element[1] )


    
#     solutionMatrix = [[0 for i in range(totW+1)]for j in range(len(fractions))]
#     solve(solutionMatrix, fractions, totW+1)

# def solve(M, items, totW):

#     for i in range(totW):
#         if items[0][1] > i:
#             M[0][i] = 0
#         else:
#             M[0][i] = items[0][0]
#     for i in range(1,len(M)):
#         for j in range(totW):
#             if (items[i][1]> j):
#                 M[i][j] = M[i-1][j]
#             else:
#                 M[i][j] = max(M[i-1][j], items[i][0] + M[i-1][j-items[i][1]])


#     # for i in M: 
#     #     print(i)
#     print(M[-1][-1])

# main()


for i in range(10,1):
    print(i)
print(".-.")
from sys import stdin

d = int(stdin.readline())
strings =[ [q-97 for q in i.rstrip().encode("utf-8")] for i in stdin]
end = len(strings)

def lengdesortering():
    global strings
    LC = [0]*(d+1)
    for i in strings:
        LC[len(i)] +=1
    
    for i in range(1,d+1):
        LC[i] += LC[i-1]
    temp = strings[:]
    for i in range(end-1,-1,-1):
        LC[len(temp[i])]-=1
        strings[LC[len(temp[i])]]= temp[i]
    return LC[1:]


def countingSort(index, beginning):
    global strings
    CC = [0]*26
 
    for i in strings[beginning:]:
        CC[i[index]] +=1
    for i in range(1,26):
        CC[i] +=CC[i-1]
    temp =strings[:]
    for i in reversed(temp[beginning:]):
        CC[i[index]] -=1
        strings[beginning+ CC[i[index]]]= i


limits = lengdesortering()
index = d-1

for i in strings:

    pass

for i in reversed(limits):
    countingSort(index, i)
    index-=1
out ="\n".join( [bytes([i+97 for i in q]).decode() for q in strings])
print(out)

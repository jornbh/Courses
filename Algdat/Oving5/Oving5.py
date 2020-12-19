from sys import stdin
class Node:
    posi=[]
    children={}
    def __init__(self):
        self.posi = []
        self.children ={}


    def addChild(self, string,currentLetterIndex, position_in_text):
        if currentLetterIndex== len(string):
            self.posi.append(position_in_text)
            return
        letter = string[currentLetterIndex]
        if not letter in self.children:
            self.children[letter] = Node()
        self.children[letter].addChild(string, currentLetterIndex+1, position_in_text)

    def find_positions_of_string(self,string, index):
        if(index >= len(string)):
            return self.posi
        letter = string[index]
        if(letter =="?"):
            out =[]
            for i in self.children:
                out+=self.children[i].find_positions_of_string(string, index+1)

            return out
        else:
            if letter in self.children:
                return self.children[letter].find_positions_of_string(string, index+1)
            else:
                return []




def build(textVector):
    head ={}
    position_in_text =0
    for i in textVector:
        if i[0] not in head:
            head[i[0]] = Node()
        head[i[0]].addChild(i,1,position_in_text)
        position_in_text +=len(i)+1
    return head


def insertion_sort(L):
    for i in range(len(L)):
        for j in range(i,0,-1):
            if L[j]<L[j-1] :
                L[j-1],L[j] =L[j],L[j-1]
            else:
                break

def main():
    basis = stdin.readline().split()
    strings = [i.rstrip() for i in stdin]
    head = build(basis)
    for i in strings:
        print(i, end =": ")
        if i[0] =="?":
            liste =[]
            if len(i) ==1:
                tempListe =[]
                for key in head:
                     tempListe+= head[key].posi
                liste = [str(position) for position in tempListe]
            else:                          
                for j in head:
                    liste+= [str(q) for q in head[j].find_positions_of_string(i,1)]
        else:
            liste =[str(q) for q in head[i[0]].find_positions_of_string(i,1)]
        insertion_sort(liste)
        print(" ".join(liste))
main()

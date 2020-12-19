#Virker

from sys import stdin
class Node:
    posi=[]
    children={}
    def __init__(self):
        self.posi = []
        self.children ={}

    def add_child(self,string, letter_index, position_in_text):
        if(letter_index == len(string)):
            self.posi.append(position_in_text)
            return
        letter = string[letter_index]
        if letter not in self.children:
            self.children[letter] = Node()
        self.children[letter].add_child(string,letter_index+1, position_in_text)

    def find_positions(self,string, letter_index):
        if letter_index == len(string):
            return self.posi

        letter = string[letter_index]
        if letter =="?":
            output = []
            for i in self.children:
                output+= self.children[i].find_positions(string,letter_index+1)
            return output
        if letter in self.children:
            return self.children[letter].find_positions(string, letter_index+1)
        else:
            return []
        

    

def insertionSort(L):
    for i in range(len(L)):
        for j in range(i,0,-1):
            if L[j] <L[j-1]:
                L[j], L[j-1] = L[j-1], L[j]
            else:
                break


def build(string_list):
    head = Node()
    position_in_text=0
    for string in string_list:
        head.add_child(string, 0,position_in_text )
        position_in_text+= len(string)+1
    return head



def main():
    base = stdin.readline().split()


    head = build(base)
    strings =[i.rstrip() for i in stdin]
    for i in strings:
        L = head.find_positions(i, 0)
        insertionSort(L)
        print(i,": ", " ".join([str(i) for i in L]), sep="")


main()
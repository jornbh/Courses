from sys import stdin
import binascii

def main():
    basis = stdin.readline().split()
    strings ={}
    for i in basis:
        length = len(i)
        if length not in basis:
            strings[length]=[]
        strings[length].append(i)
    for i in strings:
        pass

    strings = [i.rstrip() for i in stdin]


string = "aaaaaaaaaa"
ints  = [i-97 for i in string.encode("utf-8")]

print(type(string.encode("utf-8")))


print(ints)


from sys import stdin

lines = "".join([ i for i in stdin if "inet addr:" in i])
# print(lines)
parts = lines.split("inet addr:")[-1].strip()
# print(parts)
address= parts.split()[0]
print(address)
# last = lines[-1].strip().split()
# relevants = [ i for i in last if "addr" in i]




from sys import stdin
from random import randint




















def min_coins_greedy(coins, value):
    noCoins =0
    i =0
    while value:
        noCoins += value// coins[i]
        value = value % coins[i]
        i+=1
    return noCoins

# hello
def min_coins_dynamic(coins, highestValue):
    L = [i for i in range(highestValue +1)]
    for c in coins:
        for displacement in range(c):
            if displacement >= len(L):
                continue
            val = L[displacement]
            for jump in range( 1+(len(L)-1 - displacement)//c):
                pos = jump*c + displacement
                if L[pos] > val:
                    L[pos] = val
                else:
                    val = L[pos]
                val +=1

    return L
    


def can_use_greedy(coins):

    for i in range(1, len(coins)):
        if coins[i-1] % coins[i]:
            return False
    return True


coins = []
for c in stdin.readline().split():
    coins.append(int(c))
coins.sort()
coins.reverse()
method = stdin.readline().strip()
if method == "graadig" or (method == "velg" and can_use_greedy(coins)):
    for line in stdin:
        print(min_coins_greedy(coins, int(line)))
else:
    vals = [int(line) for line in stdin]
    L = min_coins_dynamic(coins, max(vals))
    for i in vals:
        print(L[i])
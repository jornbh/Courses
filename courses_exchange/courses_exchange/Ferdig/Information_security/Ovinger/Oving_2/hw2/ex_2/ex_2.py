import requests
import hashlib
import random
import binascii
import math

# Websocket
import asyncio
import websockets




# Functions 

def I2B(i):
    return i.to_bytes( (i.bit_length() +7) // 8, 'big')
def I2NB(i):

    buf = i.to_bytes( (i.bit_length() +7) // 8, 'big')
    strToSend = binascii.hexlify(buf).decode("utf-8")
    return strToSend

def NB2I(msg_received):
    buf_B = binascii.unhexlify(msg_received)
    i = B2I(buf_B)
    return i

def B2I(i):
    return int.from_bytes(i, "big")
def B2NB(i):
    return binascii.hexlify(i).decode("utf-8")
def S2NB(i):
    return i.encode("utf-8")
############################################################

#  Global constants
####################################################
mail_S = "jorn.hofstad@epfl.ch"

# HASH CONSTANTS
U_B =  b"jorn.hofstad@epfl.ch" 
# @doc B -> B
def H(x_B): # bytes -> bytes 
    hash_B = hashlib.sha256(x_B).hexdigest()
    return hash_B
    # return binascii.unhexlify(  )

N_I =   0xEEAF0AB9ADB38DD69C33F80AFA8FC5E86072618775FF3C0B9EA2314C9C256576D674DF7496EA81D3383B4813D692C6E0E0D5D8E250B98BE48E495C1D6089DAD15DC7D7B46154D6B6CE8EF4AD69B15D4982559B297BCF1885C529F566660E57EC68EDBC3C05726CC02FD4CBF4976EAA9AFD5138FE8376435B9FC61D2FC0EB06E3
g_I = 2
p_B = b"JAoEC1xIHAMdEEEFYA0FCw1AQxw=" #! PASSWORD 
TO_URL_S = "ws://com402.epfl.ch/hw2/ws"
# TO_URL_S = "ws://localhost:8765"

####################################################





# Program flow # Evereything is represented as numbers or strings on this level
async def solve():
    async with websockets.connect(TO_URL_S) as websocket:


        await websocket.send(S2NB(mail_S))                      #? Send U

        salt_B = binascii.unhexlify(await websocket.recv())
        # salt_B   = I2B(salt_I) 
        # salt_I  = NB2I(salt_NB)

        a_I     = random.getrandbits(32)                        #! a = random(32)

        A_I     = pow(g_I , a_I, N_I)                           #! A = g^a % N 
        A_NB    = I2NB(A_I)
        await websocket.send( A_NB)                             #? Send A




        
        B_NB = await websocket.recv()                           #? Recieve B
        B_I = NB2I(B_NB)
        A_B = I2B(A_I)
        B_B = I2B(B_I) 
     
        
        u_S =  H(  A_B + B_B   )                                #! u = H( A || B ) 
        u_I = int(u_S, 16)
                

        
        
        inner_hash_S = H( U_B + b":" + p_B) 
        x_S =  H( salt_B + binascii.unhexlify(inner_hash_S))                        #! x = H(salt || H( U || ":" || password)) 
        x_I = int(x_S, 16)


        first_I = ( B_I - pow(g_I,x_I, N_I)) % N_I
        S_I = pow( first_I, (a_I+u_I*x_I), N_I)                 #! (B - g^x)^(a+ u*x) % N

        S_B = I2B( S_I)
        
        hash_input_B = A_B + B_B + S_B
        final_hash_S = H(hash_input_B)


        final_hash_NB = final_hash_S.encode("utf-8")
        await websocket.send(final_hash_NB)

        response_NB = await websocket.recv()
        print("THE RESPONE IS\n",response_NB)


if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(
    solve())


    print("DONE")





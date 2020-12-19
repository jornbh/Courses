import requests
import hashlib
import random
import binascii
import math

# Websocket
import asyncio
import websockets



# N ::Int 
# g :: Int






async def solve():
    
    #send mail                              :: str          -> n_bytes
    # get salt                              :: n_bytes      -> int
    # calc a : a = random(1,2**32 -1)       :: int
    # calc A  (g,a,N)                       :: (bytes)³     -> bytes                      
    # send A                                :: bytes        -> n_bytes               
    # recv B                                :: n_bytes      -> bytes                
    # calc u = H(A || B)                    :: bytes        -> bytes                            
    # calc x = H( salt || H(U || ":" || pass))                               ::                

    # calc S =                                               

    # calc H( A || B || S)                                              
    # send S                                                
    # recv response                                                 
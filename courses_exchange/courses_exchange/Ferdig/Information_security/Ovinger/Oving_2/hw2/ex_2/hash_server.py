#!/usr/bin/env python

# WS server example
import ex_2 as F
import asyncio
import websockets

async def hello(websocket, path):
    
    name = await websocket.recv()

    print(f"< {name}")

    greeting = f"Hello {name}!"
    salt = F.I2NB(0)
    await websocket.send(salt)

    name = await websocket.recv()

    print(f"> {greeting}")

start_server = websockets.serve(hello, 'localhost', 8765)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
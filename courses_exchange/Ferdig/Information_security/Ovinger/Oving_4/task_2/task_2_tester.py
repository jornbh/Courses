import requests


base_address ="http://127.0.0.1:5000"

password_address = '/hw4/ex2'

my_json = {"user": "hello", "pass" : " my darling"}


session = requests.Session()

reply = session.post( base_address+ password_address, json=my_json )
print(reply.text)
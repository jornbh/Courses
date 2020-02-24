import requests
from arepldump import dump
def main_test():
    base_address ="http://127.0.0.1:80/"
    post_address = "messages"
    get_address = 'users'

    post_json  = {"name": "malden"}
    get_jsons_list = [ 
                        # {"limit": 3},
                        {},
                        {"limit":0},
                        {"limit": -1},
                        { "limit": "ab;a"} ,
                        {"limit": 1},
                        {"limit":3},
                        {"limit":"4"}, 
                        

                    ] #?


    session = requests.Session()

    reply = session.post( base_address+post_address, json=post_json  )

    replies = [
                session.get( base_address+ get_address, json=get_json  )
                for get_json in get_jsons_list
            ] #?
    
    texts = []
    for i,j in zip(replies, get_jsons_list):
        
        texts.append(i.text)
    # texts = [( i.text)  if "limit" in j]
    for sendt, reply in zip( get_jsons_list,replies):
        print(sendt)
        print( reply.text) 

    print("number of replies:", (replies))
    # dump()
    # dump(texts)
    # print(replies)


    

if __name__ == "__main__":
    import time
    while True: 
        try: 
            main_test()
            print("OK")
        except:
            print("crash")
        time.sleep(3)        
main_test()
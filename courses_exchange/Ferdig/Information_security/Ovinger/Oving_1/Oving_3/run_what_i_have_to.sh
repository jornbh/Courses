
# sudo apt install python3-dev
# sudo apt install libcap-dev
# sudo apt install libnfnetlink-dev
# sudo apt install libnetfilter-queue-dev


bash run_docker.sh jorn.hofstad@epfl.ch

docker exec attacker /bin/bash -c 'iptables -t nat -A POSTROUTING -j MASQUERADE'
docker exec attacker /bin/bash -c 'iptables -A FORWARD -s 172.16.0.2 -p tcp --dport 80 -j NFQUEUE --queue-num 0'

# docker exec -it attacker /bin/bash
# To run inteceptor.py from on the docker-machine
docker exec -it attacker python3 shared/interceptor.py
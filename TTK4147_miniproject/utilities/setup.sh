
echo "git pull"
git pull
echo ""

Inet_Address=$( ifconfig | grep  addr | python2 getInet.py)
# Inet_Address=${GET_Inet_Address}
echo "IP address = ${Inet_Address} (Already changed in ../src/main.c)"
echo ""
echo ${Inet_Address} | python2 insertIP.py

rm ../build/CMakeCache.txt

echo "press CTR A and Z "
echo "Press ctrl o and set the serial prot to 115200 "
echo "sudo minicom -D /dev/ttyS0"



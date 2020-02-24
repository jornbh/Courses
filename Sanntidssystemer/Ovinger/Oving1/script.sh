
for i in 1.1 1.3 1.5 2 3 4 5 7
do
    python3 makeCmake.py ${i}  > CMakeLists.txt
    echo "Factor ${i}"
    cmake -g -fsanatize=address ./ > bin/dummy.txt # Just need somewhere to dump this
    make > bin/dummy.txt
    time ./helloWorld E arbitraryArg
done









#! OLD CODE (ALL IS COMMENTED OUT )

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`


# TASK A
#################################################
echo "Task a"
make
echo "Task A: sleep"
time ./bin/program "A" "sleep"
echo "${green}There is a noticeable difference between real time sys-time and user-time${reset}"

echo "Task A: busy_wait (Provided) "
time ./bin/program "A" "busy_wait"
echo -e "${green} Busy-waiting also affects the user-time (Very bad) ${reset}"
time ./bin/program "A" "busy_wait_times" 10
echo -e "${green} A lot of time is spent doing/waiting for sys-calls (Also very bad) ${reset}"



echo "Script done!!"



# # TASK B
# ####################################################################################################

echo "task B"
make




./helloWorld "B" "Latency_times"          
./helloWorld "B" "Latency_timespec"       
./helloWorld "B" "Resolution_times"       | gnuplot -p -e "plot '<cat' with boxes"
./helloWorld "B" "Resolution_timespec"    | gnuplot -p -e "plot '<cat' with boxes"





# # TASK C
# ####################################################################################################

echo "task C"
make




./helloWorld "C" "Latency_times"          
./helloWorld "C" "Latency_timespec"       
./helloWorld "C" "Resolution_times"       | gnuplot -p -e "plot '<cat' with boxes"
./helloWorld "C" "Resolution_timespec"    | gnuplot -p -e "plot '<cat' with boxes"



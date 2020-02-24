cd ..

#Task 1a
# #* Generate main-file for task 1
# cat UserMadeCode/src/main_task1_a.txt > src/main.cpp


# make all
# D=2
# CUBE_COMMAND="time ./cpurender/cpurender -i input/cubes.obj -o output/cubes.png -w 1920 -h 1080 -d $D"
# OUTPUT_FILE="UserMadeCode/cubeTime.txt"
# echo "Angeling Cubes"                    >  OUTPUT_FILE
# for np in 1 2 3 4 5 10 20 100
# do
#     echo  "mpirun -np ${np} ${CUBE_COMMAND}"              >> OUTPUT_FILE
#     { time mpirun -np $np $CUBE_COMMAND 2>&3; } 3>&2 2>>   OUTPUT_FILE  #? How to pipe this to tee
#     echo ""
# done 

# # time ./cpurender/cpurender -i input/audir8.obj -o output/audir8.png -w 1920 -h 1080 -d $D | tee ./UserMadeCode/audi.txt


#Task 2

FromDict="./UserMadeCode/src/"
ToDict="./src/"

# main_task1_a.txt  main_task1_b.txt  main_task1_c.txt  main_task2.txt  

# rasteriser_task1.txt  rasteriser_task2.txt

#Task 1 a 
cat "${FromDict}main_task1_a.txt" > "${ToDict}main.cpp"
cat "${FromDict}rasteriser_task1.txt" > "${ToDict}rasterise.cpp"
cat "${FromDict}rasteriser_hpp_task1.txt" > "${ToDict}rasteriser.hpp"




make all

D=6
CUBE_COMMAND="time ./cpurender/cpurender -i input/cubes.obj -o output/cubes.png -w 1920 -h 1080 -d $D"
echo "Task 1 a  "                    >  OUTPUT_FILE
for np in 1
do
    echo  "mpirun -np ${np} ${CUBE_COMMAND}"              >> OUTPUT_FILE
    # mpirun -np $np $CUBE_COMMAND | tee OUTPUT_FILE
    { time mpirun -np $np $CUBE_COMMAND 2>&3; } 3>&2 2>>   OUTPUT_FILE  #? How to pipe this to tee
    echo ""
done 

#Task 2
cat "${FromDict}main_task2.txt" > "${ToDict}main.cpp"
cat "${FromDict}rasteriser_task2.txt" > "${ToDict}rasterise.cpp"
cat "${FromDict}rasteriser_hpp_task2.txt" > "${ToDict}rasteriser.hpp"

make all





# CUBE_COMMAND="time ./cpurender/cpurender -i input/cubes.obj -o output/cubes.png -w 1920 -h 1080 -d $D"
echo "Task 2 c "                    >>  OUTPUT_FILE
for np in 1
do
    echo  "mpirun -np ${np} ${CUBE_COMMAND}"              >> OUTPUT_FILE
    # mpirun -np $np $CUBE_COMMAND | tee OUTPUT_FILE
    { time mpirun -np $np $CUBE_COMMAND 2>&3; } 3>&2 2>>   OUTPUT_FILE  #? How to pipe this to tee
    echo ""
done 


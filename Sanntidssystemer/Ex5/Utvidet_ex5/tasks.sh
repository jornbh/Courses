#! Implement functionality for sorting into files
# READ_COMMAND="apt update | tee results.txt"
# READ_COMMAND="cat /dev/ttys0 | tee results.txt"

# sudo gnome-terminal -x sh -c "cd Sanntid_oving_4/ ; ${READ_COMMAND} | tee results.txt ; exec bash"

# Task A
cat baseCMakeLists.txt > CMakeLists.txt
echo "add_definitions(-DSLEEP)" >> CMakeLists.txt
cmake ./
make
./Sanntid_oving_4/Sanntid_oving_4


#Task B
cat baseCMakeLists.txt > CMakeLists.txt
echo "add_definitions(-DSINGLE_CPU)" >> CMakeLists.txt
cmake ./
make
./Sanntid_oving_4/Sanntid_oving_4



#Task C
cat baseCMakeLists.txt > CMakeLists.txt
echo "add_definitions(-DSINGLE_CPU)" >> CMakeLists.txt
echo "add_definitions(-DN_THREADS=10)" >> CMakeLists.txt
cmake ./
make
./Sanntid_oving_4/Sanntid_oving_4


#Task D
cat baseCMakeLists.txt > CMakeLists.txt
echo "add_definitions(-DSINGLE_CPU)" >> CMakeLists.txt
echo "add_definitions(-DN_THREADS=10)" >> CMakeLists.txt
echo "add_definitions(-DSLEEP)" >> CMakeLists.txt
cmake ./
make
./Sanntid_oving_4/Sanntid_oving_4


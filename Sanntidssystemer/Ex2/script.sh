
red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
#  #! 1
# echo task A

# clang -lpthread taskAB.c

# ./a.out





# echo "${red}Task B${reset}"

# clang -lpthread taskAB.c -g -fsanitize=thread
# echo "${green} The program will crash during runtime if there is a race-condition, and more than one thread tries to access it ${reset}"
# ./a.out


# #! 2

# echo "${red} Task C ${reset}"
# clang -lpthread taskC.c -g -fsanitize=thread
# time ./a.out
# echo "${green} Everything is fine, Only the first global is 'incorrect' and that is because the other thread has not completed its work yet${reset}"
# clang -lpthread taskAB.c >> dump.txt
# echo "No mutexes"
# time ./a.out
# echo "${green} Without mutexes is way faster${reset}"



# #! 3

# echo "${red} Task D ${reset}"

# echo "The pointer is initialized before the initialization is complete => Several threads 'get' the singleton, even though it is not finished yet"
# echo "It is solved by having a temp-pointer untill the init is complete"


# echo "${red} Task E ${reset}"
# echo "The bug is not visible as a data-race, so -fsanatize will not catch it"


# #! 4
# clang -lpthread taskF.c -g -fsanitize=thread
clang -lpthread taskF_sol.c -g -fsanitize=thread


./a.out

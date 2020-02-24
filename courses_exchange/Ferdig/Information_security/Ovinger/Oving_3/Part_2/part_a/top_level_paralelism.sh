
ssh jornbh@ligin.stud.ntnu.no
cd Documents/Fag/courses_exchange/Information_security/Ovinger/Oving_3/Part_2/



python3 part_1.py 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i'
python3 part_1.py 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r'
python3 part_1.py 's' 't' 'u' 'v' 'w' 'x' 'y' 'z' '0'
python3 part_1.py '1' '2' '3' '4' '5' '6' '7' '8' '9' 




for i in 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 'u' 'v' 'w' 'x' 'y' 'z' '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' 
 do 
    python3 part_1.py $i & 

 done 

 ssh -t jornbh@login.stud.ntnu.no "cd /home/shomed/j/jornbh/courses_exchange/Information_security/Ovinger/Oving_3/Part_2/part_a && git pull && bash start_all.sh"
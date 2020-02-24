import modifyers
import hashlib
import get_words
import time
import sys
class FoundException(ValueError):
    def __init__(self,*args, **kwargs):
        ValueError.__init__(self, *args, **kwargs)


def hash( password ):
    hashed = hashlib.sha256(password.encode()).hexdigest()
    return int(hashed, 16)

def check_passwords( attempt ):
    for i in passwords: 
        if attempt == i: 
            
            return True 
    return False

passwords = [   0xee4fef04b22d4b8ee9b587e741c4f64acdd0d7a4429e6504b36aefe644a047d7,
                0xa1d024fb1de1cdec98a76b93df943d3661b967e0e896753d9bc4d3877bdc2a1c,
                0xfc1906ac475c4531e35cf1d6c2feffe5f526bbf3f42795145925d4f581ea81ef,
                0xadfd309288f9c9facc72628829f57fd5bd3e4aca2b5b595630364eccd5e97e7b,
                0xf4e6db5f7d380e07c939747c0b51829638156e7bd695074e68df1433f6ea57b9,
                0x8333ce3b20b92ad033c741c59f120f6ea94253214d23fa82c0e0cf88786e3d17,
                0xf091222e77b08a2d44edd51bfb1b7e780241f2a93f62b489c8fd1d347cd5567c,
                0xb1cce3373c2170d19b21d4b39db95d3f8294cc2532b9efd263f734f8a346bfaa,
                0xf15919b736e0d6570b234266b2570f238669f913e43db98c36db44b51e877dc1,
                0x44431f43c29f7a2282d0d98a4f6793ef55ba3bef02c7aa1c0cc44e9c1e3a8b27,]

possible_salts = ["47","18","dd","58","d4","ff","fc","da","35","ec",]
# passwords = [hash("iloveyou")]

words = get_words.words
solutions = set()
partition = (0,len(words))
estimated_depth = 1
start_time = time.process_time()

def to_readable(seconds): 
        hours = seconds //3600
        seconds = seconds % 3600

        minutes = seconds // 60 
        seconds = seconds % 60
        out_string = "({},{},{}) ".format(hours, minutes, seconds) 
        return out_string
def estimate_time(counter): 
    branching = len(words)
    total_work = (branching**(estimated_depth+1)  -1)/(branching -1 )
    my_work = total_work* ((partition[1] - partition[0])/ len(words) )
    now = time.process_time()
    used_time = now-start_time
    estimated_work_time =round( (my_work/counter)*used_time)
    print( to_readable(round(used_time)) ,to_readable(estimated_work_time) )
    # print(counter,  my_work )


def make_loop_string( depth):
    
    create_string    =  "current_list =" '[""]*{}'.format(depth)
    looping_list = [
            "for ind_{}, el_{} in enumerate(words[partition[0]:partition[1]]):".format(0,0)
        ]
    line_change=  "current_list[{}] = el_{}".format(0,0)
    looping_list.append("\t"+ line_change)
    
    for i in range(1,depth): 
        line_loop = "for ind_{}, el_{} in enumerate(words):".format(i,i)
        line_change=  "current_list[{}] = el_{}".format(i,i)
        looping_list.append("\t"*i     + line_loop)
        looping_list.append("\t"*(i+1) + line_change)


    looping_text = "\n".join(looping_list)


    execution_string = "\t"*depth + "test_current_string(current_list)"
    
    completed_text = "\n".join([create_string,looping_text, execution_string])
    # print(completed_text)
    return completed_text
    
counter =0
def test_current_string(current_list):
    # print(current_list)
    global counter
    counter +=1
    if counter % 10000 ==0:
        estimate_time(counter)
    current_string = "".join(current_list)

    

    for ind, modifyer_list in enumerate(modifyers.all_uncurried): 
        for i in possible_salts: 
            modified_string = modifyers.execute_curry( modifyer_list, current_string)+i
            test_hash = hash(modified_string)

            if check_passwords(test_hash):
                solutions.add( (modified_string, test_hash))
                print(" GOT IT", modified_string, test_hash)
                write_to_file()
                if len(passwords) == len(solutions): 
                    raise( FoundException("Found everyting") )

    # Save to file
def write_to_file(): 
    f = open(str(partition[0])+ "_of_" +str(partition[1])+"_results.txt", "w")
    for i in solutions:
        cleartext = i[0]
        hashtext = i[1] 
        f.write( "{} \t=>\t {}".format(cleartext, hashtext))
        f.write("\n")
        print(i)
    f.close()

if __name__ == "__main__":

    if len(sys.argv) == 3: 
        n_workers   = int(sys.argv[2]) 
        self_id     = int(sys.argv[1]) 
        
        partition   = ( round(len(words) *  self_id   /n_workers ), 
                        round(len(words) * (self_id+1)/n_workers ) )
        print(partition)
    try:
        depth =0
        while(len(solutions) < len(passwords)): 
            depth +=1
            loop_string = make_loop_string(depth)
            exec(loop_string)
            print(depth)

            break 
    except FoundException as e:
        print(type(e))
        print("Found everything")
        



    print("DONE, the found solutions are", solutions)

    write_to_file()
    print(counter)
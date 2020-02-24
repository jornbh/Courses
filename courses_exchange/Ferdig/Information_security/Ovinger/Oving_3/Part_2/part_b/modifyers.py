import itertools


modifyers = [
    lambda x: x.title(), 
    lambda x: x.replace( "e", "3").replace("E", "3"), 
    lambda x: x.replace( "o", "0").replace("O", "0"), 
    lambda x: x.replace( "i", "1").replace("I", "1"), 

]

# modifyers = [1,2,3,4]
all_uncurried = [None]
all_combinations = [None] # Filled further down in the code



def make_all_combinations_lists(x): 
    if len(x) ==1 : 
        return [x, []]
    head, *tail = x
    later_combos = make_all_combinations_lists(tail)
    headed = [ [head] + i for i in later_combos ]
    return headed + later_combos

def execute_curry(fun_list, arg): 
    # print(fun_list)
    temp = arg
    # print(" Len of fun_list", len(fun_list))
    # print(arg)
    for foo in fun_list:
        temp = foo(temp)
        # print( arg, "=>", temp )
    return temp  

def make_all_combinations(): 

    combination_lists = make_all_combinations_lists(modifyers) 
    
    global all_uncurried
    global all_combinations
    all_uncurried = combination_lists
    all_combinations = [ lambda x : execute_curry(all_uncurried[ind] , x) for ind in range(len(all_uncurried)) ]

    # print(all_uncurried)
    return all_combinations

# test = make_all_combinations_lists([1,2,3,4,5])
# test.sort(key=len)
# print(*test, sep="\n")



all_combinations = make_all_combinations()

if __name__ == "__main__":
    
    for i in all_uncurried: 
        my_str = "eeio1t"
        print(my_str, "=>",  execute_curry(i,  my_str ) )
    
    # print(len(set(all_combinations)))








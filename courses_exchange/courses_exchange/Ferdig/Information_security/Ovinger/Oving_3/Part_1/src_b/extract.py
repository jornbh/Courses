
import re
class entry:
    def __init__(self, input_string): 
        elements = input_string.split()
        # Patterns
        ma_pat = "^([a-z|A-Z|0-9|\.|\@|\-]*) *, "
        mo_pat = '"(.*)"'
        da_pat = "(([0-9]{2}/){2}[0-9]{2}) *,"
        ra_pat = " ([0-9]*)$"

        patterns = [ma_pat, mo_pat,da_pat,ra_pat]
        progs = [ i for i in map(re.compile, patterns)]

        try:
            matches = [i.search(input_string).group(1) for i in progs ]
        except: 
            print("UNABLE TO TRANSLATE INPUT LINE: ")
            print(input_string)
            print("matches are:")
            [print(ind,":",el.search(input_string)) for ind, el in enumerate(progs)]
            quit()


        self.email  =       matches[0].strip() # .strip just in case ... 
        self.movie  =       matches[1].strip()
        self.date   =       matches[2].strip()
        self.rating = int(  matches[3].strip())
    def __str__(self): 
        elements = [
            self.email,
            self.movie,
            self.date,
            self.rating

        ]
        return "e{" + ",".join(map( str, elements)) +"}"



def get_contents(filepath):
    com_file = open(filepath, "r")
    contents = com_file.read()

    # print(contents)
    lines = contents.split("\n")
    entries = [ entry(i) for i in lines ]
    


    com_file.close()
    return entries


            # print(i)
if __name__ == "__main__":

    com_contents =  get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/com402-1.csv")
    imdb_contents = get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/imdb-1.csv")

    solve( com_contents, imdb_contents )

    print("DONE")
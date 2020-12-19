#         RES=1024
#         X=0.5
#         Y=0.5
#         S=1.0
#         I=512


# -i 512
# b 4
# -t (or not)

echo "Task 2 b"> results.txt
for type in "" "-t"
do
    for iterations in 256 512 1024 2048
    do 
        for block_dim in 1 2 3 4 8 16
            do 
                for pixel in 256 512 1024 2048
                do
                    echo "Iteration, blockDim, Pixel" ${iterations} ${block_dim} ${pixel} "Type: ${type}" >>results.txt
                    { time mandel/mandel 1 2> mandel/mandel.stderr ; } 2>> results.txt
                    # {time ./mandel/mandel -i ${iterations} -b ${block_dim} -r ${pixel} 1 ;} 2>> results.txt
                done


            done
         
    done
done 
        # -x [0;1]        Center of Re[-1.5;0.5] (default=0.5)
        # -y [0;1]        Center of Im[-1;1] (default=0.5)
        # -s (0;1]        Inverse scaling factor (default=1)
        # -r [pixel]      Image resolution (default=1024)
        # -i [iterations] Iterations or max dwell (default=512)
        # -c [colours]    colour map iterations (default=1)
        # -b [block dim]  min block dimension for subdivision (default=16)
        # -d [subdivison] subdivision of blocks (default=4)
        # -m      mark Mariani-Silver borders
        # -t      traditional computation (no Mariani-Silver)
FAC=5
HEIGHT=$((1080*${FAC})) # 5 is good, but we will simplt test the logic
WIDTH=$((1920*${FAC}))
# DEPTH=$((5*1))
OUT_FILE="../../fileDump/OUTPUT.txt"
cd ../build
cmake .. &&\
make &&\
echo "ratio: ${HEIGHT}x${WIDTH}" &&\
OUT_PATH="../../fileDump/"
CPU_TIMES="${OUT_PATH}CPU_TIMELOG.txt"
GPU_TIMES="${OUT_PATH}GPU_TIMELOG.txt"
echo "" > CPU_TIMES
echo "" > GPU_TIMES
for DEPTH in 1 2 3 4 5 6
do "GPU: $DEPTH"
    { time ./gpurender/gpurender  --height=${HEIGHT} --width=${WIDTH} --depth=${DEPTH} -g | tee -a ${GPU_TIMES}>&3 ; } 3>&2 2>>${GPU_TIMES}
    mv ../output/sphere.png ${OUT_PATH}GPU.png
done

for DEPTH in 1 2 3 4 5 6
do 
    echo "CPU: $DEPTH"
    { time ./gpurender/gpurender  --height=${HEIGHT} --width=${WIDTH} --depth=${DEPTH}  |tee -a ${CPU_TIMES}  2>&3 ; } 3>&2 2>>${CPU_TIMES}
    mv ../output/sphere.png ${OUT_PATH}CPU.png
done

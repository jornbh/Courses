
FAC=2
HEIGHT=$((1080*${FAC})) # 5 is good, but we will simplt test the logic
WIDTH=$((1920*${FAC}))
DEPTH=$((5*1))
OUT_FILE="../../fileDump/OUTPUT.txt"
cd ../build
cmake .. &&\
make &&\
echo "ratio: ${HEIGHT}x${WIDTH}" &&\

echo "ratio: ${HEIGHT}x${WIDTH}" >> TIME_LOG.txt &&\
# { time ./gpurender/gpurender --height=${HEIGHT} --width=${WIDTH} 2>&3 } 3>&2 2>>TIME_LOG.txt
echo "Output from ratio ${HEIGHT}x${WIDTH}: " | tee  ${OUT_FILE} &&\
 # { cuda-memcheck ./gpurender/gpurender -g --height=${HEIGHT} --width=${WIDTH} --depth=${DEPTH} --enable-gpu 2>&3 ; } 3>&2 2>>TIME_LOG.txt
 { time ./gpurender/gpurender  --height=${HEIGHT} --width=${WIDTH} --depth=${DEPTH} -g 2>&3 ; } 3>&2 2>>TIME_LOG.txt

#time ./gpurender/gpurender --height=${HEIGHT} --width=${WIDTH}

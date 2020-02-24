FAC=1
HEIGHT=$((1080*${FAC})) # 5 is good, but we will simplt test the logic
WIDTH=$((1920*${FAC}))
DEPTH=$((5*1))


cd ../build
cmake .. &&\
make &&\

time ./gpurender/gpurender --depth=${DEPTH} --height=${HEIGHT} --width=${WIDTH}

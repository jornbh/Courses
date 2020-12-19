#!bin/bash
## Moves and renames the currently optimal controller parameters to be the "Committed ones" (They are the defaults that will be used later)
for i in $(ls outputs/best*.mat)
do 
# echo "out: ${i}" 
SHORT_NAME=$(basename ${i})
echo "shorter: $SHORT_NAME"
mv ${i} "required_data/committed_best_params/committed_${SHORT_NAME}"
done
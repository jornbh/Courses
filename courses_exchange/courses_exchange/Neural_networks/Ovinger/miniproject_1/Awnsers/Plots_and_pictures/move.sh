
for file in  */*
do 
	echo "START"
	mv "$file" "${file//[ \':\{\}|()@$]/_,}"
done


from sys import stdin
print "t", "dt", "u"
i =0

lines = [line for line in stdin]
times = [ i.strip().split(":")[1] for i in lines if "dt" in i]
uds = [ i.strip().split(":")[1] for i in lines if "ud" in i]

for i in range(min(len( uds), len(times))):
	print i, times[i], uds[i]
# for line in stdin :
# 	i+=1
# 	if("dt" in line):
# 		print i, line.strip().split(":")[1]

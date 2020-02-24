#!/usr/bin/env python3
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from sys import stdin


for line in stdin:
    if "test" in line and "A" in line:
        break

lines = [ [ int(i) for i in line.split()] for line in stdin]
print(lines)
Mat = np.full( (len(lines), len(max(lines, key=len))), 0)
for i in range(len(lines)):
    for j in range(len(lines[i])):
        Mat[i, j]= lines[i][j]
print(np.transpose(Mat))


# the histogram of the data
n, bins, patches = plt.hist(Mat, 50, normed=0, alpha=0.75)

# add a 'best fit' line
# y = mlab.normpdf( bins, mu, sigma)
# l = plt.plot(bins, y, 'r--', linewidth=1)

plt.xlabel('Delay')
plt.ylabel('Occurences')
# plt.title(r'$\mathrm{Histogram\ of\ IQ:}\ \mu=100,\ \sigma=15$')
# plt.axis([40, 160, 0, 0.03])
plt.grid(True)

plt.show()

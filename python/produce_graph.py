import pdb
import os, sys
import numpy as np
from matplotlib import pyplot as plt
from collections import defaultdict
import argparse
from argparse import ArgumentParser
from math import cos, acos, tan, atan
import json

f = open('obj_output.json', 'r')
lines = f.readlines()

target = "9526"

pose_x = []
pose_y = []
pose_z = []
size_x = []
size_y = []
size_z = []
yaw = []
label = []
score = []

for line in lines:
    line = json.loads(line)
    output = line["output"]
    if target in output:
        target_info = output[target]
        x, y, z, sx, sy, sz, yaw_t, label_t, confidence = target_info.split()
        pose_x.append(float(x))
        pose_y.append(float(y))
        size_x.append(float(sx))
        size_y.append(float(sy))
        size_z.append(float(sz))
        yaw.append(float(yaw_t))
        label.append(int(label_t))
        score.append(float(confidence))

ax1 = plt.subplot2grid(shape=(1,1), loc=(0,0))
plt.title("Position", fontsize=17)
ax1.plot(pose_x, pose_y, color = 'red')
plt.xlabel('X (m)', fontsize=15)
plt.ylabel('Y (m)', fontsize=15)
gca = plt.gca()
gca.set_ylim([-30, 100])
plt.savefig(os.path.join('Position_' + target + '.png'))
plt.show()

ax2 = plt.subplot2grid(shape=(2,1), loc=(0, 0))
plt.title("size_x", fontsize=17)
ax2.scatter(range(len(size_x)), size_x, edgecolors='gray', s=5)
ax2.plot(range(len(size_x)), [np.mean(size_x)] * len(size_x), color='red')
plt.xlabel('frame number', fontsize=13)
plt.ylabel('size (m)', fontsize=13)
gca = plt.gca()
gca.set_ylim([0, 10])

ax3 = plt.subplot2grid(shape=(2,1), loc=(1, 0))

plt.title("size_y", fontsize=17)
ax3.scatter(range(len(size_y)), size_y, edgecolors='gray', s=5)
ax3.plot(range(len(size_y)), [np.mean(size_y)] * len(size_y), color='red')
plt.xlabel('frame number', fontsize=13)
plt.ylabel('size (m)', fontsize=13)
gca = plt.gca()
gca.set_ylim([0, 10])

plt.subplots_adjust(bottom=0.1, 
                    top=0.9, 
                    wspace=0.4, 
                    hspace=1.4)
plt.savefig(os.path.join('size_' + target + '.png'))
plt.show()

ax5 = plt.subplot2grid(shape=(1, 1), loc=(0, 0))
plt.title("yaw", fontsize=17)
ax5.scatter(range(len(yaw)), np.array(yaw) * 180.0 / np.pi, color='red', s=3)
plt.xlabel('frame number', fontsize=13)
plt.ylabel('yaw (deg)', fontsize=13)

gca = plt.gca()
gca.set_ylim([0, 360])
plt.savefig(os.path.join('yaw_' + target + '.png'))
plt.show()





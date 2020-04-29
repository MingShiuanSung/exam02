import matplotlib.pyplot as plt
import numpy as np
import serial
import time



t = np.arange(0,10,0.1) # time vector



serdev = '/dev/ttyACM0'
s = serial.Serial(serdev, 115200)

x_acc = [0.0] * 100
y_acc = [0.0] * 100
z_acc = [0.0] * 100
tilt = [0.0] * 100

for i in range(0, 100):
    line=s.readline() # Read an echo string from K66F terminated with '\n'
    # print line
    x_acc[i] = float(line)

for i in range(0, 100):
    line=s.readline() # Read an echo string from K66F terminated with '\n'
    # print line
    y_acc[i] = float(line)

for i in range(0, 100):
    line=s.readline() # Read an echo string from K66F terminated with '\n'
    # print line
    z_acc[i] = float(line)

for i in range(0, 100):
    line=s.readline() # Read an echo string from K66F terminated with '\n'
    # print line
    tilt[i] = float(line)


# plotting

fig, ax = plt.subplots(2, 1)

ax[0].plot(t, x_acc, color = 'blue', label = 'x')

ax[0].plot(t, y_acc, color = 'red', label = 'y')

ax[0].plot(t, z_acc, color = 'green', label = 'z')

ax[0].set_ylim(-1.25, 1.25)



ax[0].set_xlabel('Time')

ax[0].set_ylabel('Acc Vector')

ax[0].legend(loc = 'lower left')

ax[1].stem(t, tilt)

ax[1].set_xlabel('Time')

ax[1].set_ylabel('displacement')

plt.show()

s.close()
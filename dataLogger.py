import serial
import matplotlib.pyplot as plt

port = "COM3"
baud = 9600
fName = "EMG_Data.csv"
print("Initialising .... ")
ser = serial.Serial(port, baud, timeout = 1)
print("Connected to Arduino port:" + port)
file = open(fName, "a")

print("Data read")

xData = []
yData = []

# plt.close("all")
# plt.figure()
# plt.ion()
# plt.show()

i = 0
with open(fName, "w") as f:
    while(i < 100):
        
        l = str(ser.readline())[2:-5]
        if(i == 0):
            continue
        f.write(str(l) + "\n")
        print(i, ":", l)
        
        line = l.split(",") 

        try:

            x = float(i)
            y = float(line[0])
            xData.append(x)
            yData.append(y)
            
            # plt.cla()
            # plt.plot(y)
            # plt.pause(0.01)
        except:
            pass
        i +=1

#Ensuring equal sizes for collected x and y data
if(len(xData) - len(yData) > 0):
    for i in range(len(xData) - len(yData)):
        yData.append(0)

if(len(xData) - len(yData) < 0):
    for i in range( len(yData)- len(xData) ):
        xData.append(0)

#Displaying collected data
plt.ylim(0, 4095)
plt.plot(xData, yData)
plt.show()



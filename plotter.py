from matplotlib import pyplot as plt

N_xi=20
N_eta=10

infile=open("output.dat","r")

xB=[]
yB=[]

xT=[]
yT=[]

xL=[]
yL=[]

xR=[]
yR=[]

for i in range(0,N_xi+1):
        line=infile.readline().strip().split()
        xB.append(float(line[0]))
        yB.append(float(line[1]))

for i in range(0,N_xi+1):
        line=infile.readline().strip().split()
        xT.append(float(line[0]))
        yT.append(float(line[1]))

for i in range(0,N_eta+1):
        line=infile.readline().strip().split()
        xL.append(float(line[0]))
        yL.append(float(line[1]))

for i in range(0,N_eta+1):
        line=infile.readline().strip().split()
        xR.append(float(line[0]))
        yR.append(float(line[1]))

plt.figure(0)
plt.plot(xB,yB)
plt.plot(xT,yT)
plt.plot(xL,yL)
plt.plot(xR,yR)
plt.show()

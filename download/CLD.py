import skimage.io as io
import time
st=time.clock()
import matplotlib.pyplot as plt
fp=open("md5.txt","r")
done=False
name=fp.readline()
fp.close()
print(name)
img = io.imread(name)
io.imshow(img)
#plt.show()
h=img.shape[0]
w=img.shape[1]
print("h="+(str)(h))
print("w="+(str)(w))
fp=open("CLD.txt","a")
hmax=int(h/8)
wmax=int(w/8)
cnt=0
for si in range(0,h,hmax):
    for sj in range(0,w,wmax):
        Sum = [0, 0, 0]
        for i in range(0,hmax):
            for j in range(0,wmax):
                if(i+si<h and j+sj<w):
                    Sum[0]+=img[i+si,j+sj,0]
                    Sum[1] += img[i+si,j+sj, 1]
                    Sum[2] += img[i+si,j+sj, 2]
        fp.write(str(int(Sum[0]/hmax/wmax))+" "+str(int(Sum[1]/hmax/wmax))+" "+str(int(Sum[2]/hmax/wmax))+" ")
fp.write("\n")
print('Running time: %s Seconds'%(time.clock()-st))
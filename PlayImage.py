import os
import time
import linecache

# src = r'd:\Raiscies\Bad_Apple_Project\HexImageData.txt'
src = r'.\HexImageData.txt'
fps = 30

#test:
sign = True

def maxSpeedPlay():
	for row in range(6574):
		# while row > (time.time() - startTime) * fps : pass
		lines = linecache.getline(src, row).split(' ')
		del lines[-1]
		os.system('cls')
		for line in lines: print('{0:　>32b}'.format(int(line, 16)).replace('0', '　').replace('1', '●'))
			

def FrameUpdate(row):
	str = linecache.getline(src, row)
	if str == '': return False
	
	nums = str.split(' ')
	del nums[-1]
	os.system('cls')
	for r in nums: print('{0:　>32b}'.format(int(r, 16)).replace('0', '　').replace('1', '●'))
		
	return True
	

os.system('echo off')
os.system('cls')
print('Playing Images...')
time.sleep(1)
startTime = time.time()
while int((time.time() - startTime) * fps) == 0: pass

if sign == True:
	maxSpeedPlay()
else:
	while FrameUpdate(int((time.time() - startTime) * fps)): pass

print('Finished. Time length:', end = '')
print(time.time() - startTime)
linecache.clearcache()

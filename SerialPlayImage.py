#------------------Arguments------------------
src	  = r'.\HexImageData.txt'
fps	  = 30

port  = 'COM1'
bps	  = 115200
timex = 2
#---------------------------------------------


from serial.tools import list_ports as serial_list
from serial	import *
from os		import *
from time	import *
from sys 	import exit
import linecache as data


def printPortList():
	lst = serial_list.comports()
	print('|可用串口:', end = '')
	if len(lst) == 0	:print('无')
	else				:print(len(lst))
	for e in lst:
		print('|' + str(e))


def writeFrame(row):
	lines = data.getline(src, row)
	if lines == '': return False
	lines = lines.split(' ')
	del lines[-1]
	
	#send upper screen image data:
	for i in range(16):
		ser.write(bytes.fromhex(lines[i]))
	
	#waiting for sign: 'L'
	while True:
		if ser.read(1) == b'L' : break
	
	#send lower screen image data:
	for i in range(16, 32):
		ser.write(bytes.fromhex(lines[i]))
	
	return True



#main:
system('@echo off')
system('title Serial Matrix Image Player')
system('cls')

printPortList()
print('|')
print('|串口数据流正在启动...')
print('|端口:		' + port)
print('|波特率:	' + str(bps) )
try:
	ser = Serial(port, bps, timeout = timex)
	if not ser.is_open(): 
		raise SerialException
		
except SerialException:
	print('|错误: 无法打开端口: "' + port + '" , 请检查端口名.')
	system('pause > nul')
	exit()
except ValueError:
	print('|错误: 得到非法的参数, 请检查参数.')
	system('pause > nul')
	exit()

print('|数据路径:	' + src)
print('|帧率:		' + str(fps))
if not path.exists(src) or not path.isfile(src) :
	print('|错误: 无法找到数据文件, 请检查路径.')
	system('pause > nul')
	exit()


print('|初始化成功, 敲击任意键开始输出:')
system('pause > nul')
start_ts = time()
while int((time() - start_ts) * fps) == 0: pass

#looping:
while True:
	if ser.read(1) == b'U' and not writeFrame(int( (time() - start_ts) * fps)):
		break
	
data.clearcache()
ser.close()
print('|完成. 总时长: ' + str(int(time() - start_ts )) + '秒.')
system('pause > nul')
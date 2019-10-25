import os
import sys

def get_cpuinfo():#cpu类型和型号
	with open("/proc/cpuinfo","r") as procfile:
		fline=procfile.readlines()
		for line in fline:
			value=line.split(':')
			if(value[0].rstrip()=="model name"):
				print("cpu类型、型号、主频信息为%s"%value[1])
				break

def get_version():#linux内核版本
	with open("/proc/version","r") as procfile:
		value=(procfile.readline()).split()
		print("linux内核版本为%s\n"%value[2])


def get_uptime():#最后一次启动以来已经经历了多长时间，天、小时、分钟
	with open("/proc/uptime","r") as procfile:
		value=int(float((procfile.readline()).split()[0]))
		d=value/86400
		value%=86400
		h=value//3600
		value%=3600
		m=value//60
		print("最后一次启动以来已经经历了%d天%d小时%d分钟\n"%(d,h,m))

def get_stat():#用户态、系统态、空闲态时间、磁盘读写请求次数、上下文转换数、创建了多少进程
	with open("/proc/stat","r") as procfile:
		fline=procfile.readlines()
		value=fline[0].split()
		print("cpu用户态时间:%s"%(value[1]+value[2]))
		print("cpu内核态时间:%s"%(value[3]))
		print("cpu空闲态时间:%s\n"%(value[4]))

		for line in fline:
			value=line.split()
			if(value[0]=="intr"):
				print("磁盘读写请求次数为%s"%value[1])
			elif(value[0]=="ctxt"):
				print("上下文切换时间为%s"%value[1])
			elif(value[0]=="processes"):
				print("创建的进程总数为%s\n"%value[1])
				break
				

def get_meminfo():#配置内存数量，当前可用内存数
	with open("/proc/meminfo","r") as procfile:
		fline=procfile.readlines()
		for line in fline:
			value=line.split(':')
			if(value[0].rstrip()=="MemTotal"):
				print("配置的内存数为%s"%value[1].strip())
			elif(value[0].rstrip()=="MemFree"):
				print("空闲的内存数为%s"%value[1].strip())
			elif(value[0]=="MemAvailable"):
				print("可用的内存数位为%s"%value[1].strip())
				break

def main():
	if(len(sys.argv)>1):
		argv=sys.argv[1]
		for c in argv:
			if c not in {'c','v','u','s','m'}:
				print("参数错误！")
				exit()

		for c in argv:
			if c=='c':
				get_cpuinfo()
			elif c=='v':
				get_version()
			elif c=='u':
				get_uptime()
			elif c=='s':
				get_stat()
			elif c=='m':
				get_meminfo()
	else:
		get_cpuinfo()
		get_version()
		get_uptime()
		get_stat()
		get_meminfo()



if __name__ == '__main__':
	main()

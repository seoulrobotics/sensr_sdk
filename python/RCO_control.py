import sys,os
from time import sleep
import portio
import getopt

def main(argv):

	FILE_NAME = argv[0]
	input_output =""
	Input_output_pin = ""
	voltage = ""

	try:
		opts, etc_args = getopt.getopt(argv[1:],"I:O:tfh" , ["input=","output=","true","false","help"])
	except getop.GetoptError:
		print("\nUsage:\nRCO_DIO_control -I <GPI_IDX> | -O <GPO_IDX> -t | -O <GPO_IDX> -f \n")
		print("    -I <DI_IDX>  DI Idx.  ex. DI1, use 1\n")
		print("    -O <DO_IDX>  DO Idx.  ex. DO2, use 2\n")
		print("    -f Pull DO pin low, -t pull DO pin high\n")
		print("\nExample:\n\tRCO_DIO_control -I 1\tRead from DI1\n")
		print("\tRCO_DIO_control -O 2 -t\tPull DO2 high\n\n")
		sys.exit(2)
	for opt, arg in opts:
		if opt in("-I","--Input"):
			input_output = 1
			Input_output_pin = arg
		elif opt in("-O","--output"):
			input_output = 0
			Input_output_pin = arg
		elif opt in("-t","--true"):
			voltage = 1
		elif opt in("-f","--false"):
			voltage = 0
		elif opt in("-h","--help"):
			print("\nUsage:\nRCO_DIO_control -I <GPI_IDX> | -O <GPO_IDX> -t | -O <GPO_IDX> -f \n");
			print("    -I <DI_IDX>  DI Idx.  ex. DI1, use 1\n")
			print("    -O <DO_IDX>  DO Idx.  ex. DO2, use 2\n")
			print("    -f Pull DO pin low, -t pull DO pin high\n")
			print("\nExample:\n\tRCO_DIO_control -I 1\tRead from DI1\n")
			print("\tRCO_DIO_control -O 2 -t\tPull DO2 high\n\n")
			sys.exit()			
	if os.getuid():
		print('You need to be tooy! Exiting.')
		sys.exit()	

	status = portio.ioperm(0x4E,2,1)
	if(status):
		print('ioperm:',os.strerror(status))
		sys.exit()
	else:
		data = 0
		#### enable config mode
		portio.outb(0x87,0x4E)
		sleep(0.04)
		portio.outb(0x87,0x4E)
		
		#### switch GPIO configuration for SIO LDN 0x06
		portio.outb(0x07,0x4E)
		portio.outb(0x06,0x4F)
		
		##print(type(Input_output_pin))
		if(input_output == 1):
			portio.outb(0x82,0x4E)
			data = portio.inb(0x4F)
			data = data&(1<<int(Input_output_pin)+4-1)
			#print("Get DI%d = \"%s\"\n",Input_output_pin, data?"High":"Low")
		elif(input_output == 0):
			portio.outb(0x89,0x4E)
			data = portio.inb(0x4F)
			if(voltage == 1):
				#print("Set DO%d High\n", Input_output_pin)
				data=data | (1<<int(Input_output_pin)+0 - 1)
			elif(voltage == 0):
				#print("Set DO%d Low\n", Input_output_pin)
				low = 0xff^(1<<int(Input_output_pin)+0 - 1)
				data=data&low
			portio.outb(data,0x4F)
		portio.outb(0xAA,0x4E)
if __name__ == "__main__":
	main(sys.argv)	
				
					

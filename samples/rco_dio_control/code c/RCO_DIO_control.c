#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define AddrPort 0x4E
#define DataPort 0x4F

#define SIO_UNLOCK_VALUE 0x87
#define SIO_LOCK_VALUE 0xAA
#define SIO_LDN_GPIO 0x06

#define WriteByte(port, val)	outb(val, port)
#define ReadByte(port)	inb(port)

#define GPI_ADDR	0x82
#define GPO_ADDR	0x89
#define GPI_BIT_OFFSET	4
#define GPO_BIT_OFFSET	0
#define MAX_PIN		4

#define IN 0
#define OUT 1
#define UNDEFINED -1

void usage(void)
{
	printf("\nUsage:\nRCO_DIO_control -I <GPI_IDX> | -O <GPO_IDX> -t | -O <GPO_IDX> -f \n");
	printf("    -I <DI_IDX>  DI Idx.  ex. DI1, use 1\n");
	printf("    -O <DO_IDX>  DO Idx.  ex. DO2, use 2\n");
        printf("    -f Pull DO pin low, -t pull DO pin high\n");
	printf("\nExample:\n\tRCO_DIO_control -I 1\tRead from DI1\n");
	printf("\tRCO_DIO_control -O 2 -t\tPull DO2 high\n\n");  
	return;  
}

int set_gpio(int direction, int gpio_idx, int value)
{
	unsigned char data;

	if (direction == IN)
	{
		WriteByte(AddrPort, GPI_ADDR);
		data=ReadByte(DataPort);
		data=data&(1<<(gpio_idx+4-1));
		printf("Get DI%d = \"%s\"\n", gpio_idx, data?"High":"Low");
	}
	else
	{
		WriteByte(AddrPort, GPO_ADDR);
		data=ReadByte(DataPort);
		data = 0;
		if (value > 0)
		{
			//pull high
			printf("Set DO%d High\n", gpio_idx);
			data=data|(1<<(gpio_idx+GPO_BIT_OFFSET-1));
		}
		else	
		{
			//pull low
			printf("Set DO%d Low\n", gpio_idx);
			unsigned char low = 0xff^(1<<(gpio_idx+GPO_BIT_OFFSET-1));
			data=data&low;
		}
		WriteByte(DataPort, data);
	}
	
	return 0;
} 


int main(int argc, char *argv[])
{
	if (ioperm(AddrPort, 2, 1))
	{
		printf("Error, unable to access ports\n");
		perror("ioperm"); 
		return -1;
	}
	else{
		WriteByte(AddrPort, SIO_UNLOCK_VALUE);	
		usleep(4000);
		WriteByte(AddrPort, SIO_UNLOCK_VALUE);
		
		WriteByte(AddrPort, 0x07);
		WriteByte(DataPort, SIO_LDN_GPIO);
	}
	int c, value=UNDEFINED, gpio_idx=UNDEFINED;
	int direction=IN;
	while((c = getopt(argc, argv, "I:O:tfh")) != -1) 
	{
		switch(c) 
		{
			case 'I':
				direction = IN;
				gpio_idx=strtoul(optarg, NULL, 0);	
				break;
			case 'O':
				direction = OUT;
				gpio_idx=strtoul(optarg, NULL, 0);	
				break;
			case 't':
				value = 1;
				break;
			case 'f':
				value = 0;
				break;
			case 'h':
				usage();
				return 0;
			default:
				printf("option Invalid: %c\n", (char)c);
				usage();
				return -1;
		}
	}

	if ((gpio_idx == UNDEFINED) || (direction == OUT && value == UNDEFINED))
	{
		printf("Invalid option.\n");
		usage();
		return -1;
	}
	if (gpio_idx < 1 || gpio_idx > MAX_PIN)
	{
		printf("Invalid GPIO pin index.  Index should be in 1~%d range.\n", MAX_PIN);
		return -1;
	}

	set_gpio(direction, gpio_idx, value);
	
	WriteByte(AddrPort, SIO_LOCK_VALUE);

	return 0;
}


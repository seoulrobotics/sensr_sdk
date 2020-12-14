#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>

//
#define FAIL 1
#define SUCCESSFULL 0

// MIC-730AI DI define
#define MIC_730AI_DI0		232
#define MIC_730AI_DI1		233
#define MIC_730AI_DI2		234
#define MIC_730AI_DI3		235
#define MIC_730AI_DI4		236
#define MIC_730AI_DI5		237
#define MIC_730AI_DI6		238
#define MIC_730AI_DI7		239

// MIC-730AI DO define
#define MIC_730AI_DO0		224
#define MIC_730AI_DO1		225
#define MIC_730AI_DO2		226
#define MIC_730AI_DO3		227
#define MIC_730AI_DO4		228
#define MIC_730AI_DO5		229
#define MIC_730AI_DO6		230
#define MIC_730AI_DO7		231


unsigned int gpio_setvalue(int gpio, int value);

unsigned int gpio_getvalue(int gpio, int *value);

unsigned int gpio_setvalue(int gpio, int value)
{
	int fd = -1;
	char cmd[64];
	char dev[128];

	sprintf(cmd, "%d", gpio);
	fd = open("/sys/class/gpio/export", O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device failed!");
		return FAIL;
	}
	write(fd, cmd, sizeof(cmd));
	close(fd);

	sprintf(cmd, "out");
 	sprintf(dev, "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(dev, O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device failed!!");
		return FAIL;
	}
	write(fd, cmd, sizeof(cmd));
	close(fd);

	sprintf(cmd, "%d", value);
 	sprintf(dev, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(dev, O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device failed!!!");
		return FAIL;
	}
	write(fd, cmd, sizeof(cmd));
	close(fd);
	
	sprintf(cmd, "%d", gpio);
	fd = open("/sys/class/gpio/unexport", O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device unexport failed!!!!");
		return FAIL;
	}
	write(fd, cmd, sizeof(cmd));
	close(fd);
	
	return SUCCESSFULL;
}

unsigned int gpio_getvalue(int gpio, int *value)
{
	int ret;
	int fd = -1;
	char cmd[64];
	char dev[128];

	sprintf(cmd, "%d", gpio);
	fd = open("/sys/class/gpio/export", O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device failed!");
		return FAIL;
	}
	write(fd, cmd, sizeof(cmd));
	close(fd);

	sprintf(cmd, "in");
 	sprintf(dev, "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(dev, O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device failed!!");
		return FAIL;
	}
	write(fd, cmd, sizeof(cmd));
	close(fd);

 	sprintf(dev, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(dev, O_RDONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device failed!!!");
		return FAIL;
	}
	ret = read(fd, cmd, sizeof(cmd));
	if (ret < 0)
	{
		perror("read device failed!");
		
		return FAIL;
	}
	else
	{
		*value = atoi(cmd);
	}
	close(fd);
	
	sprintf(cmd, "%d", gpio);
	fd = open("/sys/class/gpio/unexport", O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror("open device unexport failed!!!!");
		return FAIL;
	}
	write(fd, cmd, sizeof(cmd));
	close(fd);
	
	return SUCCESSFULL;
}

int main(int argc, char **argv)
{
	int	chanel, value;
	unsigned int ret;

    if (argc > 1) 
    {
		if (!strncasecmp(argv[1], "-set", 4)) 
		{
      		if (argc < 4)
      		{ 
         		fprintf(stderr, "The input is invalid!\n");
   				return -1;
      		}

      		chanel = atoi(argv[2]);
			if ((chanel > 3) || (chanel < 0))
			{
        		printf("DO chanel is 0-3\n");
        		return -1;
			}
			chanel = MIC_730AI_DO0 + chanel;

   			value = atoi(argv[3]);  	   	
			if ((value > 1) || (value < 0))
			{
        		printf("DO value is 0 or 1\n");
        		return -1;
			}

			// MIC-730AI DO output is Anti-logic
			if(value == 0)
			    ret = gpio_setvalue(chanel, 1);
			else
				ret = gpio_setvalue(chanel, 0);
			
			if(ret == SUCCESSFULL)
			    printf("DO chanel %d is set %d\n", chanel - MIC_730AI_DO0, value);
		} 
		else if (!strncasecmp(argv[1], "-get", 4)) 
		{
      		if (argc < 3)
      		{ 
         		fprintf(stderr, "The input is invalid!\n");
   				return -1;
      		}

      		int chanel = atoi(argv[2]);
			if ((chanel > 3) || (chanel < 0))
			{
        		printf("DI chanel is 0-3\n");
        		return -1;
			}

			chanel = MIC_730AI_DI0 + chanel;
			ret = gpio_getvalue(chanel, &value);
			
			if(ret == SUCCESSFULL)
			    printf("DI chanel %d value is %d\n", chanel - MIC_730AI_DI0, value);
	    }
    } 
    else 
    {
   		printf("Usage: %s [OPTIONS] DI/DO_chanel [value]\n", argv[0]);
   		printf("OPTIONS:\n");
   		printf(" -set: set DO value\n");
   		printf(" -get: get DI value\n");
   		printf("DI/DO_chanel:\n");
   		printf(" DI: 0-7, DO: 0-7\n");
   		printf("value:\n");
   		printf(" 0 or 1, only for DO\n");
   		return -1;
    }

  	return 0;
}

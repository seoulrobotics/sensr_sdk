#include "sensr.h"
#include <iostream>

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

////////////////// operate the gpio //////////////////////
int set_gpio(int direction, int gpio_idx, int value)
{
	unsigned char data;

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

	return 0;
}


int main(int argc, char *argv[])
{
  int c, value=UNDEFINED, gpio_idx=UNDEFINED;
  int direction=IN;
  const char *client_address = "10.10.11.103"; // change your localhost ip
  int change_value = 2;
  if(ioperm(AddrPort, 2, 1)) return 0;
  else{
	WriteByte(AddrPort, SIO_UNLOCK_VALUE);
	usleep(4000);
	WriteByte(AddrPort, SIO_UNLOCK_VALUE);

	WriteByte(AddrPort, 0x07);
	WriteByte(DataPort, SIO_LDN_GPIO);

  }
	
  if (argc > 1)
  {
    client_address = argv[1];
  }
  sensr::Client client(client_address);

  while (true)
  {
    OutputMessage message;
    if (client.ReceiveMessage(message))
    {
      size_t object_points_size = 0;
      int tracked_objects_size = 0, non_tracked_objects_size = 0;
      for(const auto& object : message.objects()) {
        object_points_size += (object.points().length() / (sizeof(float) * 3));
        if (object.has_track() && object.track().tracking_reliable()) {
          tracked_objects_size++;
        } else {
          non_tracked_objects_size++;
        }
      }
    if(tracked_objects_size == 0) value = 0;
    else if(tracked_objects_size >= 1) value =1;
    if(change_value != value){
    	set_gpio(OUT, 1, value);
	change_value = value;
    }
      std::cout << "Message received from SENSR!" << std::endl
                << "Timestamp: " << message.time_stamp().seconds() << " (s) "
                                 << message.time_stamp().nanos() << " (ns)" << std::endl
                << "Ground Points: " << message.ground_points().length() / (sizeof(float) * 3) << std::endl
                << "Object Points: " << object_points_size << std::endl
                << "Tracked Objects: " << tracked_objects_size << std::endl
                << "Non Tracked Objects: " << non_tracked_objects_size << std::endl
                << std::endl;
    }
  }
  WriteByte(AddrPort, SIO_LOCK_VALUE);

}

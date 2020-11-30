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
#include <math.h>
#include <fstream>
#include <unistd.h>

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
	//data = 0;
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
  bool zone_check[4] = {0};
  std::pair<int ,int> zone_info[4];
  std::ifstream readFile;
  char strBuffer[1000];
  char *pstrBuffer = NULL;
  pstrBuffer = getcwd(strBuffer, 1000);
  char *addtext = (char*)malloc(sizeof(char)*20);
  std::strcpy(addtext,"/../zone_info(zone,pin,velocity).txt");
  std::strcat(pstrBuffer,addtext);
  readFile.open(pstrBuffer);

  if(readFile.is_open()){
	while(!readFile.eof()){
		std::string str;
		getline(readFile,str);
		if(str.length() == 0) break;
		char str_array[str.length()];	
		std::strcpy(str_array,str.c_str());
		char *ptr = strtok(str_array," ");
		int i=0;
		int zone_type = atoi(ptr);
		while(ptr != NULL){
			if(i>=2) break;
			ptr = std::strtok(NULL," ");
			if(i==0)zone_info[zone_type].first = atoi(ptr);
			else if(i==1) zone_info[zone_type].second = atoi(ptr);
			i++;		
		}
	}
  }
  int c, value=UNDEFINED, gpio_idx=UNDEFINED;
  int direction=IN;
  const char *client_address = "localhost"; // change your localhost ip
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
      for(int i=0; i<4; i++) zone_check[i] = 0;
      size_t object_points_size = 0;
      int tracked_objects_size = 0, non_tracked_objects_size = 0;
      for(const auto& zone : message.region_of_interest()) {
	for(const auto& object_zone : zone.object_ids()) {
		int cal_velocity = 0;
		for(const auto& object : message.objects()) {
        		if (object.has_track() && object.track().tracking_reliable() && object_zone == object.id()){
          			cal_velocity = sqrt(pow(object.track().velocity().x(),2)+pow(object.track().velocity().y(),2)+pow(object.track().velocity().z(),2));
				if(zone.id() == 0 && cal_velocity > zone_info[0].second) { zone_check[0] = 1;}
				else if(zone.id() == 0 && cal_velocity <= zone_info[0].second) { zone_check[0] = 0;}
				if(zone.id() == 1 && cal_velocity > zone_info[1].second) { zone_check[1] = 1;}
				else if(zone.id() == 1 && cal_velocity <= zone_info[1].second) { zone_check[1] = 0;}
				if(zone.id() == 2 && cal_velocity > zone_info[2].second) { zone_check[2] = 1;}
				else if(zone.id() == 2 && cal_velocity <= zone_info[2].second) { zone_check[2] = 0;}
				if(zone.id() == 3 && cal_velocity > zone_info[3].second) { zone_check[3] = 1;}
				else if(zone.id() == 3 && cal_velocity <= zone_info[3].second) { zone_check[3] = 0;}

        		} else {
          			non_tracked_objects_size++;
        		}
			for(int i=0; i<4; i++) {
				if(zone_info[i].first == 0 && zone_info[i].second == 0) continue; 
				if(zone_check[i]) set_gpio(OUT, zone_info[i].first,0);				
				else if(!zone_check[i]) set_gpio(OUT, zone_info[i].first, 1);
			}
      		}
      	}
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

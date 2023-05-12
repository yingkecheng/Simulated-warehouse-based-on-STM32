#ifndef ESP8266_AT_H
#define ESP8266_AT_H

#include <stdint.h>

typedef struct esp8266_at *esp8266_at_t; 

struct esp8266_at
{
	uint8_t is_ok;
	uint8_t is_rdy;
	uint8_t need_pub;
	int (*config)(esp8266_at_t handle);
};

int esp8266_at_init(void);



#endif /* ESP8266_AT_H */

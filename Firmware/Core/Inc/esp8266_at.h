#ifndef ESP8266_AT_H
#define ESP8266_AT_H

#include <stdint.h>

typedef struct esp8266_at *esp8266_at_t; 

struct esp8266_at
{
	uint8_t is_ok;
	int (*config)(esp8266_at_t handle);
};

int esp8266_at_init(void);

extern struct esp8266_at my_handle;

#endif /* ESP8266_AT_H */

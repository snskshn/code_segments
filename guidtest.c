#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#include <string.h>
// minimum buf size must be 33 (null terminated)
char *guid_ntoi(unsigned char *id, char *buf)
{   
    snprintf(buf, 33, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
	    "%02x%02x%02x", (uint32_t)id[0], (uint32_t)id[1],
	    (uint32_t)id[2], (uint32_t)id[3], (uint32_t)id[4],
	    (uint32_t)id[5], (uint32_t)id[6], (uint32_t)id[7],
	    (uint32_t)id[8], (uint32_t)id[9], (uint32_t)id[10],
	    (uint32_t)id[11], (uint32_t)id[12], (uint32_t)id[13],
	    (uint32_t)id[14], (uint32_t)id[15]); 

    return buf;
}

#if 0
uint16_t *guid_iton(const char *buf, uint16_t *id)
{
    sscanf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
	    "%02x%02x%02x", (uint32_t *)&id[0], (uint32_t *)&id[1],
	    (uint32_t *)&id[2], (uint32_t *)&id[3], (uint32_t *)&id[4],
	    (uint32_t *)&id[5], (uint32_t *)&id[6], (uint32_t *)&id[7],
	    (uint32_t *)&id[8], (uint32_t *)&id[9], (uint32_t *)&id[10],
	    (uint32_t *)&id[11], (uint32_t *)&id[12], (uint32_t *)&id[13],
	    (uint32_t *)&id[14], (uint32_t *)&id[15]);

    return id;
}
#endif

unsigned char *guid_iton(char *buf, unsigned char id[16])
{
    int i;
    char token[3] = {0};

    for(i = 0; i < 32; i += 2) {
	strncpy(token, &buf[i], 2);
	id[i/2] = (unsigned char)strtol(token, NULL, 16);
    }

    return id;
}


int main()
{
    //uint16_t temp[16];
    //char buf[34] = {0};

    //guid_iton("c8ebdfb0b51011d080e500a0c92542e3", temp);
    //printf("%s\n", guid_ntoi(temp, buf));

    char *str = "c8ebdfb0b51011d080e500a0c92542e3";
    char temp[33];
    unsigned char id[16];

    guid_iton(str, id);
    guid_ntoi(id, temp);
    printf("%s\n", temp);


    //printf("min:%ld, max:%ld\n", LONG_MIN, LONG_MAX);

    return 0;
}

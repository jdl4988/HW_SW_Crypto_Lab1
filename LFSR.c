#include <stdio.h>

int main(){	
	uint32_t i;
    //set the inital value
	uint32_t value = 77;
    //set the feedback mask
	uint32_t mask = 0b11100010;

    //loop through the entire set fo registers
	for(i = 0; i < 5; i++){
        //shift the value right once
		uint32_t next = value >> 1;
        uint32_t masked = next;
        //check if the LSB is zero or one
		if(value & 1 == 1){
		    masked = next ^ mask;
		}
        value = masked;
		printf("%d\r\n", value);
	}

    return 0;
}




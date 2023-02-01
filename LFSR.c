#include <stdio.h>

int main(){	
	int i;
    //set the inital value
	int init_value = 77;
    int value = init_value;
    //set the feedback mask
	int mask = 0b11100010;
    int count = 0;
    //loop through the entire set fo registers
	do{
        //shift the value right once
		int next = value >> 1;
        int masked = next;
        //check if the LSB is zero or one
		if(value & 1 == 1){
		    masked = next ^ mask;
		}
        value = masked;
		printf("count %d, register value:%d\r\n", count, value);
        count ++;
	}while(init_value != value);
    printf("Inital Value: %d, Final Value: %d\r\n", init_value, value);
    return 0;
}




#include "ringBuffer.h"
#include <stdio.h>

int main(void)
{
    ringBuffer_t rb;
    char buffer[10];
    ringBuffer_new(&rb, buffer, 10);
    
    if(!ringBuffer_isEmpty(&rb)) {
        goto failure;
    }
    
    ringBuffer_writeChar(&rb, 'A');
    if('A' != ringBuffer_readChar(&rb)) {
        goto failure;
    }
    
    int i = 0;
    ringBuffer_writeChar(&rb, 'A');
    for(i = 0; i < 10; i++) {
        ringBuffer_writeChar(&rb, 'B');
    }
    if('B' != ringBuffer_readChar(&rb)) {
        goto failure;
    }
    
    printf("{Success}\n");
    while(1);

failure:
    printf("{Failure}\n");
    while(1);
}
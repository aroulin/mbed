#include "ringBuffer.h"

void ringBuffer_new(ringBuffer_t* ringBuffer, char* buffer, unsigned int buffer_size)
{
    ringBuffer->buffer = buffer;
    ringBuffer->size = buffer_size;
    ringBuffer->length = 0;
    ringBuffer->start = 0;
    ringBuffer->end = 0;
}

void ringBuffer_writeChar(ringBuffer_t* ringBuffer, char c)
{
    ringBuffer->buffer[ringBuffer->end] = c;
    ringBuffer->end = (ringBuffer->end + 1) % ringBuffer->size;
    
    // If the buffer is full then start must also advance as a character is lost
    if(ringBuffer->length == ringBuffer->size) {
        ringBuffer->start = (ringBuffer->start + 1) % ringBuffer->size;
    } else {
        ringBuffer->length++;
    }
}

char ringBuffer_readChar(ringBuffer_t* ringBuffer)
{
    if(ringBuffer->length > 0) {
        char c = ringBuffer->buffer[ringBuffer->start];
        ringBuffer->start = (ringBuffer->start + 1) % ringBuffer->size;
        return c;
    } else {
        return 0;
    }
}

bool ringBuffer_isEmpty(ringBuffer_t* ringBuffer)
{
    return  ringBuffer->length == 0;
}
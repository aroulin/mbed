#ifndef __RING_BUFFER_API_H__
#define __RING_BUFFER_API_H__

typedef int bool;

typedef struct ringBuffer_t {
    char* buffer;
    unsigned int size;
    unsigned int length;
    unsigned int start;
    unsigned int end;
} ringBuffer_t;

/**
 * Set-up a ring buffer from a given buffer
 * @param ringBuffer the ring buffer to set-up
 * @param buffer the buffer
 * @param buffer_size the size of the buffer
 */
void ringBuffer_new(ringBuffer_t* ringBuffer, char* buffer, unsigned int buffer_size);

/**
 * Write a char in the ring buffer. If the buffer is full, then
 * the oldest characters received will be overwritten.
 * @param ringBuffer the buffer
 * @param c the char
 */
void ringBuffer_writeChar(ringBuffer_t* ringBuffer, char c);

/**
 * Read a char in the ring buffer. The buffer must not be empty.
 * @param ringBuffer the ring buffer
 * @returns the oldest character not read
 */
char ringBuffer_readChar(ringBuffer_t* ringBuffer);

/**
 * Tell if the ring buffer is empty.
 * @param ringBuffer the ring buffer
 * @returns true if buffer is empty, false otherwise
 */
bool ringBuffer_isEmpty(ringBuffer_t* ringBuffer);

#endif

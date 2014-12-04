#include "gsm_api.h"
#include "c027_api.h"
#include "serial_api.h"
#include "wait_api.h"
#include <stdio.h>

static serial_t serial_gsm;

static char* response;
int volatile waitingForResponse;
static volatile uint32_t indexInResp;

static volatile bool expectCR = 0;
static volatile bool expectLF = 0;
static volatile bool expectEndCR = 0;
static volatile bool expectEndLF = 0;

void ISR_serial_gsm(uint32_t id, SerialIrq event)
{
    //TODO precondition gsm should be ON
    if(event != RxIrq) {
        return;
    }
    
    char c = serial_getc(&serial_gsm);
    if(waitingForResponse) {
        if(expectCR && c == '\r') {
            expectCR = 0;
            expectLF = 1;
        }
        
        else if(expectLF) {
            if(c == '\n') {
                expectEndCR = 1;
                expectLF = 0;
            } else if(c != '\r') {
                expectCR = 1;
                expectLF = 0;
            }
        }
        
        else if(expectEndCR) { 
            if(c == '\r') {
                expectEndCR = 0;
                expectEndLF = 1;
            } else {
                response[indexInResp++] = c;
            }
        }
        
        else if(expectEndLF) {
            if(c == '\n') {
                expectEndLF = 0;
                waitingForResponse = 0;
                response[indexInResp] = '\0';
            } else if(c == '\r') {
                response[indexInResp++] = '\r';
            } else {
                expectEndLF = 0;
                expectEndCR = 1;
                response[indexInResp++] = '\r';
                response[indexInResp++] = c;
            }
        }
    }
}

void serial_puts(serial_t *obj, const char* str) {
    while (*str != 0) {
        serial_putc(obj, *str);
        str++;
    }
}

void gsm_powerOn(void)
{
    // TODO precondition: c027 should be initialized
    c027_mdm_powerOn(0);
    serial_init(&serial_gsm, MDMTXD, MDMRXD);
    serial_irq_handler(&serial_gsm, &ISR_serial_gsm, (uint32_t) &serial_gsm);
    serial_irq_set(&serial_gsm, RxIrq, 1);
    
    // Enter command mode
    wait(1);
    serial_puts(&serial_gsm, "+++");
    wait(1);
    
    // disable echo
    serial_puts(&serial_gsm, "ATE0\r");
    wait(1);
    
    // disable flow control
    serial_puts(&serial_gsm, "AT&K0\r");
    wait(1);
}

void gsm_powerOff(void)
{
    c027_mdm_powerOff();
}

void gsm_send(const char* cmd, char* resp)
{
    //TODO precondition gsm should be ON
    printf("DTE: %s\n", cmd);
    waitingForResponse = 1;
    response = resp;
    expectCR = 1;
    indexInResp = 0;

    while(*cmd != 0 && *cmd != '\r') {
        serial_putc(&serial_gsm, *cmd);
        cmd++;
    }
    serial_putc(&serial_gsm, '\r');
    
    while(waitingForResponse);
    printf("DCE: %s\n", resp);
    wait_ms(20);
}
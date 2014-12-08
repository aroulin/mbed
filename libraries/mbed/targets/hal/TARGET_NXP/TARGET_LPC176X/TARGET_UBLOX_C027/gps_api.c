#include "gps_api.h"
#include "c027_api.h"
#include "serial_api.h"
#include "wait_api.h"
#include "ringBuffer.h"

static serial_t serial_gps;
static char buffer[2 * GPS_NMEA_MAX_PKT_SIZE];
static ringBuffer_t ringBuffer;
static volatile bool lineTerminationHasBeenSeen;

bool gps_parse_nmea_packet(gps_pkt_t* pkt);

void ISR_serial_gps(uint32_t id, SerialIrq event)
{
    if(event != RxIrq) {
        return;
    }
    
    char c = serial_getc(&serial_gps);
    ringBuffer_writeChar(&ringBuffer, c);
    if(c == '\n') {
        lineTerminationHasBeenSeen = 1;
    }
}

void gps_powerOn(void)
{
    // TODO assert c027 is ON
    c027_gps_powerOn();
    serial_init(&serial_gps, GPSTXD, GPSRXD);
    serial_irq_handler(&serial_gps, &ISR_serial_gps, (uint32_t) &serial_gps);
    serial_irq_set(&serial_gps, RxIrq, 0);
    
    ringBuffer_new(&ringBuffer, buffer, 2 * GPS_NMEA_MAX_PKT_SIZE);
    
    wait(1);
}

void gps_powerOff(void)
{
    c027_gps_powerOff();
}

void gps_receiveNMEAPacket(gps_pkt_t* pkt)
{
    // TODO assert c027 is ON
    serial_irq_set(&serial_gps, RxIrq, 1);
    while(1) {
        lineTerminationHasBeenSeen = 0;
        while(!lineTerminationHasBeenSeen);
        if(gps_parse_nmea_packet(pkt)) {
            break;
        }
    }
    serial_irq_set(&serial_gps, RxIrq, 0);
}

bool gps_parse_nmea_packet(gps_pkt_t* pkt)
{
    pkt->ID = GPS_ID_GP;
    pkt->type = GPS_PKT_TYPE_RMC;
    pkt->data[0] = 'O';
    pkt->data[1] = 'K';
    pkt->data[2] = '\0';
    
    return 1;
}

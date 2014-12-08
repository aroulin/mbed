#ifndef __GPS_API_H__
#define __GPS_API_H__

typedef int bool;

#define GPS_NMEA_MAX_PKT_SIZE 82

typedef enum gps_pktID_t {
    GPS_ID_GP
} gps_pktID_t;

typedef enum gps_pktType_t {
    GPS_PKT_TYPE_RMC
} gps_pktType_t;

typedef struct gps_pkt_t {
    gps_pktID_t ID;
    gps_pktType_t type;
    char data[GPS_NMEA_MAX_PKT_SIZE];
} gps_pkt_t;

/**
 * Power on the GPS receiver.
 */
void gps_powerOn(void);

/**
 * Power off the GPS receiver.
 */
void gps_powerOn(void);

/**
 * Receive one NMEA packet from the GPS receiver. This function will block
 * until it receives a entire valid packet.
 * @param pkt packet received by the GPS receiver
 */
void gps_receiveNMEAPacket(gps_pkt_t* pkt);

#endif
#ifndef __GSM_API_H__
#define __GSM_API_H__

typedef int bool;

/**
 * Power on the GSM modem. When this function returns, the modem is ready to
 * receive AT-commands.
 */
void gsm_powerOn(void);

/**
 * Power off the GSM modem.
 */
void gsm_powerOff(void);

/**
 * Send one AT-command to the GSM modem. This function is blocking until the
 * GSM modem answers the command response.
 * It should not be used with AT-command that results in unsolicited responses. 
 * @param cmd command to be sent. S3 termination character is optional.
 * @param resp buffer containing the command response
 */
void gsm_send(const char* cmd, char* resp);

#endif
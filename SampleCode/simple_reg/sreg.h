/*
 * ---------------------------------------------
 *  sreg.h -- simple registration header file
 * ---------------------------------------------
 */

/*
 * Protocol description
 *
 * This is a simple request-answer ASCII protcol.  There is only one
 * round of interaction.  The client sends a request message with the 
 * name and studnet id.  The server answers back with either an OK or NOK 
 * (not OK) message.
 *
 * Client message (must include keyword 'name' and 'id'):
 *     name=John Smith; id = 12345js
 *
 * Server message:
 *     OK; or 
 *     NOK
 */

#define SREG_PORT   20515


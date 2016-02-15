/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_app_srv.c
 *  Function : This file contains the 'application' layer part of the server.
 *             In the context of this example, it services the calculation
 *             request.
 * ------------------------------------------------------------------------
 */

#include    <stdio.h>
#include    <netinet/in.h>
#include    "netcalc.h"

/*
 * ------------------------------------------------------------------------
 *  Name     : doCalc
 *  Function : This function parses the request in qBuf (the question buffer),
 *             does the calculation and puts the result in aBuf (the answer
 *             buffer) 
 *  Return   : It returns SUCCESS if the input is legally formated, and 
 *             NO_SUCCESS otherwise.
 *  Note     : Handle the user input error and the protocol error differently.
 *             Also notice that this simple protocol does not have error
 *             handling mechanism.
 * ------------------------------------------------------------------------
 */
int 
doCalc(char *qBuf, int qBufLen, char *aBuf, int *aBufLen)
{
    int     a, b, c;

    /* check app protocol */
    if (qBufLen > 1 && qBuf[0] == OPCODE_QUESTION && 
        qBuf[qBufLen-1] == MSG_TERMINATOR)
    {
        qBuf[qBufLen-1] = 0; /* turn it into a C string */

        /* the sscanf solution will discard any remaining garbage if any */
        if (sscanf(qBuf+1, "%d + %d", &a, &b) == 2)
        {
            c = a + b;
            *aBufLen = sprintf(aBuf, "%c%d=", OPCODE_ANSWER, c);
        }
        else if (sscanf(qBuf+1, "%d - %d", &a, &b) == 2)
        {
            c = a - b;
            *aBufLen = sprintf(aBuf, "%c%d=", OPCODE_ANSWER, c);
        }
        else
        {
            *aBufLen = sprintf(aBuf, "%c=", OPCODE_REJECT);
        }

        return (SUCCESS);
    }
    else 
    {
        return (NO_SUCCESS);
    }
}

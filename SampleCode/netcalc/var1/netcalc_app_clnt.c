/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_app_clnt.c
 *  Function : This file contains the 'application' layer part of the client.
 * ------------------------------------------------------------------------
 */

#include    <stdio.h>
#include    <netinet/in.h>
#include    <string.h>
#include    <strings.h>
#include    "netcalc.h"

/*
 * ------------------------------------------------------------------------
 *  Name     : buildReq
 *  Function : This function builds a request message
 *  Return   : It returns SUCCESS if the messgae is built successfully, 
 *             NO_SUCCESS otherwise
 * ------------------------------------------------------------------------
 */
int 
buildReq(char *inBuf, int inBufLen, char *outBuf, int *outBufLen)
{
    int     n;

    /* remove the \n */
    n = inBufLen - 1; 

    if (n > 0)
    {
        bcopy(inBuf, outBuf+1, n);
        outBuf[0] = OPCODE_QUESTION;
        outBuf[n+1] = MSG_TERMINATOR;

        *outBufLen = n+2;

        return (SUCCESS);
    }
    else
    {
        return (NO_SUCCESS);
    }
}


/*
 * ------------------------------------------------------------------------
 *  Name     : parseRes
 *  Function : This function parses the response
 *  Return   : It returns SUCCESS if the messgae is parsed successfully, 
 *             NO_SUCCESS otherwise
 * ------------------------------------------------------------------------
 */
int 
parseRes(char *inBuf, int inBufLen, char *outBuf)
{
    unsigned char   opcode;

    if (inBufLen >= 2 && inBuf[inBufLen-1] == MSG_TERMINATOR)
    {
        opcode = inBuf[0];

        switch (opcode)
        {
            case OPCODE_ANSWER:
                inBuf[inBufLen - 1] = 0; /* turn it into a C string */
                sprintf(outBuf, "Answer: %s\n", inBuf+1);
                return (SUCCESS);

            case OPCODE_REJECT:
                strcpy(outBuf, "Question rejected\n");
                return (SUCCESS);

            default:
                return (NO_SUCCESS);
        }
    } 
    else
    {
        return (NO_SUCCESS);
    }
}

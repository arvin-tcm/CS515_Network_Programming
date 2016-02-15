/*
 * ------------------------------------------------------------------------
 *  Name     : async_io
 *  Function : This program demonstrates how to do asynchronous i/o. 
 *  Author   : Bin Zhang
 *  Note     : Do not exit the program with kill -9 (or any other signal)
 *             as it will leave the terminal in an undesirable state.
 * ------------------------------------------------------------------------
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <errno.h>
#include    <signal.h>

#define     BUF_SIZE        80  /* the line buffer size */

static char promptStr[] = "\nPlease input (q to quit)\n";
int         fExit = 0;    /* flag to indicate if it is time to exit */

/* ----- various support functions ----- */

/*
 * ------------------------------------------------------------------------
 *  Name     : set_fl
 *  Function : This function is copied from the distribution with the
 *             W. Richard Stevens' book "Advanced UNIX Programming".  
 * ------------------------------------------------------------------------
 */

void
set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int     val;

    if ( (val = fcntl(fd, F_GETFL, 0)) < 0) {
        perror("fcntl F_GETFL error");
        exit(-1);
    }

    printf("%s: flags = 0x%x\n", __FUNCTION__, val);
    val |= flags;       /* turn on flags */

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl F_SETFL error");
        exit(-1);
    }
}


/*
 * ------------------------------------------------------------------------
 *  Name     : clr_fl
 *  Function : This function is copied from the distribution with the
 *             W. Richard Stevens' book "Advanced UNIX Programming".
 * ------------------------------------------------------------------------
 */

void
clr_fl(int fd, int flags) /* flags are the file status flags to turn off */
{
    int     val;

    if ( (val = fcntl(fd, F_GETFL, 0)) < 0) {
        perror("fcntl F_GETFL error");
        exit(-1);
    }

    printf("%s: flags = 0x%x\n", __FUNCTION__, val);
    val &= ~flags;      /* turn flags off */

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl F_SETFL error");
        exit(-1);
    }
}

/*
 * ------------------------------------------------------------------------
 *  Name     : sigHandler
 *  Function : this signal handler catches ctrl-C and ctrl-\ to set the
 *             the exit flag so that the program can reset the terminal
 *             setup.
 *  Comment  :
 * ------------------------------------------------------------------------
 */
void 
sigHandler(int sigNo)
{
    fExit = 1;
}


/* ----- the main ----- */
main(int argc, char *argv[])
{
    char    buf[BUF_SIZE];
    int     n;
    int     fVerbose;
    struct  sigaction    act;
    struct  sigaction    oldActInt; /* old handler for SIGINT  */ 
    struct  sigaction    oldActQuit; /* old handler for SIGQUIT */ 

    /* handle command line args, a very simple-minded approach */
    if (argc == 1) 
    {   /* no verbose flag ? */
        fVerbose = 0;
    }
    else if (argc == 2 && *argv[1] == '-' && *(argv[1]+1) == 'v')
    {   /* need verbose flag */
        fVerbose = 1;
    }
    else 
    {   /* wront command line */
        printf("Usage: %s [-v]", argv[0]);
        exit(0);
    }

    /* fill in the act */
    act.sa_handler = sigHandler;    /* point to the signal handler */
    sigemptyset(&act.sa_mask);      /* mask nothing */
    act.sa_flags = SA_RESTART;      /* restart interrupted system call */

    /* install SIGINT handler */
    if (sigaction(SIGINT, &act, &oldActInt) < 0)
    {
        perror("Failed to install handler for signal SIGINT");
        exit(-1);
    }

    /* install SIGQUIT handler */
    if (sigaction(SIGQUIT, &act, &oldActQuit) < 0)
    {
        perror("Failed to install handler for signal SIGQUIT");
        exit(-1);
    }

    /* make the stdin non-blocking */
    set_fl(STDIN_FILENO, O_NONBLOCK);

    write(STDOUT_FILENO, promptStr, sizeof(promptStr));

    /* loop forever */
    while (!fExit)
    {
        /* read from stdin, but can not be blocked */
        if ((n = read(STDIN_FILENO, buf, BUF_SIZE)) < 0)
        {
            /* just no data ? */
            if (errno == EAGAIN)
            {
                /* no input, sleep 1 to save the CPU cycle */
                sleep(1);

                /* for debug purpose */
                if (fVerbose)
                {
                    write(STDOUT_FILENO, ".", 1);
                }
            }
            else {
                perror("read failed");
                exit(-1);
            }
        }
        /* got something from the user */
        else
        {
            /* write to stdout whatever we read in */
            write(STDOUT_FILENO, buf, n);

            /* start with 'q' */
            if (buf[0] == 'q')
            {
                fExit = 1;
            }
            else
            {
                write(STDOUT_FILENO, promptStr, sizeof(promptStr));
            }
        }
    }

    /* restore signal handlers */
    sigaction(SIGINT, &oldActInt, NULL); 
    sigaction(SIGQUIT, &oldActQuit, NULL); 

    /* make the stdio blocking, again !!! */
    clr_fl(STDIN_FILENO, O_NONBLOCK);

    fflush(stdout);
}


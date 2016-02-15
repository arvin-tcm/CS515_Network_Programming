/*
 * ------------------------------------------------------------------------
 *  Name     : file_cp
 *  Function : Copy a file
 *  Author   : Bin Zhang
 * ------------------------------------------------------------------------
 */
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE        4096

int main(int argc, char *argv[])
{
    int in_fd, out_fd, rd_count, wr_count;
    char buffer[BUF_SIZE];

    if (argc!=3)
    {
        printf("Usage: %s input_file output_file\n", argv[0]);
        exit(1);
    }

    /* open the input file */
    in_fd = open(argv[1], O_RDONLY);
    if (in_fd < 0)
    {
        printf("open failed: %s\n", argv[1]);
        exit(2);
    }
    
    /* create the output file */
    out_fd = open(argv[2], O_WRONLY|O_CREAT, 0644);
    if (out_fd < 0)
    {
        printf("open failed: %s\n", argv[2]);
        exit(3);
    }

    /* the copy loop */
    while (1)
    {
        /* read a block */
        rd_count = read(in_fd, buffer, BUF_SIZE);
        if (rd_count <= 0)
        {
            break; /* end of file */
        }

        /* write a block */
        wr_count = write (out_fd, buffer, rd_count);
        if (wr_count <= 0)
        {
            perror("write failed\n");
            exit(4); /* write error */
        }
    }

    /* close files */
    close(in_fd);
    close(out_fd);
    
    if (rd_count == 0) /* no error on last read */
        exit(0);
    else {
        printf("read failed\n");
        exit(5);       /* error no last read */
    }
}

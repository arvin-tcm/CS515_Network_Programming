/*
 * ------------------------------------------------------------------------
 *  Name     : myerror.h
 *  Function : This file contains the declaration of a group of error 
 *             handling functions that are defined in myerror.c.
 *  Author   : Bin Zhang
 * ------------------------------------------------------------------------
 */

#include	<stdio.h>		/* for definition of errno */
#include	<errno.h>		/* for definition of errno */
#include	<stdarg.h>		/* ANSI C header file */

void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);

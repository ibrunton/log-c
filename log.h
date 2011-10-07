/*
 *       Filename:  log.h
 *
 *    Description:  log library
 *
 *        Version:  1.0
 *        Created:  2011-09-13 17:04
 *       Compiler:  gcc
 *
 *         Author:  Ian D Brunton (ib), iandbrunton@gmail.com
 *
 */

#ifndef LOG_H
#define LOG_H

char* log_date;
char* log_time;
char* log_input;
char* log_output;
char* log_comment;

char* parse_datetime (char*);
char* get_date ();
char* set_time ();

void replace_tags (char);
void wrap_output ();
char* tag (char*, char*);
#endif

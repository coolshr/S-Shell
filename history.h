#ifndef __HISTORY_H
#define __HISTORY_H

void init_history();
void add_history(char *inp);
void write_back();
void write_history();

char history_array[20][4096];
int num_history ;
#endif
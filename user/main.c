/*
 * @Author: your name
 * @Date: 2020-02-14 23:23:34
 * @LastEditTime : 2020-02-15 00:05:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cproj/MIDI/src/mian.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AudioPort.h"
#include "Midi_Dec.h"


unsigned long file_size(char* filename)
{
	FILE* fp;
	fp=fopen(filename,"r");
	if (!fp) return -1;
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fclose(fp);

	return size;
}


int main(int argc, char* argv[])
{	
	FILE *fp;
	unsigned long size;


if (argc>1){
	/* code */
	fp=fopen(argv[1],"r");
}else{
	/* code */
	return 0;
}
	if(!fp){
		printf("open file failed\n");
	}
	size=file_size(argv[1]);
	unsigned char *buf=(unsigned char*)malloc(size);
	if(!buf){
		printf("malloc failed\n");
	}
	memset(buf,0,size);
	fread(buf,size,1,fp);
	Midi_Play(buf,size);
	while (1){
		//do nothing
	}
	
}
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

#include "AudioPort.h"
#include "Midi_Dec.h"


int file_size(char* filename)
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
	AudioPlay("sound/*.wav");
	return 0;
}
/*
 * @Author: your name
 * @Date: 2020-02-14 23:21:15
 * @LastEditTime : 2020-02-14 23:58:45
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cproj/MIDI/AudioPort.c
 */
/*----------------------------------------------
创建于2020/2/12 下午
文件名:AudioPort.c
描述:音频接口文件，提供音频播放相关函数

Fyra-BH
-----------------------------------------------*/
#include "AudioPort.h"

#include <stdio.h>
#include <string.h>

int AudioInit() {
	return 0;
}
int AudioPlay(char* path) {
	char cmd[255];
	memset(cmd,0,255);
	sprintf(cmd,"play %s",path);
	system(cmd);
}
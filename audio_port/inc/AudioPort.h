#ifndef AUDIOPORT__H
#define AUDIOPORT__H
/*----------------------------------------------
创建于2020/2/12 下午
文件名:AudioPort.h
描述:音频接口头文件，提供音频播放相关函数

Fyra-BH
-----------------------------------------------*/

int AudioInit(void);
int AudioPlay(char* path);
#endif // !AUDIOPORT__H

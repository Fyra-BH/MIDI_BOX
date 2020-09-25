/*
 * @Author: your name
 * @Date: 2020-02-14 23:54:31
 * @LastEditTime: 2020-02-15 00:00:46
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /cproj/MIDI_PROJ/midi_dec/inc/Midi_Dec.h
 */
/*----------------------------------------------
创建于2020/2/12 下午
文件名:Midi_Dec.h
描述:midi文件解码头文件

Fyra-BH
-----------------------------------------------*/
#ifndef MIDI_DEC__H
#define MIDI_DEC__H

/*
midi文件头
head_tag固定为MThd
head_len固定为6
midi_type:
	00 00表示一个音轨
	00 01表示多个同步音轨（最常用）
	00 10表示多个独立音轨、
audio_track表示音轨数
tick_format表示基本计时格式
注意字节序!!!
*/

typedef struct midi_head {
	unsigned char head_tag[4];
	unsigned long head_len;
	unsigned short midi_type;
	unsigned short audio_track;
	unsigned short tick_format;
}midi_head_t ;

/*
midi参数结构体
*/
typedef struct midi_para
{
    unsigned long size;//文件大小
    unsigned long speed;//速度，单位us
    unsigned short ticks;//每个四分音符的tick数
    unsigned char tracknum;//音轨数
    /* data */
}midi_para_t;


typedef struct note_table {
	unsigned int number;//midi音符标号
	char* note;//音符名(字符串)
}note_table_t;

static unsigned int Trak_Seek(unsigned char* buf, unsigned long buf_len, unsigned long  start, char* str, unsigned int str_len);
static unsigned int Midi_Player_Init((void*)buf,unsigned long len);
#endif // !MIDI_DEC__H



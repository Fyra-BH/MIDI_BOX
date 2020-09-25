/*
 * @Author: your name
 * @Date: 2020-02-14 23:54:37
 * @LastEditTime: 2020-02-15 00:00:33
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /cproj/MIDI_PROJ/midi_dec/src/Midi_Dec.c
 */
/*----------------------------------------------
创建于2020/2/12 下午
文件名:Midi_Dec.c
描述:提供midi文件解码

Fyra-BH
-----------------------------------------------*/
#include "Midi_Dec.h"

#include <string.h>

/*
midi音符对照表
用于调试
*/
const note_table_t NOTE_TABLE[] = {
{0,"C0"},{1,"C#0"},{2,"D0"},{3,"D#0"},{4,"E0"},{5,"F0"},{6,"F#0"},{7,"G0"},{8,"G#0"},{9,"A0"},{10,"A#0"},{11,"B0"},
{12,"C1"},{13,"C#1"},{14,"D1"},{15,"D#1"},{16,"E1"},{17,"F1"},{18,"F#1"},{19,"G1"},{20,"G#1"},{21,"A1"},{22,"A#1"},{23,"B1"},
{24,"C2"},{25,"C#2"},{26,"D2"},{27,"D#2"},{28,"E2"},{29,"F2"},{30,"F#2"},{31,"G2"},{32,"G#2"},{33,"A2"},{34,"A#2"},{35,"B2"},
{36,"C3"},{37,"C#3"},{38,"D3"},{39,"D#3"},{40,"E3"},{41,"F3"},{42,"F#3"},{43,"G3"},{44,"G#3"},{45,"A3"},{46,"A#3"},{47,"B3"},
{48,"C4"},{49,"C#4"},{50,"D4"},{51,"D#4"},{52,"E4"},{53,"F4"},{54,"F#4"},{55,"G4"},{56,"G#4"},{57,"A4"},{58,"A#4"},{59,"B4"},
{60,"C5"},{61,"C#5"},{62,"D5"},{63,"D#5"},{64,"E5"},{65,"F5"},{66,"F#5"},{67,"G5"},{68,"G#5"},{69,"A5"},{70,"A#5"},{71,"B5"},
{72,"C6"},{73,"C#6"},{74,"D6"},{75,"D#6"},{76,"E6"},{77,"F6"},{78,"F#6"},{79,"G6"},{80,"G#6"},{81,"A6"},{82,"A#6"},{83,"B6"},
{84,"C7"},{85,"C#7"},{86,"D7"},{87,"D#7"},{88,"E7"},{89,"F7"},{90,"F#7"},{91,"G7"},{92,"G#7"},{93,"A7"},{94,"A#7"},{95,"B7"},
{96,"C8"},{97,"C#8"},{98,"D8"},{99,"D#8"},{100,"E8"},{101,"F8"},{102,"F#8"},{103,"G8"},{104,"G#8"},{105,"A8"},{106,"A#8"},{107,"B8"},
{108,"C9"},{109,"C#9"},{110,"D9"},{111,"D#9"},{112,"E9"},{113,"F9"},{114,"F#9"},{115,"G9"},{116,"G#9"},{117,"A9"},{118,"A#9"},{119,"B9"},
{120,"C10"},{121,"C#10"},{122,"D10"},{123,"D#10"},{124,"E10"},{125,"F10"},{126,"F#10"},{127,"G10"}
};

/*
函数名	Trak_Seek
说明：在midi文件中寻找字符串，用于确定音轨标志MTrk
参数：
buf		midi原始数据缓冲区
buf_len	缓冲区大小
start	起始偏移位置
str		要匹配的字符串
str_len	要匹配的字符串长度
*/
static unsigned int Trak_Seek(unsigned char* buf,unsigned long buf_len , unsigned long  start,char *str,unsigned int str_len) {
	for (unsigned int i = start; i < buf_len; i++){
		if (*(buf + i) == *(str)) {//首字母匹配
			unsigned int j;
			for (j = 1; j < str_len; j++){//后续匹配
				if (*(buf + i + j) == *(str + j)){
					//do nothing
				}
				else {
					break;
				}
			}
			if (j == str_len) {//说明匹配
				return i;
			}
		}
	}
}

/*
文件头
默认参数
初始化时会用到

注意字节序!!!
注意字节序!!!
注意字节序!!!
*/
midi_head_t head; 
midi_para_t default_para;
/*
函数名	Midi_Player_Init
说明：初始化default_para
参数：
buf		midi原始数据缓冲区
len		midi原始数据缓冲区大小
*/
static unsigned int Midi_Player_Init((void*)buf,unsigned long len){
//先确定几个必要的参数
    memccpy(&head,buf,sizeof(head));
    default_para.size=len;
    default_para.ticks=0;
}

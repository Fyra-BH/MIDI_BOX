/*
 * @Author: your name
 * @Date: 2020-02-14 23:54:37
 * @LastEditTime: 2020-02-18 13:13:18
 * @LastEditors: Please set LastEditors
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
#include "AudioPort.h"

#include <string.h>
#include <stdio.h>
#include <pthread.h>

/*
midi音符对照表
用于调试
*/
const note_table_t NOTE_TABLE[128] = {
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
函数名	Seq_Find
说明：在midi文件中寻找字符串，用于确定音轨标志MTrk等
参数：
buf		midi原始数据缓冲区
buf_len	缓冲区大小
start	起始偏移位置
str		要匹配的字符串
str_len	要匹配的字符串长度
*/
 unsigned int Seq_Find(char* buf,unsigned long buf_len , unsigned long  start,char *str,unsigned int str_len) {
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
	return -1;
}

/*
默认参数
初始化时会用到
*/
midi_para_t default_para;
/*
函数名	Midi_Player_Init
说明：初始化default_para
参数：
buf		midi原始数据缓冲区
len		midi原始数据缓冲区大小
*/
static unsigned int Midi_Player_Init(unsigned char* buf,unsigned long len){
//先确定几个必要的参数
	if(strcmp(buf,"MThd")){
		printf("error file type\n");
		return -1;
	}

    default_para.size=len;
	default_para.time=0;
	/**
	 * 获取音轨数
	*/
	default_para.tracknum=*(buf+MIDI_TRCKS)*256+*(buf+MIDI_TRCKS+1);
	printf("tracknum=%d\n",default_para.tracknum);

	/*
	获取每个四分音符的tick数
	*/
	default_para.ticks=*(buf+MIDI_TICK_FORMAT)*256+*(buf+MIDI_TICK_FORMAT+1);
	printf("ticks=%d\n",default_para.ticks);
	
	/**
	 * 获取速度
	 * 需要寻找 FF 51	事件
	 * 以第一次设定的速度为默认速度
	 * default_para.speed
	*/{
		char evt[3]={0xff,0x51};//事件
		int pos=Seq_Find(buf,len,0,evt,2);
		pos+=2;
		int num=*(buf+pos);
		pos++;//准备开始读数据
		unsigned long speed=0;
		for (int i = 0; i < num; i++){
			speed=speed*256+*(buf+pos++);
		}
		default_para.speed=speed;
		if(default_para.speed>10000000)
			default_para.speed=652173;
		printf("speed=%ld\n",default_para.speed);
	}

	/**
	 * 设置音轨偏移表
	*/
	default_para.offset_table=(unsigned long**)malloc(sizeof(unsigned int*)*default_para.tracknum);
	if (default_para.offset_table==NULL){
		printf("malloc failed\n");
		return -1;
	}
	unsigned long offset=0;
	for (int i = 0; i < default_para.tracknum; i++){
		*(default_para.offset_table+i)=Seq_Find(buf,len,offset+1,"MTrk",4);
		offset=*(default_para.offset_table+i);
		printf("off[%d]=%ld\n",i+1,*(default_para.offset_table+i));
	}
	return 0;
}


/*
函数名	Midi_Play
说明：播放函数
参数：
buf		midi数据
len		缓冲区大小
*/
int Midi_Play(unsigned char* buf,unsigned long len){
	Midi_Player_Init(buf,len);
	unsigned long offset=Seq_Find(buf,len,0,"MTrk",4);
	Midi_Play_Track(buf,offset);
	offset=Seq_Find(buf,len,offset+1,"MTrk",4);
	//Midi_Play_Track(buf,offset);
}

/**
 * 函数名	Midi_Play_Track
 * 说明 	播放单个midi音轨
 * 参数	
 * buf		midi数据
 * offset	从某个音轨标识开始的偏移
*/
int Midi_Play_Track(unsigned char* buf,unsigned long  offset){
	unsigned char *pointer=buf+offset;//数据指针
	/*
	*先检测标记字符
	*/
	char cmp[5];
	memcpy(cmp,buf+offset,5);
 	if(strcmp(cmp,"MTrk")){
		 printf("error track\n");
		 return -1;
	 }
	pointer+=8;
	 printf("start\n");


	unsigned long time=0;//这个参数来自动态字节
	unsigned int num=0;//用于记录动态字节个数
	unsigned long maxlen=0xffff;//防止死循环

	while (maxlen--){//开始解码
		/**
		 * 时间
		*/
		time=Read_Dynamic_Byte(pointer,&num);
		unsigned long tmp=time*default_para.speed/default_para.ticks;
		pointer+=num;
		usleep(tmp);
		/**
		 * 事件处理
		*/
		if(Deal_Channel_event(pointer,&num)==-1){
			printf("end track\n");
			return 0;
		}
		default_para.time+=(float)tmp/1000000.0;
		pointer+=num;
	}

	return 0;
}

/**
 * 函数名	Deal_Channel_event
 * 描述		处理midi事件
 * 参数		
 * data		数据指针
 * cnt		记录长度用
*/
static int Deal_Channel_event(unsigned char *data,unsigned int *cnt){
	*cnt=0;
	switch (*data>>4)
	{
	case 8:
		//printf("松开音符\n");
		*cnt=3;//前进3 
		break;
	case 9:
		//printf("按下音符%s\ttime=%0.2fs\n",NOTE_TABLE[*(data+1)].note,default_para.time);
		;
		char filename[255];
		memset(filename,0,255);
		sprintf(filename,"sound/MUSICBOX/%s.wav",NOTE_TABLE[*(data+1)].note);
		AudioPlay(filename);
		*cnt=3;//前进3
		break;
	case 10://0x0a
		printf("触后音符\n");
		*cnt=3;//前进3
		break;
	case 11://0x0b
		printf("控制器变化\n");
		*cnt=3;//前进3
		break;
	case 12://0x0c
		printf("改变乐器\n");
		*cnt=1;
		break;
	case 13://0x0d
		printf("通道触动压力\n");
		*cnt=1;
		break;
	case 14://0x0e
		printf("弯音轮变换\n");
		*cnt=3;//前进3
		break;
	case 15://0x0f 元事件
		//printf("元事件\n");
		*cnt=*(data+2)+3;
		if(*(data+1)==0x2f){
			return(-1);//会引起调用它的Midi_Play_Track返回，结束播放
		}
		break;
	
	default:
		printf("unkonw command\n");
		exit(0);
		break;
	}
}

/**
 * 函数名	Read_Dynamic_Byte
 * 说明 	读取midi中的动态字节
 * 参数		
 * data		数据地址
 * cnt		记录长度用
*/
static unsigned long Read_Dynamic_Byte(unsigned char *data,unsigned int *cnt){
	unsigned long res=0;
	*cnt=0;
	do{
		unsigned char tmp=*data&(~0x80);
		res=res*128+tmp;
		*cnt=*cnt+1;
	}
	while (*data++&0x80);//高位为1
	return res;
}

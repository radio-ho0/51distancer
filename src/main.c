#include <at89x52.h>
#include "lcd1602.h"
#include <compiler.h>
//typedef unsigned int uint;
typedef unsigned char uchar;


/* 
+--------+------+------+-----------+
|   P0   |  P1  | P2   |  P3       |
+--------+------+------+-----------+
|   0x80 | 0x90 | 0xA0 | 0xB0      |
+--------+------+------+-----------+
*/

enum count_num{
    TIMER_1S = 200,

};

//#define sdcc
#define GDO(count, val, func) do\
            {\
                if(++count > val){\
                    count = 0;\
                    func();\
                }\
        }while(0)

SBIT(Tr, 0xa0 , 2);
SBIT(Ec, 0xb0, 3);
SBIT(LED, 0x80, 0);

int L = 0;
int tmp;

void ultInit()
{
    TMOD = 0x01;           //设T0为方式1；
    TH0 = 0;
    TL0 = 0; 
    TR0 = 1;  
    ET0 = 1;               //允许T0中断
    EA = 1;                //开启总中断
    Tr = 0;
}

void ultStart()
{
    char i;
    Tr = 1;
    for ( i = 0; i < 45; i++ ) {
        ;
    }
    Tr = 0; 
}

void count()
{
    uint time = 0;

    while(!Ec);                //等待高电平
    TR0 = 1;                   //打开计时器
    while(Ec);                 //等待低电平
    TR0 = 0;                   //关闭计时器

    time = TH0*256 + TL0;      //计算时间
    //L = 0.18446*time;
    L = 1 *time;

    TH0 = 0;                   //重置计时器
    TL0 = 0;
}

void show()
{
    LcdShowNum(1, 2, L);
}

void main(void)
{
    InitLcd1602();
    ultInit();
    LED = 0;
    tmp = TMOD;
    LcdShowStr(0, 0, "51 distancer");
    LcdShowNum(1, 1, tmp);

    for(;;){
        int j;
        for ( j  = 0 ; j  < 9999; j ++ ) {
            ;
        }
        ultStart();
        count();
        LcdShowStr(0, 0, "--");
        show();
    }

}


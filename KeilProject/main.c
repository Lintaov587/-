#include <REGX52.H>
#define uint8_t unsigned char

/* ����������״̬ */
#define REST  0x00
#define INCN  0x01
#define NONE  0x02

/* ����ܵĶ���   */
uint8_t seg[] = {0x18, 0x3f, 0x92, 0x13, 0x35,
				 0x51, 0x50, 0x3b, 0x10, 0x11};

/* ����ʹ�õ����� */
sfr     seg7  = 0xA0;
sbit    seg1  = 0x87;
sbit    seg2  = 0x86;
sbit    k1    = 0x82;
sbit    LED   = 0x85;
sbit    k2    = 0x84;

/* ����ʹ�õĺ��� */
void delay(unsigned int time);  
     /* ��ʱ����  */

void segDisplay(uint8_t *disNum); 
    /* �������ʾ */

void ledDisplay(int period, uint8_t disNum); 
    /* PWM ����led��ǰ������� */

void ledDisplay2(int period, uint8_t disNum);  
    /* PWM ����led�ĺ������� */

void numInc(uint8_t *disNum);	
 	/*disNum ����λ��ʾ����ܸ�λ��
   ����λ��ʾ����ܵ�λ��
�����������ʾ��ֵ��ȷ��1  */

uint8_t low4B(uint8_t num);                      
   /*ȡnum����λ*/

uint8_t high4B(uint8_t num);            
   /*ȡnum����λ*/

uint8_t readKey(void);              
   /*��ȡ����*/

void main()
{
    uint8_t initDisplayNum = 0x00;    
	   //��ʼ��ʾ��ֵ
    LED = 1;                       
	   //��ʼ��LED
    while (1)
    {
	    segDisplay(&initDisplayNum);  
		  //��ѭ������
    }
}
void ledDisplay(int period, uint8_t disNum)
{
    static int brightNess = 0;             
	  //��ʼ����
    int i;                              
	  //ѭ����������
    
    if(disNum > brightNess) brightNess++;
    else if (brightNess == 0) brightNess = 0;
    else brightNess--;                
	  //��λʱLED�����������Ͷ�����ͻȻϨ��

    for(i = 0; i < period; i++)
    {
        if (i > brightNess)
        {
            LED = 1;
        }
        else
        {
            LED = 0;
        }
    }
    LED = 1;                     
	   //ǰ��PWM����
}

void ledDisplay2(int period, uint8_t disNum)   
//���PWM���ڣ�ͬ�Ϻ���
{
    static int brightNess = 0;
    int i;

    if(disNum > brightNess) brightNess++;
    else if (disNum == 0) brightNess = 0;
    else brightNess--;

    for(i = 200; i < 200+period; i++)
    {
        if (i > brightNess)
        {
            LED = 1;
        }
        else
        {
            LED = 0;
        }
    }
    LED = 1;    

}							

uint8_t readKey(void)           //������⺯��
{
    static uint8_t k1_press_time = 0;
    static uint8_t k2_press_time = 0;
    static uint8_t k1_last = 0;
    static uint8_t k2_last = 0;
    static bit toggle1 = 0;
    static bit toggle2 = 0;
    uint8_t key1;
    uint8_t key2;                       
	       //���帨������

    key1 = ~k1;
    key2 = ~k2;

    k1_press_time = k1_press_time * key1 + key1;
    k2_press_time = k2_press_time * key2 + key2;    
//����ʱ�����������ʱ�����kx_press_time * ��������


    if(k1_last == 0 && key1 == 1)  
    {
        toggle1 = 1;
    }                              
	   //����1������



    if(k2_last == 0 && key2 == 1)
    {
        toggle2 = 1;
    }                           
	       //����2������
    

    k1_last = key1;
    k2_last = key2;


    if(k1_press_time > 5 && toggle1)
    {
        k1_press_time = 0;
        toggle1 = 0;
        return INCN;           
		        //����1״̬�ж�
    }
    else if(k2_press_time > 5 && toggle2)
    {
        k2_press_time = 0;
        toggle2 = 0;
        return REST;
    }                            
	      //����2״̬�ж�
    else
    {
        return NONE;         
		       //��״̬�ж�
    }
}

void delay(unsigned int time)        //��ʱ
{
    int i,j;
    for(i = 0; i < time; i++)
    {
        for(j = 0; j < i; j++)
        ;
    }
}

uint8_t low4B(uint8_t num)           //
{
    return num & 0x0f;
}

uint8_t high4B(uint8_t num)                   //
{
    return (num & 0xf0) >> 4;
}

void segDisplay(uint8_t *disNum)
{
    uint8_t keyStatus = NONE;
    seg1 = 0;
    seg2 = 1;
    seg7 = seg[low4B(*disNum)];   
	         //����disNum ����λ�����������ʾ��λ
	//delay(5);
    ledDisplay(150, *disNum);        
	       //����disNum ����LED ���� ǰ������
    seg7 = 0xff;                 
	         //�����ʾ��������Ӱ
    seg1 = 1;
    seg2 = 0;
    seg7 = seg[high4B(*disNum)];      
	     //����disNum ����λ�����������ʾ��λ
	//delay(5); 
    ledDisplay2(150, *disNum);         
	     //����disNum ����LED ���� �������
    seg7 = 0xff;                     
	     //�����ʾ��������Ӱ
    keyStatus = readKey();           
	      //��ȡ����
    if(keyStatus == INCN)
    {
        numInc(disNum);             
		    //����������һ�¼�
    }
    else if(keyStatus == REST)
    {
        *disNum = 0x00;            
		     //��ʾ����
    }
    else
    {
        ;
    }                                    
    
}
void numInc(uint8_t *disNum)
{
	if(low4B(*disNum) != 0x09)
	{

		(*disNum)++;
	}	                   
	      //������Ǹý�λ���ͼ�һ
	else if((*disNum) == 0x99)
	{
		*disNum = 0x99;
	}                           
	 //�������������
	else
	{
		(*disNum) = (((high4B(*disNum)+1)<<4) & 0xf0);
	}                          
	   //�����λ��������λ��һ������λ���㡣
}

/**************��C��COPYRIGHT 2019 ��Ƶ�뼯�ɵ�·��ϵͳ�о�����*******************
* �ļ���   : tm1914.c
* ����     : ��ʼ��������tm1914
* ʵ��ƽ̨ : STM32F103ZET6
* �汾     : V0.1
* Ƕ��ϵͳ : ��
* ��ע     : 
*******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "boards.h"
#include "tm1914.h"
#define BulbNum  5
u8 BulbData[BulbNum*3]={0};
/**************��C��COPYRIGHT 2019 ��Ƶ�뼯�ɵ�·��ϵͳ�о�����*******************
* ������   : TM1914_Init
* ����˵�� : ��ʼ����TM1914�������ݵ�����
* ����˵�� : ��
* �������� : ��
* ��ע     : �˴��趨����ΪPB8��PB9��ͨ���ƹ������������ݵĴ���
* ����     :������
* �޸�ʱ�� :2019.9.18
*******************************************************************************/
void TM1914_Init(void)
{
	nrf_gpio_cfg_output(DIN);
	nrf_gpio_cfg_output(FDIN);
}
/**************��C��COPYRIGHT 2019 ��Ƶ�뼯�ɵ�·��ϵͳ�о�����*******************
* ������   : TM1914_SendByte
* ����˵�� : ����һ���ֽ���TM1914
* ����˵�� : b - ׼�����͵��ֽ�
* �������� : ��
* ��ע     : �͵�ƽʱ��Ϊ360��50nsʱ����������0
			   �͵�ƽʱ��Ϊ650-1000nsʱ����������1
			   һ��bit������Ϊ1.25��s��Ƶ�� 800KHz���� 2.5��s��Ƶ�� 400KHz����Χ��
			   �ô������ʱʱ����Ҫ���в���
* ����     :������
* �޸�ʱ�� :2019.9.18
*******************************************************************************/
void TM1914_SendByte(u8 b)//����һ���ֽ�
{//��ʱ�Ǹ�����Ƶ48M����,����ܽſ����Լ�����
 int i;
	for(i=0;i<8;i++)
	{
		if(b&0x80)
		{
 			nrf_gpio_pin_clear(DIN);	//0
			nrf_gpio_pin_clear(FDIN);
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();

///
 			nrf_gpio_pin_set(DIN);	//1
			nrf_gpio_pin_set(FDIN);
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();

		}
		else
		{
     	nrf_gpio_pin_clear(DIN);	//0
			nrf_gpio_pin_clear(FDIN);
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
    __nop();
		__nop();
     	nrf_gpio_pin_set(DIN);	//1
			nrf_gpio_pin_set(FDIN);
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		}
		b<<=1;
  }
}
/**************��C��COPYRIGHT 2019 ��Ƶ�뼯�ɵ�·��ϵͳ�о�����*******************
* ������   : TM1914_SetData
* ����˵�� : �ڷ������ݰ����趨һ��ָ�����ݵ�����
* ����˵�� : r - ָ�����ݵ�rͨ����ֵ
						 g - ָ�����ݵ�gͨ����ֵ
						 b - ָ�����ݵ�bͨ����ֵ
						 num - ���ݱ��
* �������� : ��
* ��ע     : 
* ����     :������
* �޸�ʱ�� :2019.9.18
*******************************************************************************/
void TM1914_SetData(u8 r,u8 g,u8 b,int num)
{
	  u8 *p;
		p=&BulbData[(num-1)*3];
	   *p++=r;
	   *p++=g;
	   *p++=b;
}
/**************��C��COPYRIGHT 2019 ��Ƶ�뼯�ɵ�·��ϵͳ�о�����*******************
* ������   : TM1914_SendFrame
* ����˵�� : ��TM1914����������һ֡����
* ����˵�� : ��
* �������� : ��
* ��ע     : 
* ����     :������
* �޸�ʱ�� :2019.9.18
*******************************************************************************/
void TM1914_SendFrame(void)
{
	 u8 *p;
	 int i;	
	 TM1914_SendByte(0xff);
	 TM1914_SendByte(0xff);
	 TM1914_SendByte(0xff);
	 TM1914_SendByte(0x00);
	 TM1914_SendByte(0x00);
	 TM1914_SendByte(0x00);	
	 p=(void*)BulbData;
	 for(i=0;i<BulbNum*3;i++)
	 {
	  TM1914_SendByte(*p++);
	 }	

}
void TM1914_Test(void)
{

TM1914_SendByte(0xAA);
}
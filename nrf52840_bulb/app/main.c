#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "esp8266.h"
#include "w5500.h"
#include "tm1914.h"
#define RXBUF_LEN  3              //Ӧ�ó���UART���ջ����ֽ���
#define UART_TX_BUF_SIZE 256       //���ڷ��ͻ����С���ֽ�����
#define UART_RX_BUF_SIZE 256       //���ڽ��ջ����С���ֽ�����

//SPI���ͻ������飬ʹ��EasyDMAʱһ��Ҫ����Ϊstatic����
static uint8_t    my_tx_buf[4096];  
//SPI���ջ������飬ʹ��EasyDMAʱһ��Ҫ����Ϊstatic����
static uint8_t    my_rx_buf[4096];  
extern  char Rx232buffer[Buf_Max]; 
void test(void);

/***************************************************************************
* ��  �� : ����GPIO�ߵ�ƽʱ�������ѹΪ3.3V 
* ��  �� : �� 
* ����ֵ : ��
**************************************************************************/
static void gpio_output_voltage_setup_3v3(void)
{
    //��ȡUICR_REGOUT0�Ĵ������жϵ�ǰGPIO�����ѹ���õ��ǲ���3.3V��������ǣ����ó�3.3V
    if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) !=
        (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos))
    {
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
                            (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos);

        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        //��λ����UICR�Ĵ���
        NVIC_SystemReset();
    }
}
int main(void)
{
		uint8_t RxCnt = 0;             //UART�����ֽ���
	  uint8_t UartRxBuf[RXBUF_LEN];  //UART���ջ���

	//����GPIO�����ѹΪ3.3V
  gpio_output_voltage_setup_3v3();
  bsp_board_init(BSP_INIT_LEDS);
  esp8266_init();
	SPI_W5500_Init();              //��ʼ��SPI���ⲿ�ж�
	Load_Net_Parameters();		    //װ���������	
	W5500_Hardware_Reset();		    //Ӳ����λW5500
	W5500_Initialization();		    //W5500��ʼ������
	TM1914_Init();
	
	while(true)
	{
	test();
	}
}
void test()
{
	TM1914_SendFrame();
	W5500_Socket_Set();           //W5500�˿ڳ�ʼ������
	if(Hand("LED"))                      //  �յ���LED1��ָ��
	{
		nrf_gpio_pin_toggle(LED_1);           //����ָʾ��D1
		for(int i=0;i<50;i++)
			if(Rx232buffer[i]=='L'&&Rx232buffer[i+1]=='E'&&Rx232buffer[i+2]=='D')
			{
				TM1914_SetData(Rx232buffer[i+4],Rx232buffer[i+5],Rx232buffer[i+6],Rx232buffer[i+3]-48);
			}
		CLR_Buf();			
	}  

			
		
    if(W5500_Interrupt)         //����W5500�ж�
	  {
			W5500_Interrupt_Process();//W5500�жϴ��������
		}

		if((S0_Data & S_RECEIVE) == S_RECEIVE)//���Socket0���յ�����
		{
			S0_Data&=~S_RECEIVE;
			Read_SOCK_Data_Buffer(0, Rx_Buffer);
            if(Rx_Buffer[0]=='2')		
               nrf_gpio_pin_toggle(LED_2);				
				
		}
}

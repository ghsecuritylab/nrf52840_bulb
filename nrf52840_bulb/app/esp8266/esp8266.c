#include "esp8266.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_uart.h"
#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

#define UART_TX_BUF_SIZE 256       //���ڷ��ͻ����С���ֽ�����
#define UART_RX_BUF_SIZE 256       //���ڽ��ջ����С���ֽ�����

#define RXBUF_LEN  3              //Ӧ�ó���UART���ջ����ֽ���

volatile uint8_t Count=0;                                                      // ���������ۼӼ���       
char Rx232buffer[Buf_Max];                                                     // ��������


void uart_error_handle(app_uart_evt_t * p_event)
{
    uint8_t cr;
	
	  //ͨѶ�����¼�
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    //FIFO�����¼�
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
		//���ڽ����¼�
    else if (p_event->evt_type == APP_UART_DATA_READY)
    {
			app_uart_get(&cr);
			Rx232buffer[Count]=cr;          //ͨ������USART3��������
			Count++;                                                //�����ۼӽ�������                          
	  if(Count>Buf_Max)                                       //���������ڶ����������������ʱ����ͷ��ʼ��������
		{
			 Count = 0;
		} 
		}
  
		//���ڷ�������¼�
    else if (p_event->evt_type == APP_UART_TX_EMPTY)
    {
        //��תָʾ��D2״̬��ָʾ���ڷ�������¼�
			 // nrf_gpio_pin_toggle(LED_2);
    }
}
/**************************************************************************************
 * ��  �� : ��ʼ��ESP8266ģ���õ�������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void esp8266_init(void)
{
	uint32_t err_code;
	
	//���崮��ͨѶ�������ýṹ�岢��ʼ��
  const app_uart_comm_params_t comm_params =
  {
    RX_PIN_NUMBER,//����uart��������
    TX_PIN_NUMBER,//����uart��������
    RTS_PIN_NUMBER,//����uart RTS���ţ����عرպ���Ȼ������RTS��CTS���ţ����������������ԣ������������������ţ����������Կ���ΪIOʹ��
    CTS_PIN_NUMBER,//����uart CTS����
    APP_UART_FLOW_CONTROL_DISABLED,//�ر�uartӲ������
    false,//��ֹ��ż����
    NRF_UART_BAUDRATE_115200//uart����������Ϊ115200bps
  };
  //��ʼ�����ڣ�ע�ᴮ���¼��ص�����
  APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);

  APP_ERROR_CHECK(err_code);
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 5));
	nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 5));
	nrf_delay_ms(500);
	esp8266_test();
  esp8266_asmod();   
}

void esp8266_test(void)
{
	while(!Hand("OK"))                        //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{		
		  printf("AT\r\n");                     //��������ָ��"AT"
		  nrf_delay_ms(500);                     //����ʱ������
	}
  CLR_Buf();                                //��ս�����������
  nrf_gpio_pin_clear(LED_3);                            //���ESP8266ģ������ָ��󣬵���ָʾ��D3
}

void esp8266_asmod(void)
{
	 //��ESP8266ģ�鷢������ESP8266����ģʽ���������"OK"����"no change"
	while(!(Hand("OK") | Hand("no change")))   //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{		
		  printf("AT+CWMODE=3\r\n");            //��������ESP8266����ģʽΪAP+STATIONģʽ
		  nrf_delay_ms(500);                     //����ʱ������
	}
  //����"OK"�Ļ�
//  if(Hand("OK")) 
//  {		
//		  printf("AT+RST\r\n");                 //����ģ�飬�����λ
//		  nrf_delay_ms(500);                     //����ʱ������
//	 }
  CLR_Buf();                                //��ս�����������
  //��ESP8266ģ�鷢������ESP8266�����ڶ�·����ģʽ������
	while(!(Hand("OK")))                       //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{		
		  printf("AT+CIPMUX=1\r\n");            //����ESP8266�����ڶ�·����ģʽ
		  nrf_delay_ms(500);                     //����ʱ������
	 }
  CLR_Buf();                                //��ս�����������
  //��ESP8266ģ�鷢�ͽ���TCP�������ҿ��Ŷ˿�Ϊ5000������
	while(!(Hand("OK")))                       //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{		
		  printf("AT+CIPSERVER=1,5000\r\n");    //����TCP�������ҿ��Ŷ˿�Ϊ5000
		  nrf_delay_ms(500);                     //����ʱ������
	 }
  CLR_Buf();                                //��ս�����������
  //��ESP8266ģ�鷢�ͻ�ȡ����IP��ַ������
	while(!(Hand("OK")))                       //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{		
		  printf("AT+CIFSR\r\n");               //��ȡ����IP��ַ
		  nrf_delay_ms(500);                     //����ʱ������
	 }
  CLR_Buf();                                //��ս�����������
}



/**************************************************************************************
 * ��  �� : ���ֳɹ������
 * ��  �� : char *a���Ա��ַ���
 * ����ֵ : ��
 **************************************************************************************/
bool Hand( char *a)
{ 
  if(strstr(Rx232buffer,a)!=NULL)    //�ж�ָ��a�е��ַ����Ƿ���Rx232buffer�������ַ������Ӵ�
	   return true;
	else
		 return false;
}

/**************************************************************************************
 * ��  �� : ��ս�����������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void CLR_Buf(void)
{
	uint8_t k;
  for(k=0;k<Buf_Max;k++)      
  {
			Rx232buffer[k] = 0;
	}      
  Count = 0;                                         //���������ۼӼ������㣬���´ν��մ�ͷ��ʼ
}



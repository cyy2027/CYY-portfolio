
#include "work.h"


extern void create_task_of_host( void );
extern void create_task_of_listen( void );
extern void create_task_of_ctrl( void );

void cslock_init(void);
void myprintf( char* format, ... );


//串口0(调试口)收到的数据，通过串口1发送出去
void swap_rx0_to_tx1(void)
{
	uint8_t buf[8];
	int ret = uart->read( _e_uart0,buf,sizeof( buf ) );
	if( ret == 0 ) return;
	
	for( int i=0;i<ret;i++ )
		uart->write( _e_uart1,buf[i] );
}

//串口1收到的数据，通过串口0发送出去。
void swap_rx1_to_tx0(void)
{
	uint8_t buf[8];
	int ret = uart->read( _e_uart1,buf,sizeof( buf ) );
	if( ret == 0 ) return;
	
	for( int i=0;i<ret;i++ )
		uart->write( _e_uart0,buf[i] );
}

#define NetRST 56
void task_start_entry(void *arg)
{
	printf("%s\r\n",__FUNCTION__);
	
	//初始化uart0
	uart->close( _e_uart0 );
	uart->init( _e_uart0,115200 );
	uart->open( _e_uart0 );
	
	
	//初始化 sysprintf
	cslock_init();
	
	work_data_load();
	
	//创建其他任务
	create_task_of_host();
	create_task_of_listen();
	create_task_of_ctrl();
	

	//调试4G模块
	
	//4G模块的串口 1
	uart->init( _e_uart1,115200 );
	uart->open( _e_uart1 );
	
	
	//初始化4G模块的复位引脚,并复位4G模块
	gpio->set_mode( NetRST,PIN_MODE_OUTPUT );
	gpio->write( NetRST,0 );
	vTaskDelay( pdMS_TO_TICKS( 100 ) );
	gpio->write( NetRST,1 );
	vTaskDelay( pdMS_TO_TICKS( 100 ) );
	gpio->write( NetRST,0 );
	
	
	
	//实验之前，需要先接通220V，
	//因为4G模块的5V供电，依赖于220V转12V 转5V。
	//调试器只能提供3.3V，无法驱动4G模块
	
/*
AT

OK
------
AT+RESET
OK
^boot.rom.'v....'!\n
RDY

^MODE: 17,17

+E_UTRAN Service

+CGEV: ME PDN ACT 1

------
读取模块厂家信息
AT+CGMI

+CGMI: "AirM2M"

OK

------
读取模块的固件信息
AT+CGMR

+CGMR: "AirM2M_780EP_V1010_LTE_AT"

OK
------
查询物联网卡/SIM卡有没有插好
AT+CPIN?

+CPIN: READY

OK
------
查询信号质量
AT+CSQ

+CSQ: 22,0

OK
------
查询网络的注册状态
AT+CREG?

+CREG: 0,1

OK
------
查询附着GPRS网络
AT+CGATT?

+CGATT: 1

OK

------
设置透明传输
AT+CIPMODE=1


OK
------
设置模块为单链接
AT+CIPMUX=0

OK
------
自动获取APN

AT+CSTT="","",""

OK
------
激活移动场景,激活之后，可以去获取IP
AT+CIICR

OK
------
查询IP
AT+CIFSR

10.191.209.135
------
连接TCP服务器
AT+CIPSTART="TCP","www.armsoc.cn","9002"

AT+CIPSTART="TCP","www.armsoc.cn","9002"

OK

CONNECT




*/
	
	
	
	
	
	
	
	while(1)
	{
		swap_rx0_to_tx1();
		swap_rx1_to_tx0();
		
		vTaskDelay( pdMS_TO_TICKS( 10 ) );
		
	}
	
}

static TaskHandle_t TaskHandle_start;


void create_task_of_start( void )
{
	printf("%s\r\n",__FUNCTION__);
	//创建任务
	xTaskCreate( task_start_entry, (const char *)"start", 1024, NULL, 2, &TaskHandle_start );
}


/*----------------------------------------------------------------------------*/


static SemaphoreHandle_t cslock;
static int flag_init = 0;				//描述锁是否已经初始化了
//初始化锁
void cslock_init(void)
{
	if( flag_init == 1 ) return;
	cslock = xSemaphoreCreateMutex();
	if( cslock == NULL )
	{//说明不成功
		printf( "err:%s\r\n",__func__ );
		while(1);
	}
	flag_init = 1;
}

void cslock_get(void)
{
	if( flag_init == 0 ) return;
	
	//获取cslock，最多等待100个tick，超时，则返回错误；成功，返回TRUE
	int ret = xSemaphoreTake( cslock,0xFFFFFFFF );
	if( ret == pdTRUE )
	{//说明，成功获取了
	
	}
	else
	{//失败
		
	}
}

void cslock_free(void)
{
	if( flag_init == 0 ) return;
	xSemaphoreGive( cslock );
}


void sysprintf( char* format, ... )
{
	if( flag_init == 0 ) return;
	
	char buf[256];		//最大可以打印的字符串长度
	
	va_list v_args;
	va_start( v_args,format );
	(void)vsnprintf( buf,sizeof(buf),format,v_args );
	
	va_end( v_args );
	
	//获取锁
	cslock_get();
	//使用互斥资源,因为printf最终调用到uart,指向同一个硬件资源，好几个任务同时使用，是不行的。
	printf("%s",buf);
	//释放锁
	cslock_free();
}


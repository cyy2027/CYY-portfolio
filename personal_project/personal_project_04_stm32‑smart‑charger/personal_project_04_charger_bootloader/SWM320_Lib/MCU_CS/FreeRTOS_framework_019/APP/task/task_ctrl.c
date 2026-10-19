#include "framework.h"

#include "hlw8012.h"


void task_ctrl_entry(void *arg)
{
	sysprintf("%s\r\n",__FUNCTION__);
	
	hlw_pul_t event ;
	
	vTaskDelay( pdMS_TO_TICKS( 5000 ) );
	hlw_opt->init();
	while(1)
	{
		for( int road=0;road<MaxSock;road++ )
		{
			if( hlw_opt->get( road,&event ) >= 0 )
			{//成立，则说明收到消息
				sysprintf( "road:%d \ttype:%d \tcycle:%d \tval:%d\r\n",
					road,event.type,event.cycle,event.val );
				
			}
		}
		
		vTaskDelay( pdMS_TO_TICKS( 5 ) );
	}
	
}

static TaskHandle_t TaskHandle_ctrl;


void create_task_of_ctrl( void )
{
	sysprintf("%s\r\n",__FUNCTION__);
	//创建任务
	xTaskCreate( task_ctrl_entry, (const char *)"ctrl", 1024, NULL, 2, &TaskHandle_ctrl );
}



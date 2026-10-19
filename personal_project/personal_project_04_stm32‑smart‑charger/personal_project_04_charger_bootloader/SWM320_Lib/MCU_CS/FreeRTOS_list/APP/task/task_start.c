#include "SWM320.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stdarg.h>
#include <string.h>


extern void create_task_of_host( void );
extern void create_task_of_listen( void );
extern void create_task_of_ctrl( void );

void cslock_init(void);
void myprintf( char* format, ... );

List_t mylist;		//链表根节点


void task_start_entry(void *arg)
{
	uint32_t tick = 0;
	printf("%s\r\n",__FUNCTION__);
	
	//初始化 myprintf
	cslock_init();
	//创建其他任务
	create_task_of_host();
	create_task_of_listen();
	create_task_of_ctrl();
	
	//链表示例
	ListItem_t node0;
	ListItem_t node1;
	ListItem_t node2;
	
	
	
	//①创建一个链表的根节点
	vListInitialise( &mylist ) ;
	
	//②n个节点初始化 
	vListInitialiseItem( &node0 );
	vListInitialiseItem( &node1 );
	vListInitialiseItem( &node2 );
	
	node0.xItemValue = 3;
	node1.xItemValue = 5;
	node2.xItemValue = 2;
	

	//③把这些节点插入到链表里面去
#if ( 0 )
	//排序插入
	vListInsert( &mylist,&node0 );
	vListInsert( &mylist,&node1 );
	vListInsert( &mylist,&node2 );
	

#else
	//插入到尾部
	
	vListInsertEnd( &mylist,&node0 );
	vListInsertEnd( &mylist,&node1 );
	vListInsertEnd( &mylist,&node2 );
#endif
	
	ListItem_t* node = listGET_HEAD_ENTRY( &mylist );
	while( 1 )
	{
		//最后一个节点，就是根节点
		//判断是否为最后一个节点，如果是，则退出 
//		if( node == listGET_END_MARKER( &mylist )   ) break;
		
		//如果当前的节点，就是链表的根节点 pxIndex ，说明就是表尾
		if( node == mylist.pxIndex ) break;
		
		
		myprintf(" val = %08x\r\n",node->xItemValue );
		
		node = node->pxNext;
		
	}
	
	
	
	while(1)
	{
		
		vTaskDelay( pdMS_TO_TICKS( 1000 ) );
		
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


void myprintf( char* format, ... )
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


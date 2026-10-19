#include "wapp.h"
#include "framework.h"


/*
wapp做什么事情呢

write 数据到app区域

数据信息块

操作函数


*/

static wapp_t m_wapp={0,0,0};
wapp_t* wapp = &m_wapp;


static void init( wapp_t* wapp )
{
	if( wapp->init != 0 ) return;
	
	//获取想要操作的fal分区
	const struct fal_partition * ptt= fal_partition_find( "app" );
	if( NULL == ptt )
	{//成立，说明获取失败
		sysprintf("err:%s,%d\r\n",__func__,__LINE__);
		while(1);
	}
	wapp->ptt = (struct fal_partition *)ptt;
	
	wapp->wlen = 0;
	wapp->waddr = 0;	//fal ptt 的分区相对地址
	wapp->init = 1;
	
}

static void erase( wapp_t* wapp )
{
	if( wapp->init == 0 ) return;
	sysprintf( "erase fal_partition %s\r\n",wapp->ptt->name );
	fal_partition_erase_all( wapp->ptt );
	sysprintf( "erase end!\r\n" );

}


static void program( wapp_t* wapp,uint8_t* buf,uint32_t len )
{
	fal_partition_write( wapp->ptt,wapp->waddr,buf,len );
	wapp->waddr += len;
}

static wapp_opt_t do_wapp_opt={
	.init = init,
	.erase = erase,
	.program = program,
	
};

wapp_opt_t* wapp_opt = &do_wapp_opt;






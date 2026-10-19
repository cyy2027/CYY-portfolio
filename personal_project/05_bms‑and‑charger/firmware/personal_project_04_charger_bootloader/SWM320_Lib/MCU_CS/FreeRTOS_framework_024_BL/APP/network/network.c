#include "network.h"
#include "work.h"
#include "wapp.h"

char* serv_ip = "www.armsoc.cn";
char* http_file_path ="/zxx/ota/app_charge_inner_hlw8012.bin";



enum{
	_e_port_init = 0,	//初始化端口
	_e_rst_net_dev,		//复位网络设备
	_e_init_net_dev,	//初始化网络设备
	_e_connect_serv,	//连接服务器
	_e_http_get_file_size,		//获取文件的尺寸
	_e_http_get_file_data,		//文件分片下载
	_e_http_get_file_end,			//文件获取结束
	
	
};


static wk_net_t m_wk_net;
wk_net_t* wk_net = &m_wk_net;

static net_dev_t m_net_dev;
static net_dev_t* air7xx = &m_net_dev;

extern int cmd_reboot( int argc,char** argv ) ;

void network_entry( void )
{
	int val = 0;
	
	if( wdat->storage->net_mode == NetMode_Not ) return;
	
	switch( wk_net->msta )
	{
		//初始化端口
		case _e_port_init:
			
		
			//创建一个网络设备，并指定网络接口,指定复位引脚
			netdev_opt->create_dev( air7xx,_e_uart1,NetRST );
						
		
			//初始化串口
			uart->init( air7xx->dev,115200 );
			uart->open( air7xx->dev );
		
			//初始化复位引脚
			gpio->set_mode( air7xx->RstPin,PIN_MODE_OUTPUT );
			gpio->write( air7xx->RstPin,0 );
		
			//初始化wapp
			wapp_opt->init( wapp );
			wapp_opt->erase( wapp );
			wk_net->msta++;
			break;
		
		//复位网络设备
		case _e_rst_net_dev:
			val = netdev_opt->work_rst_dev( air7xx );
			if( val != 1 ) break;
			//执行到这里，说明网络设备复位完毕
			netdev_opt->clr_sta( air7xx );
			wk_net->msta++;
			wk_net->ssta=0;

			break;
		
		//初始化网络环境
		case _e_init_net_dev:
			val = netdev_opt->work_init_dev( air7xx );
			if( val == 0 ) break;

			//执行到这里，说明成功/或者发生了异常
			netdev_opt->clr_sta( air7xx );
			if( val == -1 )
			{//发生了异常
				wk_net->msta = _e_rst_net_dev;
				wk_net->ssta=0;
			}
			else if( val == 1 )
			{//说明成功
				wk_net->msta++;
				wk_net->ssta=0;
				
			}
			break;
		
		//连接服务器
		//工具获取地址:http://www.armsoc.cn/zxx/doc/share/
			
		case _e_connect_serv:
			val = netdev_opt->work_connect_serv( air7xx,wdat->storage->user_name,wdat->storage->user_pwd,"01" );
			if( val == 0 ) break;
		
			netdev_opt->clr_sta( air7xx );
		
			if( val == 1 )
			{//说明已经成功连接上服务器了
				wk_net->msta ++;
				wk_net->ssta=0;
				
				air7xx->msta = 0;
				air7xx->ssta = 0;
				
				break;
			}
		
			if( val == -1 )
			{//发生了不可预料的错误，直接重启网络
				wk_net->msta = _e_rst_net_dev;
				wk_net->ssta=0;
				break;
			}
			
			if( val == -2 )
			{//发生了网络异常,尝试重连,没有重启网络设备
				wk_net->msta = _e_connect_serv;
				wk_net->ssta = 0;
				
				air7xx->msta = 0;
				air7xx->ssta = 0;
				break;
			}
			break;
		
		case _e_http_get_file_size:
			val = netdev_opt->work_http_get_file_size( air7xx );
			if( val == 0 ) break;
			
			if( val == 1 )
			{//说明已经成功获取文件尺寸
				wk_net->msta ++;
				wk_net->ssta=0;
				
				air7xx->msta = 0;
				air7xx->ssta = 0;
				break;
			}
			
			if( val <0 )
			{//发生了不可预料的错误，直接重启网络
				wk_net->msta = _e_rst_net_dev;
				wk_net->ssta=0;
				break;
			}
			break;
		
		case _e_http_get_file_data:
			val = netdev_opt->work_http_get_file_data( air7xx );
			if( val == 0 ) break;
			
			if( val == 1 )
			{//说明已经成功获取文件所有分片
				wk_net->msta ++;
				wk_net->ssta=0;
				
				air7xx->msta = 0;
				air7xx->ssta = 0;
				
				break;
			}	
		
			if( val <0 )
			{//发生了不可预料的错误，直接重启网络
				wk_net->msta = _e_rst_net_dev;
				wk_net->ssta=0;
				break;
			}
			break;
		
		case _e_http_get_file_end:
			//复位系统
			cmd_reboot( NULL,NULL );
			break;
			
		default:
			break;
	}

}


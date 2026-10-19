
#include "network.h"

file_info_t file_info = {0,0,0,0};

extern char* serv_ip;
extern char* http_file_path;

/*
GET /zxx/ota/app_charge_inner_hlw8012.bin
HTTP/1.1
HOST:www.armsoc.cn
Range:bytes=0-0





返回:
HTTP/1.1 206 Partial Content
Date: Sat, 12 Jul 2025 23:06:37 GMT
Server: Apache/2.4.58 (Ubuntu)
Last-Modified: Sat, 12 Jul 2025 13:08:45 GMT
ETag: "9f74-639bb20678ff0"
Accept-Ranges: bytes
Content-Length: 1
Content-Range: bytes 0-0/40820
Content-Type: application/octet-stream

?


bytes 0-0/

回车换行+Content-Type

*/

enum{
	
	//http请求获取文件尺寸
	_e_http_file_get_size_start = 0,
	_e_http_file_get_size_wait,
	_e_http_file_get_size_work,
	
	
};

static int tlen  = 0;
int air_work_http_get_file_size( net_dev_t* netdev )
{
	int fun_ret = 0;
	
	int ret = 0;
	
	char* txbuf = netdev->txbuf;
	char* rxbuf = netdev->rxbuf;
	
	if( file_info.get_flag ) return 1;
	
	
	switch( netdev->msta )
	{
		case _e_http_file_get_size_start:
			NET_LINE();
			netdev_opt->clr_txbuf( netdev );
			netdev_opt->clr_rxbuf( netdev );
			snprintf( txbuf,Ndev_TX_BUF_LEN,
				"GET %s HTTP/1.1\r\nHOST:%s\r\nRange:bytes=0-0\r\n\r\n\r\n",
				http_file_path,serv_ip );
			netdev_opt->send_str( netdev,txbuf );
			netdev->msta++;
			netdev->ssta=0;
			netdev->otick = get_sys_ticks();
			tlen = 0;
			
			break;
		
		case _e_http_file_get_size_wait:
			netdev->ntick = get_sys_ticks();
			if( (netdev->ntick - netdev->otick) < 200 ) break;
			
			//记录当前的滴答到otick
			netdev->otick = get_sys_ticks();
		
			//切换下一个状态
			netdev->msta++;
			netdev->ssta=0;
			break;
		
		case _e_http_file_get_size_work:
			//接收数据到rxbuf
			tlen = uart->read( netdev->dev,&( rxbuf[netdev->rx_len ]),64 );
			
			for( int i=0;i<tlen;i++ )
			{
				sysprintf( "%c",rxbuf[ netdev->rx_len + i ] );
			}
			
			netdev->rx_len += tlen;
			
			if( netdev->rx_len >= Ndev_RX_BUF_LEN )
			{//检查接收缓冲区
				netdev->rx_len = 0;
				netdev->msta = netdev->msta - 2;	//回退2步
				NET_LINE( );
				break;
			}
			
			
			//处理超时 2s超时, 4次尝试,如果失败，则返回 前2步 ，如果重大错误，自动进入_e_dev_err_msta
			netdev_opt->deal_cmd_timeout( netdev,2000,4,netdev->msta-2,__func__,__LINE__ );
			
			//判断rxbuf里面是否存在 http结束标志 
			if( stropt->findstr( rxbuf,"\r\n\r\n" ) < 0 ) break;
			
			NET_LINE( );
			//借助txbuf来存储临时数据
			ret = stropt->intercept( rxbuf,txbuf,"bytes 0-0/","\r\nContent-Type" );
			if( ret < 0 )
			{
				NET_LOG( "err: unknow\r\n");
				netdev->msta = netdev->msta - 2;	//回退2步
			}
			else
			{
				NET_LOG( "size = %s\r\n",txbuf );
				file_info.file_size = atol( txbuf );
				file_info.get_flag = 1;
				file_info.recv_size  = 0;
				file_info.one_percent_size = file_info.file_size/100;
				file_info.stick = get_sys_ticks();
				NET_LOG( "file_info.file_size = %d\r\n",file_info.file_size );
				
				netdev->msta = _e_dev_done_msta;
			
			}
			break;
		
		//------------------------------------------------------------------------
		case _e_dev_done_msta:
			fun_ret = 1;
			break;
		
		case _e_dev_err_msta:
			//网络灯灭
			//Todo...
			led_board_opt->set_net( __OFF );
			fun_ret = -1;
			break;
		
		case (_e_dev_err_msta+1):
			fun_ret = -2;
			break;
		
		default:
			break;
		
	
	}

	return fun_ret;
}

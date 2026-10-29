#include "network.h"

extern char* serv_ip;


//-----------------------------------------------------------------------------------

enum{
	//------------------------------------------------------------------------
	//连接服务器
	_e_AT_CIPSTART_start = 0,
	_e_AT_CIPSTART_wait,
	_e_AT_CIPSTART_work,
	
	//------------------------------------------------------------------------
	//根据协议，登录服务器
	_e_Login_start,
	_e_Login_wait,
	_e_Login_work,
	
	//------------------------------------------------------------------------
	//进行数据交互
	_e_msg_cycle,
	_e_clr_swap,
	
	//------------------------------------------------------------------------
	//处理服务器断开异常
	_e_AT_CIPCLOSE_start,
	_e_AT_CIPCLOSE_wait,
	_e_AT_CIPCLOSE_work,
	
	//------------------------------------------------------------------------
	//处理超时异常
	_e_deal_timeout,
	
	
	
};

//netdev 网卡
//usr: 终端用户名 
//pwd: 终端密码
//vision: 版本号


static int tlen  = 0;
#define MaxArgc 10
int air_work_connect_serv( net_dev_t* netdev,char* usr,char* pwd,char* vision )
{
	int fun_ret = 0;
	int cmd_ret = 0;
	cmd_ret = cmd_ret;
	char* rxbuf = netdev->rxbuf;
	
	
	
	switch( netdev->msta )
	{
		//------------------------------------------------------------------------
		//连接服务器
		case _e_AT_CIPSTART_start:
			NET_LINE( );
//			netdev_opt->clr_rxbuf( netdev );
//			netdev_opt->send_str(netdev,"AT+CIPSTART=");
//			netdev_opt->send_str(netdev,"\"TCP\"");
//			netdev_opt->send_str(netdev,",");
//			netdev_opt->send_str(netdev,"\"www.armsoc.cn\"");
//			netdev_opt->send_str(netdev,",");
//			netdev_opt->send_str(netdev,"\"80\"");
//			netdev_opt->send_str(netdev,"\r\n");
			
			memset( netdev->txbuf,0,Ndev_TX_BUF_LEN ); 
			snprintf( netdev->txbuf,Ndev_TX_BUF_LEN,"AT+CIPSTART=\"TCP\",\"%s\",\"80\"\r\n",serv_ip ); 
			netdev_opt->send_str( netdev, netdev->txbuf );
		
			netdev->otick = get_sys_ticks();
			tlen = 0;
			netdev->msta++;
		
			break;
		case _e_AT_CIPSTART_wait:
			//等待100ms
			netdev->ntick = get_sys_ticks();
			if( (netdev->ntick - netdev->otick) < 100 ) break;
			
			//记录当前的滴答到otick
			netdev->otick = get_sys_ticks();
		
			//切换下一个状态
			netdev->msta++;
			netdev->ssta=0;
			break;
		case _e_AT_CIPSTART_work:
			//接收数据到rxbuf
			tlen = uart->read( netdev->dev,&( rxbuf[netdev->rx_len ]),64 );
			netdev->rx_len += tlen;
			
			if( netdev->rx_len >= Ndev_RX_BUF_LEN )
			{//检查接收缓冲区
				netdev->rx_len = 0;
				netdev->msta = netdev->msta - 2;	//回退2步
				NET_LINE( );
				break;
			}
			
			//判断是否收到 CONNECT
			if( strstr( rxbuf,"CONNECT" ) != NULL ) 
			{//成立说明连接上服务器
				netdev->msta = _e_dev_done_msta;
				netdev->ssta=0;
				netdev_opt->ptf_rxbuf( netdev,__func__,__LINE__ );
				NET_LINE();
				break;
			}
		
			if( strstr( rxbuf,"ERROR" ) != NULL ) 
			{//成立说明异常
				netdev->msta = _e_dev_err_msta;
				netdev->ssta=0;
				netdev_opt->ptf_rxbuf( netdev,__func__,__LINE__ );
				NET_LINE();
				break;
			}
			
			//处理超时 2s超时, 4次尝试,如果失败，则返回 前2步 ，如果重大错误，自动进入_e_dev_err_msta
			netdev_opt->deal_cmd_timeout( netdev,2000,4,netdev->msta-2,__func__,__LINE__ );
			
			break;
	
		//------------------------------------------------------------------------
		
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
			netdev->msta = _e_dev_err_msta;
			break;
		
		
	}		
	

	return fun_ret;
}







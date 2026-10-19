#include "network.h"
#include "wapp.h"
#include "work.h"

/*
align_file.exe 用于将文件向上对齐到128字节

	220B的文件，经过align_file处理之后，文件会在后面补充若干个0，直到256B

crc_bin_to_bin.exe 用于将文件按每128字节进行crc处理，并把crc补充在128字节后面
	确保文件是128字节对齐情况下，
	[128][128][128]
	经过crc_bin_to_bin处理后，
	[128+2CRC][128+2CRC][128+2CRC]
	处理后的文件尺寸，会膨胀为：file_size/128*130
	

*/


/*
GET /zxx/ota/app_charge_inner_hlw8012.bin
HTTP/1.1
HOST:www.armsoc.cn
Range:bytes=0-0


*/


extern file_info_t file_info;

extern char* serv_ip;
extern char* http_file_path;

static uint16_t crc16_ibm(const void* data, size_t size);

enum{
	//获取文件分片
	_e_http_get_file_part_start = 0,
	_e_http_get_file_part_wait,
	_e_http_get_file_part_head,
	_e_http_get_file_part_data,
	
	//检查文件下完是否完成
	_e_check_file_download_done,
	
};


static int tlen  = 0;

//0-0
char str_range[32];

int air_work_http_get_file_data( net_dev_t* netdev )
{
	int fun_ret = 0;
	uint16_t crc = 0;
	char* txbuf = netdev->txbuf;
	char* rxbuf = netdev->rxbuf;
	
	switch( netdev->msta )
	{
		case _e_http_get_file_part_start:
			NET_LINE();
			netdev_opt->clr_txbuf( netdev );
			netdev_opt->clr_rxbuf( netdev );
			
			snprintf( txbuf,Ndev_TX_BUF_LEN,
				"GET %s HTTP/1.1\r\nHOST:%s\r\nRange:bytes=%d-%d\r\n\r\n\r\n",
				http_file_path,serv_ip,
				file_info.recv_size,file_info.recv_size+129);
			
			memset( str_range,0,sizeof( str_range ) );
			snprintf( str_range,sizeof( str_range ),"%d-%d",
				file_info.recv_size,file_info.recv_size+129 );
			
//			NET_LOG( "-->send:%s\r\n",txbuf );
			netdev_opt->send_str( netdev,txbuf );
			netdev->msta++;
			netdev->ssta=0;
			netdev->otick = get_sys_ticks();
			tlen = 0;
		
			break;
		
		case _e_http_get_file_part_wait:
			//记录当前的滴答到otick
			netdev->otick = get_sys_ticks();
			//切换下一个状态
			netdev->msta++;
			netdev->ssta=0;
			break;
		
		case _e_http_get_file_part_head:
			//接收数据到rxbuf
			tlen = uart->read( netdev->dev,&( rxbuf[netdev->rx_len ]),1 );
			
//			for( int i=0;i<tlen;i++ )
//			{
//				sysprintf( "%c",rxbuf[ netdev->rx_len + i ] );
//			}
			
			netdev->rx_len += tlen;
			
//			break;
			
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
			
			//判断rxbuf里面是否存在 http数据标志 
			if( stropt->findstr( rxbuf,str_range ) < 0 ) break;
			
			netdev->msta++;
			netdev->ssta = 0;
			netdev_opt->clr_rxbuf( netdev );
			netdev->otick = get_sys_ticks();
			
			NET_LINE();
			
			break;
		
		case _e_http_get_file_part_data:
			//继续收取数据，收130个数据
			tlen = uart->read( netdev->dev,&( rxbuf[netdev->rx_len ]),130 );
			netdev->rx_len += tlen;
			
			if( netdev->rx_len >= 130 )
			{//成立，则检验数据，并写入
				
				NET_LINE();
				crc = crc16_ibm( rxbuf,128 );
				if( ( (uint8_t)(crc>>0) ) != rxbuf[128] )
				{//成立，说明收到的数据不对
					netdev->msta = 0;
					netdev->ssta = 0;
					break;
				}
				
				if( ( (uint8_t)(crc>>8) ) != rxbuf[129] )
				{//成立，说明收到的数据不对
					netdev->msta = 0;
					netdev->ssta = 0;
					break;
				}
				
				//执行到这里，说明数据合法，写入数据
				wapp_opt->program( wapp,(uint8_t*)rxbuf,128 );
			
				//进度增加
				file_info.recv_size += 130;
				
				uint32_t percent = file_info.recv_size/file_info.one_percent_size;
				uint32_t dtick = get_sys_ticks() - file_info.stick; //ms
				uint32_t speed = file_info.recv_size *1000/dtick;
				uint32_t etick = (file_info.file_size - file_info.recv_size)/speed;
				
				NET_LOG("\r\n[%02d%%]/[%8d/%8d] %d B/s %d s\r\n",
					percent,file_info.recv_size,file_info.file_size,speed,etick );
				
				//写入本文件片完成，判断是否完成整个文件的下载
				netdev->msta = _e_check_file_download_done;
				netdev->ssta = 0;

				break;
			}
			
			//处理超时 2s超时, 4次尝试,如果失败，则返回 前2步 ，如果重大错误，自动进入_e_dev_err_msta
			netdev_opt->deal_cmd_timeout( netdev,2000,4,netdev->msta-3,__func__,__LINE__ );
			
		
			break;
		
		case _e_check_file_download_done:
			if( file_info.recv_size >= file_info.file_size )
			{
				NET_LOG(" recv success!\r\n");
				netdev->msta = _e_dev_done_msta;
				netdev->ssta = 0;
				wdat->storage->ota_req = 0;
				work_data_write();
			}
			else
			{
				netdev->msta = 0;
				netdev->ssta = 0;
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

static uint16_t crc16_ibm(const void* data, size_t size)
{
    const uint8_t* bytes = (uint8_t*)data;
    uint16_t crc = 0;

    while (size--) {
        crc ^= *bytes++;
        for (int i = 0; i < 8; ++i) {
            crc = (crc >> 1) ^ ((crc & 0x01) ? 0xA001 : 0);
        }
    }

    return crc;
}


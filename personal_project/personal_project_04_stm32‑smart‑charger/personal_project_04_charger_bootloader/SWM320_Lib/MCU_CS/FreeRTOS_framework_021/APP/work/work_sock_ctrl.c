#include "work.h"
/*-------------------------------------------------------------------------*/
//硬件操作
static void open( int road )
{
	switch( road )
	{
		case 0: gpio->write( IO_ELC1,1 ); break;
		case 1: gpio->write( IO_ELC0,1 ); break;
		default: 
			break;
	}
}
static void close( int road )
{
	switch( road )
	{
		case 0: gpio->write( IO_ELC1,0 ); break;
		case 1: gpio->write( IO_ELC0,0 ); break;
		default:
			break;
	}
}
/*-------------------------------------------------------------------------*/
static void recv_hlw_event( int road )
{
	hlw_pul_t event;
	
	int ret = hlw_opt->get( road,&event );
	if( ret <0 ) return;
	
	sock_t* sock = &(wdat->work->sock[road]);
	
	//如果不是在充电过程，则直接丢弃消息
	if( 0 == sock->working ) return;
	
	switch( event.type )
	{
		case _type_hlw_sock_out:
			sock->sock_out = 1;
			
			sysprintf( "%s(%d): sock_out\r\n",__func__,road );
			break;
		
		case _type_hlw_cyc_pul:
			//发送消息给网络任务，让网络任务把电流报文传给服务器
			//TODO
			
			//记录实时周期脉冲
			
			//量化数据
			sock->pwr = ( event.val * wdat->storage->k[road])/(event.cycle/1000);
			
		
			//打印出来
			sysprintf( "%s(%d): cyc_pul:%d  pwr:%f\r\n",__func__,road,event.val,sock->pwr );
			sock->num++;
		
			break;
		
		case _type_hlw_min_pul:
			//增加总电能
			sock->pul += event.val;
		
			//记录分钟电能数
			sock->min_e = event.val * wdat->storage->k[road];	
			
			
			//打印出来
			sysprintf( "%s(%d): min_pul:%d\r\n",__func__,road,event.val );
			break;
		
		default:
			break;
		
	}
	
	
}



/*-------------------------------------------------------------------------*/

enum{
	_e_osc_init = 0,						//初始化
	_e_osc_wait_condition,			//等待条件
	_e_osc_wait_order,					//等待订单
	_e_osc_charging,						//充电中
	
	//约定 msta 处于0xE0~0xEF状态时，为订单结束，并且(msta-0xE0)，表示订单结束的原因
	
};

enum{
	_e_end_by_time = 0,					//订单时间消耗完毕,正常结束
	_e_end_curr_too_small,			//电流过小
	_e_end_curr_too_big,				//电流过大
	_e_end_no_ins,							//没有插入用电器
	_e_end_fuse_err,						//保险丝异常
	_e_end_manual_stop,					//手动结束
};


static void one_sock_ctrl( int road )
{
	uint64_t sval;
	uint64_t eval;
	
	uint32_t ntick = get_sys_ticks();
	sock_t* sock = &(wdat->work->sock[road]);
	switch( sock->msta )
	{
		case _e_osc_init:
			close( road );
			led_board_opt->set_sock( road,0 );
			sock->msta ++;
			sock->ssta = 0;
		
			
		
			break;
		
		case _e_osc_wait_condition:
			sock->msta ++;
			sock->ssta = 0;
			break;
		
		case _e_osc_wait_order:
			//接收HLW8012的消息，并丢弃
			recv_hlw_event(road);
			
			//等待订单，work_deal_order收到订单后，会主动修改 working
			if( sock->working == 0 ) break;
			
			open( road );
			led_board_opt->set_sock( road,1 );
			sock->stick = get_sys_ticks();
			sock->msta ++;
			sock->ssta = 0;
			
			break;
		
		case _e_osc_charging:
			//接收HLW8012的消息，并处理
			recv_hlw_event(road);
#if 1			
			//(1)10s后，检测是否插入插座，如果没有，则结束订单
			if( (ntick - sock->stick)>= (10*1000))
			{
				if( sock->sock_out ) 
				{
					sock->msta = 0xE0+_e_end_no_ins;
					break;
				}
			}
			else
			{
				sock->sock_out = 0;
			}
			
			//(2)2分钟后，检测电能消耗是否过小，如果太小了，则认为充满，订单结束
			if( (ntick - sock->stick)>= (2*60*1000))
			{
				if( sock->min_e <= ( 15*60 ) )
				{//成立，说明周期脉冲太小了，设备已经充满
					sock->msta = 0xE0+_e_end_curr_too_small;
					break;
				}
			}
		
			//(3)10s后，检测电流是否过大，如果过大，则结束订单，保护设备
			if( (ntick - sock->stick)>= (10*1000))
			{
				if( sock->pwr > ( 800 ) )
				{//成立，说明电流太大了
					sock->msta = 0xE0+_e_end_curr_too_big;
					break;
				}
			}
		
			//(4)检测保险丝是否异常，如果异常，则结束订单
			if(  1== fuse->cat_err( road ) )
			{
				sock->msta = 0xE0+_e_end_fuse_err;
				break;
			}
			//(5)检测是否存在手动结束请求，存在，则结束订单
			if(  sock->stop_req )
			{
				sock->msta = 0xE0+_e_end_manual_stop;
				break;
			}
		
			//(6)检测订单时长是否用光，如果用光，则结束订单

			sval = utc_opt->utc_to_timestamp( &(sock->start) );
			eval = utc_opt->utc_to_timestamp( &(sock->end  ) );
			if( eval >= sval )
			{//成立，则订单结束
				sock->msta = 0xE0+_e_end_by_time;
				break;
				
			}
#endif
			
			break;
		
		case 0xE0+_e_end_by_time:
		case 0xE0+_e_end_curr_too_small:
		case 0xE0+_e_end_curr_too_big:
		case 0xE0+_e_end_no_ins:
		case 0xE0+_e_end_fuse_err:
		case 0xE0+_e_end_manual_stop:
			//把结束原因，通知网络任务，网络任务，按照报文的格式，帮我们告知服务器
			//TODO
			sysprintf("--->(%d): event:%d\r\n",road,sock->msta );
			sock->msta = 0;
			sock->ssta = 0;
		
			break;
		
		default:
			break;
	}
}
/*-------------------------------------------------------------------------*/


void work_sock_ctrl(void)
{
	for( int road=0;road<MaxSock;road++)
	{
		one_sock_ctrl( road );
	}
	
}
/*-------------------------------------------------------------------------*/

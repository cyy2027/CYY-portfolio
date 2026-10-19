#ifndef __wapp_h__
#define __wapp_h__

#include "framework.h"
#include "stdlib.h"

typedef struct{
	uint32_t init;		//描述本数据块是否已经初始化
	uint32_t wlen;		//已经写入数据的长度
	uint32_t waddr;		//当前写入的地址
	struct fal_partition * ptt;	//fal分区,app分区
}wapp_t;
extern wapp_t* wapp;

typedef struct{
	void (*init)( wapp_t* wapp );
	void (*erase)( wapp_t* wapp );
	void (*program)( wapp_t* wapp,uint8_t* buf,uint32_t len );

}wapp_opt_t;

extern wapp_opt_t* wapp_opt;

#endif





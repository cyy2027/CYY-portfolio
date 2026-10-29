#include "framework.h"


//切割字符串
//max 最多解析几个参数
//str 需要处理的字符串
//result 存放处理的结果
//delimiters 分隔符 " \t\n\r"
static int split_string_n(int max,char *str, char **result, const char *delimiters) 
{
    int count = 0;
    char *token;
    char *saveptr; // 用于保存上下文的指针

    // 使用 strtok_r 函数拆分字符串
    token = strtok_r(str, delimiters, &saveptr);

    while (token != NULL) {
        result[count] = token; // 保存当前的 token
        count++;
				if( count>=max ) break;
        // 获取下一个 token
        token = strtok_r(NULL, delimiters, &saveptr);
    }

    return count;
}


//字符串查找子串
//成功，返回字符串中子串的偏移量
//失败， 返回-1
static int findstr( char* haystack,char* needle  )
{
	char *a = NULL;
	a = strstr( haystack,needle );
	if( a == NULL  )	return -1;
	return ((uint32_t)a - (uint32_t)haystack);
}

//截取夹住的字符串
//src 被用于分析的字符串源
//dest 用于存储目标位置
//sstr 被夹子串的前面
//estr 被夹子串的后面
/*例如我想把def抽出来 abc-def-ghi
	sstr是 abc-
	estr是 -ghi

*/
static int intercept( char* src,char* dest,char* sstr,char* estr )
{
	//数据被本字段名称和下一个字段名称夹住，pos_field存放2个字段的起始位置
	int pos_field[2];
			
	char *pdat = dest;
	int index = 0;

	int start = 0,end = 0;

	pos_field[0] = findstr( (char*)src, sstr );
	if( pos_field[0] < 0 ) return -1; 
	
	pos_field[1] = findstr( (char*)src, estr );
	if( pos_field[1] < 0 ) return -1; 
	
	//获取字符串数据
	start =  pos_field[0] + strlen( sstr );
	end = pos_field[1] - 1;
	for( index = start; index <= end; index++  )
	{
		*pdat = src[index]; 
		pdat++;
	}
	*pdat = '\0';
	
	return 0;
}


static stropt_t do_stropt={
	.split_string_n = split_string_n,
	.findstr = findstr,
	.intercept = intercept,
};

stropt_t* stropt = &do_stropt;

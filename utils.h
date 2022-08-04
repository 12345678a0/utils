#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct _tag_date_time_st {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} date_time_st;    

#define BSWAP_16(x) \
        (uint16_t) ( (((uint16_t)(x) & 0x00ff) << 8) \
        | (((uint16_t)(x) & 0xff00) >> 8) )
		
#define BSWAP_32(x) \
        (uint32_t) ( (((uint32_t)(x) & 0x000000ff) << 24) \
        | (((uint32_t)(x) & 0x0000ff00) << 8) \
        | (((uint32_t)(x) & 0x00ff0000) >> 8) \
        | (((uint32_t)(x) & 0xff000000) >> 24) )
		
#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)            (((a) < (b)) ? (b) : (a))
#endif

#define BETWEEN(VAL, vmin, vmax) ((VAL) >= (vmin) && (VAL) <= (vmax))
#define ISHEXCHAR(VAL) (BETWEEN(VAL, '0', '9') || BETWEEN(VAL, 'A', 'F') || BETWEEN(VAL, 'a', 'f'))

#define MEMBER_OFFSET(struct_type, member)      ((uint32_t)&((struct_type *)0)->member)

#define CONTAINER_OF(member_ptr, struct_type, member)                       \
    ({                                                                      \
        (struct_type *)((char *)member_ptr - MEMBER_OFFSET(struct_type, member)); \
    })
		
/*
 *@brief 16位数据大小端转换
 *@param x:16位的数据 
 *@return 转换后16位的数据  
 */
uint16_t bswap_16(uint16_t x);

/*
 *@brief 32位数据大小端转换
 *@param x:32位的数据 
 *@return 转换后32位的数据  
 */
uint32_t bswap_32(uint32_t x);	

/*
 *@brief 将BIN数据装换为对应的HEX字符
 *@param p_binstr[in]: BIN数据
 *@param bin_len: BIN数据长度
 *@param p_hexstr[out]: HEX字符
 *@param capital: true(大写)  false(小写) 
 *@return HEX字符长度
 */ 
uint16_t BIN2HEX(uint8_t *p_binstr, uint16_t bin_len, uint8_t *p_hexstr, bool capital);

/*
 *@brief 将HEX字符转换成对应的BIN数据
 *@param p_hexstr[in]: HEX字符
 *@param hex_len: HEX字符长度
 *@param p_binstr[out]: BIN数据
 *@return BIN数据长度
 */ 
uint16_t HEX2BIN(uint8_t *p_hexstr, uint16_t hex_len, uint8_t *p_binstr);

/*
 *@brief 16进制数据转换成字符
 *@param p_hex[in]: 输入的十六进制数据
 *@param hex_len: 输入的十六进制数据长度 
 *@param str[out]: 输出的字符串 
 *@param capital: true(大写)  false(小写) 
 *@return 0(成功) -1(失败)  
 */
int hex2str(uint8_t *p_hex, uint16_t hex_len, char *str, bool capital);	

/*
 *@brief 字符转换成16进制数据
 *@param p_str[in]: 输入的字符
 *@param str_len: 输入的字符长度 
 *@param hex[out]: 输出的十六进制数据
 *@return 0(成功) -1(失败)  
 */
int str2hex(char *p_str, uint16_t str_len, uint8_t *hex);

void user_clock_time_transform(uint32_t utcTime, date_time_st * dateTime);

#endif /* __UTILS_H__  */

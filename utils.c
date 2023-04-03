#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

uint16_t bswap_16(uint16_t x)
{
    return ((x & 0x00ff) << 8) | ((x & 0xff00) >> 8);
}

uint32_t bswap_32(uint32_t x)
{
    return ((x & 0x000000ff) << 24) |
           ((x & 0x0000ff00) << 8) |
           ((x & 0x00ff0000) >> 8) |
           ((x & 0xff000000) >> 24);
}

/*
 *@brief 单个HEX字符转换成对应数值
 *@param hex_ch: 单个HEX字符 
 *@return 返回十六进制数值 0x0-0xF   
 */
int8_t HEX2BYTE(uint8_t hex_ch)
{
    if (BETWEEN(hex_ch, '0', '9'))
    {
        return hex_ch - '0';
    }

    if (BETWEEN(hex_ch, 'a', 'f'))
    {
        return hex_ch - 'a' + 10;
    }

    if (BETWEEN(hex_ch, 'A', 'F'))
    {
        return hex_ch - 'A' + 10;
    }

    return 0x00;
}
 
/*
 *@brief 将0x0-0xF的整数转换为对应的HEX字符
 *@param val: 0x0-0xF 
 *@param capital: true(大写)  false(小写) 
 *@return 返回HEX字符 '0'~'9'或'a'~'f'或'A'~'F'  
 */
char BYTE2HEX(uint8_t val, bool capital)
{
    if (BETWEEN(val, 0, 9))
    {
        return val + '0';
    }
	
	char c = capital ? 'A' : 'a';
	if (BETWEEN(val, 0xA, 0xF))
	{
		return (val - 0xA) + c;
	}	
	
    return '0';
}
 
/*
 *@brief 将BIN数据装换为对应的HEX字符
 *@param p_binstr[in]: BIN数据
 *@param bin_len: BIN数据长度
 *@param p_hexstr[out]: HEX字符
 *@param capital: true(大写)  false(小写) 
 *@return HEX字符长度
 */ 
uint16_t BIN2HEX(uint8_t *p_binstr, uint16_t bin_len, uint8_t *p_hexstr, bool capital)
{
    uint16_t index   = 0;
    uint16_t hex_len = bin_len * 2;

    for (index = 0; index < bin_len; index++)
    {
        p_hexstr[index * 2] = BYTE2HEX((p_binstr[index] >> 4) & 0x0F, capital);
        p_hexstr[(index * 2) + 1] = BYTE2HEX(p_binstr[index] & 0x0F, capital);
    }
	
    return hex_len;
}
 
/*
 *@brief 将HEX字符转换成对应的BIN数据
 *@param p_hexstr[in]: HEX字符
 *@param hex_len: HEX字符长度
 *@param p_binstr[out]: BIN数据
 *@return BIN数据长度
 */ 
uint16_t HEX2BIN(uint8_t *p_hexstr, uint16_t hex_len, uint8_t *p_binstr)
{
    uint16_t bin_len = 0;
    uint16_t index = 0;

    if (hex_len % 2 == 1)
    {
        hex_len -= 1;
    }

    bin_len = hex_len / 2;

    for(index = 0; index < hex_len; index+=2)
    {
        p_binstr[index/2] = ((HEX2BYTE(p_hexstr[index]) << 4) & 0xF0) + HEX2BYTE(p_hexstr[index + 1]); 
    }

    return bin_len;
}


int hex2str(uint8_t *p_hex, uint16_t hex_len, char *str, bool capital)
{
	if ((p_hex == NULL) || (str == NULL) || (hex_len == 0))
	{
		return -1;
	}

	uint8_t hex_h = 0, hex_l = 0;
	char c = capital ? 'A' : 'a';

	while (hex_len --)
	{
		hex_h = p_hex[hex_len] >> 4;
		hex_l = p_hex[hex_len] & 0x0F;
		
		if (hex_h <= 9)
		{
			*(str + 2*hex_len)  = hex_h + '0'; 
		}
		else
		{
			*(str + 2*hex_len)  = hex_h - 10 + c; 
		}

		if (hex_l <= 9)
		{
			*(str + 2*hex_len + 1) = hex_l + '0'; 
		}
		else
		{
			*(str + 2*hex_len + 1) = hex_l - 10 + c; 
		}	
	}
	
	return 0;
}

int str2hex(char *p_str, uint16_t str_len, uint8_t *hex)
{
	if (str_len < 2)
	{
		return -1;
	}

	if (str_len % 2 != 0)
	{
		return -1;
	}
	
	char c;
	uint8_t hex_h = 0, hex_l = 0;
	while (str_len --)
	{
		c = p_str[str_len];
		
		if ((c >= '0') && (c <= '9'))
		{
			if (str_len % 2 == 0)
			{
				hex_h = (c - '0') << 4; 	
			}
			else
			{
				hex_l = c - '0'; 	
			}					
		}
		else if ((c >= 'a') && (c <= 'f'))
		{
			if (str_len % 2 == 0)
			{
				hex_h = 16 * (c - 'a' + 10); 	
			}
			else
			{
				hex_l = c - 'a' + 10; 	
			}	
		}
		else if ((c >= 'A') && (c <= 'F'))
		{
			if (str_len % 2 == 0)
			{
				hex_h = 16 * (c - 'A' + 10); 	
			}
			else
			{
				hex_l = c - 'A' + 10; 		
			}	
		}
		else
		{
			return -1;
		}
		
		if (str_len % 2 == 0)
		{
			hex[str_len/2] = hex_h | hex_l; 		
		}
	}
	
	return 0;
}

void user_clock_time_transform(uint32_t utcTime, date_time_st * dateTime)
{
	int8_t ClockTimeZone = 8;  
	uint32_t a, b, c, d;
	
#define USER_CLOCK_JAN_1970    0x83AA7E80	// 2208988800 | 1970-1900 in seconds	
	uint64_t all = 68569 + 2415021 + (utcTime + USER_CLOCK_JAN_1970 + ClockTimeZone * 3600) / 86400;
#undef USER_CLOCK_JAN_1970	

	a	= (all << 2) / 146097;
	all	= all - ((146097 * a + 3) >> 2);
	b	= (4000 * (all + 1)) / 1461001;
	all	= all - ((1461 * b) >> 2) + 31;
	c	= (80 * all) / 2447;
	d	= (utcTime + ClockTimeZone * 3600);

	dateTime->year		= 100 * (a - 49) + b + c / 11;
	dateTime->month		= c + 2 - (12 * (c / 11));
	dateTime->day		= all - (2447 * c) / 80;
	dateTime->week		= (d / (24 * 3600) + 4) % 7;
	dateTime->week      = (dateTime->week ? dateTime->week : 7);
	dateTime->hour		= (d % 86400) / 3600;
	dateTime->minute	= (d % 3600) / 60;
	dateTime->second	= d % 60;；
}


int main(int argc, char *argv[])
{
	int ret = -1;
	int num = 0;
	int i = 0;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint16_t tmp16 = 0x1234;
	uint32_t tmp32 = 0x12345678;
	
	printf("tmp16: %#x  bswap_16(tmp16): %#x\n", tmp16, bswap_16(tmp16));
	printf("tmp16: %#x  BSWAP_16(tmp16): %#x\n", tmp16, BSWAP_16(tmp16));
	printf("tmp32: %#x  bswap_16(tmp32): %#x\n", tmp16, bswap_16(tmp32));
	printf("tmp32: %#x  BSWAP_32(tmp32): %#x\n", tmp16, BSWAP_32(tmp32));
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint8_t bin_1[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	char hex_1[8*2+1] = {0};
	
	num = BIN2HEX(bin_1, sizeof(bin_1)/sizeof(uint8_t), hex_1, false);
	printf("num: %d  hex: %s\n", num, hex_1);
	
	num = BIN2HEX(bin_1, sizeof(bin_1)/sizeof(uint8_t), hex_1, true);
	printf("num: %d  hex: %s\n", num, hex_1);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint8_t bin_2[11] = {0};
	char *hex_2 = "0123456789abcdefABCDEF";
	num = HEX2BIN(hex_2, strlen(hex_2), bin_2);
	printf("bin_2[len:%d]: ", num);
	for (i = 0; i < num; i ++)
	{
		printf("%#x ", bin_2[i]);	
	}
	printf("\n");
		
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint8_t hex_1_buffer[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	char str_1_buffer[8*2+1] = {0};
	
	ret = hex2str(hex_1_buffer, sizeof(hex_1_buffer)/sizeof(uint8_t), str_1_buffer, false);
	if (ret != 0)
	{
		printf("hex2str is faild...\n");
	}
	printf("str_1_buffer: %s\n", str_1_buffer);

	ret = hex2str(hex_1_buffer, sizeof(hex_1_buffer)/sizeof(uint8_t), str_1_buffer, true);
	if (ret != 0)
	{
		printf("hex2str is faild...\n");
	}
    printf("str_1_buffer: %s\n", str_1_buffer);	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint8_t hex_2_buffer[11] = {0};
	char *str_2_buffer = "0123456789abcdefABCDEF";
	
	//int str2hex(char *str, uint16_t str_len, uint8_t *hex)
	
	ret = str2hex(str_2_buffer, strlen(str_2_buffer), hex_2_buffer);
	if (ret != 0)
	{
		printf("str2hex is faild...\n");
	}
	
	printf("hex_2_buffer[len:%d]: ", 11);
	for (i = 0; i < 11; i ++)
	{
		printf("%#x ", bin_2[i]);	
	}
	printf("\n");
		
	printf("hello...\n");	
	return 0;	
}


















 


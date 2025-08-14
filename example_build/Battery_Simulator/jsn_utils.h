#pragma once
/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_utils.h
 * Author       : Gokhan Akkaya
 * Description  : Util functions related with ThreadX.
 * Project      : VESTEL Common modules project
 * Creation Date: 01.04.2021
 ******************************************************************************************/

#ifndef JSN_UTILS_JSN_UTILS_H_
#define JSN_UTILS_JSN_UTILS_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define INT16_MAX_VALUE             32767
#define UINT16_MAX_VALUE            65535

#define UINT8_AS_STR_MAX_SIZE       3
#define UINT16_AS_STR_MAX_SIZE      5

#define ARRAY_SIZE(array)       (sizeof(array[0])*sizeof(array)/sizeof(array[0]))
#define ARRAY_SIZE2(array)      (sizeof(array)/sizeof(array[0]))

#define JSN_USER_STANDART_LIB
#define jsn_memcmp(s1, s2, n)                   ( jsn_memcmp2((void*)(s1), (void*)(s2), (size_t16 )(n))               )
#define jsn_memmove(s1, s2, n)                  ( jsn_memmove2((void*)(s1), (void*)(s2), (size_t16 )(n))              )
#define jsn_memcpy(s1, s2, n)                   ( jsn_memcpy2((void*)(s1), (void*)(s2), (size_t16 )(n))               )
#define jsn_memcpy_w(s1, s2, n)                 ( jsn_memcpy_w2((void*)(s1), (void*)(s2), (size_t16 )(n))             )
#define jsn_memset(s, c, n)                     ( jsn_memset2((void*)(s), (int )(c), (size_t16 )(n))                  )
#define jsn_strcat(s1, s2)                      ( jsn_strcat2((char*)(s1), (char*)(s2))                               )
#define jsn_strncat(s1, s2, n)                  ( jsn_strncat2((char*)(s1), (char*)(s2), (size_t)(n))                 )
#define jsn_strcmp(s1, s2)                      ( jsn_strcmp2((char*)(s1), (char*)(s2))                               )
#define jsn_strcpy(s1, s2)                      ( jsn_strcpy2((char*)(s1), (char*)(s2))                               )
#define jsn_strlen(s)                           ( jsn_strlen2((char*)(s))											  )

#if(FSP_USED)
#define APP_PARAMETER_NOT_USED(p)                   FSP_PARAMETER_NOT_USED(p)
#else
#define APP_PARAMETER_NOT_USED(p)                   SSP_PARAMETER_NOT_USED(p)
#endif

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef union
{
    unsigned char byte;
    struct
    {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
    } bit;
} byte_t;

// uint4_t definition
typedef union uint4_t
{
    uint8_t u8;
    struct
    {
        uint8_t u4 : 4;
        uint8_t nonused : 4;
    } u4_t;
} uint4_t;

// Sub bits for uint16 definition
typedef union uint16_sub_bits_t
{
    uint16_t u16;
    struct
    {
        uint16_t u15 : 15;
        uint16_t nonused : 1;
    } u15_t;
    struct
    {
        uint16_t u12 : 12;
        uint16_t nonused : 4;
    } u12_t;
} uint16_sub_bits_t;

// Sub bits for uint8 definition
typedef union uint8_sub_bits_t
{
    uint8_t u8;
    struct
    {
        uint8_t u3 : 3;
        uint8_t nonused : 5;
    } u3_t;
    struct
    {
        uint8_t u4 : 4;
        uint8_t nonused : 4;
    } u4_t;
    struct
    {
        uint8_t u5 : 5;
        uint8_t nonused : 3;
    } u5_t;
    struct
    {
        uint8_t u6 : 6;
        uint8_t nonused : 2;
    } u6_t;
    struct
    {
        uint8_t u7 : 7;
        uint8_t nonused : 1;
    } u7_t;
} uint8_sub_bits_t;

typedef struct
{
    uint8_t hysterisis_chk_cnt;
    int32_t base_measurement;
    uint16_t hysterisis_range;
    uint8_t hysterisis_check_num;
} hysterisis_filter_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

uint16_t max_value_uint16(const uint16_t* p_arr, uint8_t len);
uint16_t min_value_uint16(const uint16_t* p_arr, uint8_t len);
void hex_to_str(uint16_t x, char* p_str);
void hex_to_str_old(uint16_t x, char* p_str);
uint8_t ascii_to_hex(uint8_t ascii_val);
uint32_t generate_random_number(void);

//  Replace standard functions.
int jsn_memcmp2(void* s1, void* s2, size_t n);
void* jsn_memmove2(void* s1, void* s2, size_t n);
void* jsn_memcpy2(void* s1, void* s2, size_t n);
void* jsn_memcpy_w2(void* s1, void* s2, size_t n);/* memcpy2() for word unit      */
void* jsn_memset2(void* s, int c, size_t n);
char* jsn_strcat2(char* s1, char* s2);
char* jsn_strncat2(char* s1, char* s2, size_t n);
int jsn_strcmp2(char* s1, char* s2);
char* jsn_strcpy2(char* s1, char* s2);
size_t jsn_strlen2(char* s);
int dayofweek(int d, int m, int y);
bool check_hysterisis(hysterisis_filter_t* filter, int32_t new_measurement);
uint8_t int_to_str(int64_t value, char* p_str);
void jsn_print_text(char* in_buffer, char* in_text);
void jsn_print_int(char* in_buffer, int64_t in_data);
void jsn_print_textint(char* in_buffer, char* in_text, int64_t in_data);

void jsn_print_text_direct(char* in_text);
void jsn_print_int_direct(int64_t in_data);
void jsn_print_textint_direct(char* in_text, int64_t in_data);

void jsn_sprintf(char* p_buffer, uint8_t* p_text, uint8_t buffer_size, int64_t value);
void jsn_sprintf_2var(uint8_t* p_buffer, uint8_t* p_text, uint8_t buffer_size, int64_t first_value,
    int64_t second_value);
unsigned char compare_string(void* string1, void* string2);
uint8_t* strstr_mem_area(uint8_t* searched_mem, uint8_t* p_searched_str, uint32_t memsize, uint32_t strsize);

#endif

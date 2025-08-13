/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_utils.c
 * Author       : Emre Erisgen
 * Description  : Util functions related with ThreadX.
 * Project      : VESTEL Common modules project
 * Creation Date: 01.04.2021
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "jsn_utils.h"
#include "jsn_ring_buffer.h"

#if(RTC_DRV)
#if(FSP_USED)
#include "RTC/fer_rtc_drv.h"
#else
#if (HERACLES_V2)
#include "her2_rtc_drv.h"
#else
#include "RTC/her_rtc_drv.h"
#endif
#endif
#endif

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define BUFFER_MAX_LENGTH                   400

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX jsn_print_mutex =
{ 0 };

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static void reverse(char* p_str, uint8_t len);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: reverse
 * Description  : Reverses the item of the string.
 * Arguments    : char* p_str --> the string to be reverse
 *                cuint8_t len --> item count of the string
 * Return Value : None
 ******************************************************************************************/
static void reverse(char* p_str, uint8_t len)
{
    uint8_t indx = 0;
    uint8_t jndx = 0;
    uint8_t temp = 0;
    jndx = (uint8_t)(len - 1);
    while (indx < jndx)
    {
        temp = (uint8_t)p_str[indx];
        p_str[indx] = p_str[jndx];
        p_str[jndx] = (char)temp;
        indx++;
        jndx--;
    }
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: max_value_uint16
 * Description  : Return the max value from uint16_t array.
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
uint16_t max_value_uint16(const uint16_t* p_arr, uint8_t len)
{
    uint8_t indx = 0;
    uint16_t ret = 0;
    for (; indx < len; indx++)
    {
        if (p_arr[indx] > ret)
            ret = p_arr[indx];
    }
    return ret;
}

/******************************************************************************************
 * Function Name: min_value_uint16
 * Description  : Return the min value from uint16_t array.
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
uint16_t min_value_uint16(const uint16_t* p_arr, uint8_t len)
{
    uint8_t indx = 0;
    uint16_t ret = UINT16_MAX_VALUE;
    for (; indx < len; indx++)
    {
        if (p_arr[indx] < ret)
            ret = p_arr[indx];
    }
    return ret;
}

/******************************************************************************************
 * Function Name: hex_to_str
 * Description  : Convert hex to string.
 * Arguments    : uint16_t - x --> integer value to be converted
 *                char - str[] --> string that is filled out
 * Return Value : None
 ******************************************************************************************/
void hex_to_str(uint16_t x, char* p_str)
{
    uint8_t indx = 0;
    do
    {
        uint8_t tmp = (x % 16);

        if (tmp < 10)
            *(p_str + indx) = (char)((x % 16) + 0x30);

        if (tmp >= 10 && tmp < 17)
            *(p_str + indx) = (char)((x % 16) + 0x37);
        indx++;
        x = x / 16;
    } while (x);
    if ((1 == indx) || (3 == indx))
    {
        *(p_str + indx) = 0x30;
        indx++;
    }
    reverse(p_str, indx);
    *(p_str + indx) = '\0';
}

/******************************************************************************************
 * Function Name: hex_to_str_old
 * Description  : Convert hex to string.
 * Arguments    : uint16_t - x --> integer value to be converted
 *                char - str[] --> string that is filled out
 * Return Value : None
 ******************************************************************************************/
void hex_to_str_old(uint16_t x, char* p_str)
{
    uint8_t indx = 0;
    do
    {
        uint8_t tmp = (x % 16);

        if (tmp < 10)
            *(p_str + indx) = (char)((x % 16) + 0x30);

        if (tmp >= 10 && tmp < 17)
            *(p_str + indx) = (char)((x % 16) + 0x37);
        indx++;
        x = x / 16;
    } while (x);
    reverse(p_str, indx);
    *(p_str + indx) = '\0';
}

/******************************************************************************************
 * Function Name: ascii_to_hex
 * Description  : Convert ascii to hex.
 * Arguments    : uint8_t ascii_val
 * Return Value : None
 ******************************************************************************************/
uint8_t ascii_to_hex(uint8_t ascii_val)
{
    uint8_t ret = 0x00;
    if ((ascii_val > 0x2F) && (ascii_val < 0x3A))
    {
        ret = (uint8_t)(ascii_val - 0x30);
    }
    else if ((ascii_val > 0x40) && (ascii_val < 0x47))
    {
        ret = (uint8_t)(ascii_val - 0x37);
    }
    else if ((ascii_val > 0x60) && (ascii_val < 0x67))
    {
        ret = (uint8_t)(ascii_val - 0x57);
    }
    else
    {
        ret = 0xFF;
    }
    return ret;
}

/******************************************************************************************
 * Function Name: generate_random_number
 * Description  : It generates 32-bit random number.
 * Arguments    : None
 * Return Value : uint32_t
 ******************************************************************************************/
uint32_t generate_random_number(void)
{
    uint32_t rand = 0;
#if(RTC_DRV)
    struct tm temp_time;

    rtc_drv_time_get(&temp_time);
    // create a random num
    rand = (uint32_t)((((uint8_t)temp_time.tm_mday) << 24) | (((uint8_t)temp_time.tm_hour) << 16)
        | (((uint8_t)temp_time.tm_min) << 8) | ((uint8_t)temp_time.tm_sec));
    rand ^= 0x83659047;
#endif
    return rand;
}

/************************************************************************************************
 *  Standard Functions
 ************************************************************************************************/

 /******************************************************************************************
  * Function Name: jsn_memcmp2
  * Description  : Compares two memory areas for n bytes in byte units.
  * Arguments    : void*     s1      ; Pointer to first memory area to be compared
  *              : void*     s2      ; Pointer to second memory area to be compared
  *              : size_t    n       ; Number of bytes to be compared
  * Return Value : == 0                  ; The two memory areas are equal
  *              :  > 0                  ; First memory area (s1) is larger
  *              :  < 0                  ; Second memory area (s2) is larger
  ******************************************************************************************/
int jsn_memcmp2(void* s1, void* s2, size_t n)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* original processing is used          */
    char* ss1 = s1;
    char* ss2 = s2;

    while (n > 0)
    {
        if (*ss1 != *ss2)
        {
            return ((int)*ss1 - (int)*ss2); /* The two memory areas are different   */
        }
        ss1++;
        ss2++;
        n--;
    }
    return 0; /* The two memory areas are equal       */
#else   /* ---------------------------------------------|* Standard library is used             */
    return memcmp(s1, s2, (size_t)n);
#endif
}

/******************************************************************************************
 * Function Name: jsn_memmove2
 * Description  : Data is moved from a area to other area which may overlap
 * Arguments    : void*     s1      ; Pointer to memory area to which data is moved
 *              : void*     s2      ; Pointer to memory area from which data is moved
 *              : size_t    n       ; Number of bytes to be moved
 * Return Value : Pointer to memory area to which data is moved
 ******************************************************************************************/
void* jsn_memmove2(void* s1, void* s2, size_t n)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* original processing is used          */
    char* pp1;
    char* pp2;

    pp1 = (char*)s1;
    pp2 = (char*)s2;
    if (pp2 >= pp1)
    { /* Data are copied from the top when left shift copy is performed.  */
        for (; n > 0; n--)
        {
            *pp1++ = *pp2++;
        }
    }
    else
    { /* Data are copied from the end when right shift copy is performed. */
        pp1 += (n - 1);
        pp2 += (n - 1);
        for (; n > 0; n--)
        {
            *pp1-- = *pp2--;
        }
    }
    return s1;
#else   /* ---------------------------------------------|* Standard library is used             */
    return memmove(s1, s2, (size_t)n);
#endif
}

/******************************************************************************************
 * Function Name: jsn_memcpy2
 * Description  : Copies data for the number of bytes specified by "n" from the memory
 *              : area indicated by "s2" to the memory area indicated by "s1".
 * Arguments    : void*     s1      ; Pointer to memory area to which data is copied
 *              : void*     s2      ; Pointer to memory area from which data is copied
 *              : size_t    n       ; Number of bytes to be copied
 * Return Value : Pointer to memory area to which data is copied
 ******************************************************************************************/
void* jsn_memcpy2(void* s1, void* s2, size_t n)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* original processing is used          */
    char* ss1 = s1;
    char* ss2 = s2;

    while (n > 0)
    {
        *ss1++ = *ss2++;
        n--;
    }
    return s1; /*  Pointer to memory area to which data is copied  */
#else   /* ---------------------------------------------|* Standard library is used             */
    return memcpy(s1, s2, (size_t)n);
#endif
}

/******************************************************************************************
 * Function Name: jsn_memcpy_w2
 * Description  : Copies data for the number of words specified by "n" from the memory
 *              : area indicated by "s2" to the memory area indicated by "s1".
 * Arguments    : void*     s1      ; Pointer to memory area to which data is copied
 *              : void*     s2      ; Pointer to memory area from which data is copied
 *              : size_t    n       ; Number of words to be copied
 * Return Value : Pointer to memory area to which data is copied
 ******************************************************************************************/
void* jsn_memcpy_w2(void* s1, void* s2, size_t n)
{
    short* ss1 = s1;
    short* ss2 = s2;

    while (n > 0)
    {
        *ss1++ = *ss2++;
        n--;
    }
    return s1; /*  Pointer to memory area to which data is copied  */
}

/******************************************************************************************
 * Function Name: jsn_memset2
 * Description  : Sets the data specified by "c" for the number of bytes specified
 *              : by "n" to the memory area indicated by "s".
 * Arguments    : void*         s           ; Pointer to memory area to which data is set
 *              : int           c           ; Data to be set
 *              : size_t        n           ; Number of bytes to be set
 * Return Value : Pointer to memory area to which data is set
 ******************************************************************************************/
void* jsn_memset2(void* s, int c, size_t n)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* original processing is used          */
    char* ss = s;

    while (n > 0)
    {
        *ss++ = (char)c;
        n--;
    }
    return s; /* Pointer to memory area to which data is set  */
#else   /* ---------------------------------------------|* Standard library is used             */
    return memset(s, c, (size_t)n);
#endif
}

/******************************************************************************************
 * Function Name: jsn_strcat2
 * Description  : Links the character string indicated by "s2" to the end of the character
 *              : string indicated by "s1".
 *              : The character string after linkage ends with NULL.
 * Arguments    : char* s1  ; Pointer to character string to which linkage is performed
 *              : char* s2  ; Pointer to character string which is to be linked
 * Return Value : Pointer to character string after linkage
 ******************************************************************************************/
char* jsn_strcat2(char* s1, char* s2)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* Original processing is used          */
    char* ptr = s1;

    while (*s1 != 0)
    {
        s1++; /* Search for end of "s1"               */
    }
    while (*s2 != 0)
    {
        *s1++ = *s2++; /* Link "s2" to "s1"                    */
    }
    *s1 = *s2; /* Add NULL                             */

    return ptr; /* Pointer to character string to which linkage is performed    */

#else   /* ---------------------------------------------|* Standard library is used             */
    return strcat(s1, s2);
#endif
}

/******************************************************************************************
 * Function Name: jsn_strncat2
 * Description  : Links n character of the character string indicated by "s2" to the end of the character
 *              : string indicated by "s1".
 *              : The character string after linkage ends with NULL.
 * Arguments    : char* s1  ; Pointer to character string to which linkage is performed
 *              : char* s2  ; Pointer to character string which is to be linked
 *              : size_t n  ; number of characters to be linked
 * Return Value : Pointer to character string after linkage
 ******************************************************************************************/
char* jsn_strncat2(char* s1, char* s2, size_t n)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* Original processing is used          */
    char* ptr = s1;

    while (*s1 != 0)
    {
        s1++; /* Search for end of "s1"               */
    }
    while ((*s2 != 0) && (n > 0))
    {
        *s1++ = *s2++; /* Link "s2" to "s1"                    */
        n--;
    }
    *s1 = '\0'; /* Add NULL                             */
    return ptr; /* Pointer to character string to which linkage is performed    */
#else   /* ---------------------------------------------|* Standard library is used             */
    return strncat(s1, s2, n);
#endif
}

/******************************************************************************************
 * Function Name: jsn_strcmp2
 * Description  : Compares two character strings which end with NULL in byte units.
 * Arguments    : char* s1  ; Pointer to first character string to be compared
 *              : char* s2  ; Pointer to second character string to be compared
 * Return Value : == 0              ; The two character strings are equal
 *              :  > 0              ; First character string (s1) is longer
 *              :  < 0              ; Second character string (s2) is longer
 ******************************************************************************************/
int jsn_strcmp2(char* s1, char* s2)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* Original processing is used          */
    for (; *s1 == *s2; s1++, s2++)
    {
        if (*s1 == '\0')
        {
            return 0; /*The two character strings are equal   */
        }
    }

    return ((int)*s1 - (int)*s2); /*The two character strings are different*/

#else   /* ---------------------------------------------|* Standard library is used             */
    return strcmp(s1, s2);
#endif
}

/******************************************************************************************
 * Function Name: jsn_strcpy2
 * Description  : Copies the character string (ends with NULL) indicated by "s2" to
 *              : the area indicated by "s1".
 *              : The character string to which copy is performed ends with NULL.
 * Arguments    : char* s1  ; Pointer to character string to which copy is performed
 *              : char* s2  ; Pointer to character string which is copied
 * Return Value : Pointer to character string to which copy is performed
 ******************************************************************************************/
char* jsn_strcpy2(char* s1, char* s2)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* Original processing is used          */
    char* ptr = s1;

    while (*s2 != 0)
    {
        *s1++ = *s2++; /* Copy s2 to s1                        */
    }
    *s1 = *s2; /* Add Null                             */

    return ptr; /*Pointer to character string to which copy is performed    */

#else   /* ---------------------------------------------|* Standard library is used             */
    return strcpy(s1, s2);
#endif
}

/******************************************************************************************
 * Function Name: jsn_strlen2
 * Description  : Obtains the length of the character string (NULL included) indicated by "s".
 * Arguments    : char* s   ; Pointer to character string whose length is to be obtained
 * Return Value : Character string length
 ******************************************************************************************/
size_t jsn_strlen2(char* s)
{
#ifdef JSN_USER_STANDART_LIB /* ---------------------------------|* Original processing is used          */
    char* ptr = s;

    while (*s != 0)
    {
        s++;
    }
    return (size_t)((unsigned long)s - (unsigned long)ptr); /* Character string length              */
#else   /* ---------------------------------------------|* Standard library is used             */
    return ((size_t)strlen(s));
#endif
}

/******************************************************************************************
 * Function Name: dayofweek
 * Description  : Gives the day of the week by given date.
 * Arguments    : int d => day,
 *                int m => month,
 *                int y => year
 * Return Value : returns int
 ******************************************************************************************/
int dayofweek(int d, int m, int y)
{
    static int t[] =
    { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

/******************************************************************************************
 * Function Name: check_hysterisis
 * Description  : Checks if measured value stays in given interval for given number of samples
 * Arguments    : hysterisis_filter_t *filter
 *                int32_t new_measurement
 * Return Value : None
 ******************************************************************************************/
bool check_hysterisis(hysterisis_filter_t* filter, int32_t new_measurement)
{
    int32_t hysterisis_max_val = (int32_t)filter->base_measurement + filter->hysterisis_range;
    int32_t hysterisis_min_val = (int32_t)filter->base_measurement - filter->hysterisis_range;

    if ((hysterisis_min_val <= new_measurement) && (hysterisis_max_val >= new_measurement))
    {
        if (filter->hysterisis_check_num == filter->hysterisis_chk_cnt)
            return true;

        filter->hysterisis_chk_cnt++;
        return false;
    }
    else
    {
        filter->hysterisis_chk_cnt = 0;
        filter->base_measurement = new_measurement;
        return false;
    }
}

/******************************************************************************************
 * Function Name: int_to_str
 * Description  : Converts integer to string
 * Arguments    : int32_t value, char *p_str
 * Return Value : uint8_t len of the output char array
 ******************************************************************************************/
uint8_t int_to_str(int64_t value, char* p_str)
{
    uint8_t length = 0;
    uint8_t arr[20];
    uint8_t digitcount = 0;
    if (value < 0)
    {
        *p_str++ = '-';
        value = -value;
        length++;
    }
    while (digitcount < 20)
    {
        arr[digitcount++] = (uint8_t)((value % 10) + 48);
        value /= 10;
        if (value == 0)
        {
            break;
        }
    }
    length += digitcount;
    while (digitcount != 0)
    {
        *p_str++ = (char)(arr[--digitcount]);
    }
    return length;
}

/******************************************************************************************
 * Function Name: jsn_print_text
 * Description  : Print the input text.
 * Arguments    : char *in_buffer, char* in_text
 * Return Value : None
 ******************************************************************************************/
void jsn_print_text(char* in_buffer, char* in_text)
{
    uint32_t text_length = strlen((const char*)in_text);

    if (BUFFER_MAX_LENGTH < text_length) // TODO GA: BUFFER_MAX_LENGTH macro must be same with the rb array macro so change it later.
        text_length = 0;                    // TODO GA: maybe there is no need to macro, take it as argument.

    uint16_t text_index_counter = 0;
    while (*in_buffer != 0)
    {
        text_index_counter++;
        if (text_index_counter >= BUFFER_MAX_LENGTH)
            break;
        in_buffer++;
    }

    for (uint32_t text_index = 0;
        (text_index < text_length) && ((uint32_t)(BUFFER_MAX_LENGTH - text_index_counter) >= text_length);
        text_index++)
    {
        *in_buffer++ = (char)*in_text++;
    }
}

void jsn_print_text_direct(char* in_text)
{
    if (jsn_print_mutex.tx_mutex_id == 0)
    {
        UINT err_jsn_print_mutex;
        err_jsn_print_mutex = tx_mutex_create(&jsn_print_mutex, (CHAR*)"Jason Print Mutex", TX_INHERIT);
        if (TX_SUCCESS != err_jsn_print_mutex)
        {
            return;
        }
    }

    tx_mutex_get(&jsn_print_mutex, TX_WAIT_FOREVER);
    uint16_t validity_count = 0;

    while (*in_text != 0)
    {
        validity_count++;
        if (validity_count >= BUFFER_MAX_LENGTH)
            break;
#if(RINGBUFFER)
        rb_one_char_put(*in_text++);
#endif
    }
    tx_mutex_put(&jsn_print_mutex);
}

/******************************************************************************************
 * Function Name: jsn_print_int
 * Description  : Print the input integer.
 * Arguments    : char *in_buffer, int64_t in_data
 * Return Value : None
 ******************************************************************************************/
void jsn_print_int(char* in_buffer, int64_t in_data)
{
    char buffer[20];
    uint8_t value_length = 0;
    uint16_t int_index_counter = 0;

    value_length = int_to_str(in_data, buffer);
    while (*in_buffer != 0)
    {
        int_index_counter++;
        if (int_index_counter >= BUFFER_MAX_LENGTH)
            break;
        in_buffer++;
    }
    for (uint8_t value_index = 0;
        (value_index < value_length) && ((BUFFER_MAX_LENGTH - int_index_counter) >= value_length); value_index++)
    {
        *in_buffer++ = buffer[value_index];
    }
}

void jsn_print_int_direct(int64_t in_data)
{
    if (jsn_print_mutex.tx_mutex_id == 0)
    {
        UINT err_jsn_print_mutex;
        err_jsn_print_mutex = tx_mutex_create(&jsn_print_mutex, (CHAR*)"Jason Print Mutex", TX_INHERIT);
        if (TX_SUCCESS != err_jsn_print_mutex)
        {
            return;
        }
    }
    tx_mutex_get(&jsn_print_mutex, TX_WAIT_FOREVER);

    char buffer[30] =
    { 0 };
    uint8_t length = int_to_str(in_data, buffer);
    for (uint8_t index = 0; index < length; index++)
    {
#if(RINGBUFFER)
        rb_one_char_put(buffer[index]);
#endif
    }
    tx_mutex_put(&jsn_print_mutex);
}

/******************************************************************************************
 * Function Name: jsn_print_textint
 * Description  : Print the input integer.
 * Arguments    : char *in_buffer, char* in_text, int64_t in_data
 * Return Value : None
 ******************************************************************************************/
void jsn_print_textint(char* in_buffer, char* in_text, int64_t in_data)
{
    jsn_print_text(in_buffer, in_text);
    jsn_print_int(in_buffer, in_data);

}

void jsn_print_textint_direct(char* in_text, int64_t in_data)
{
    jsn_print_text_direct(in_text);
    jsn_print_int_direct(in_data);
}

///******************************************************************************************
// * Function Name: jsn_sprintf
// * Description  : Same functionality with sprintf.
// * Arguments    : uint8_t *p_buffer, uint8_t *p_text, int64_t value
// * Return Value : None
// ******************************************************************************************/
//void jsn_sprintf(char* p_buffer, uint8_t* p_text, uint8_t buffer_size, int64_t value)
//{
//    uint32_t text_length = strlen((const char*)p_text);
//
//    for (uint8_t text_index = 0; text_index < text_length; text_index++)
//    {
//        *p_buffer++ = (char)*p_text++;
//    }
//
//    char buffer[buffer_size];
//    uint8_t value_length = 0;
//    value_length = int_to_str(value, buffer);
//
//    for (uint8_t value_index = 0; value_index < value_length; value_index++)
//    {
//        *p_buffer++ = buffer[value_index];
//    }
//
//    for (uint8_t fresh_index = 0; fresh_index < (buffer_size - (value_length + text_length)); fresh_index++)
//    {
//        *p_buffer++ = 0;
//    }
//}
//
///******************************************************************************************
// * Function Name: jsn_sprintf_2var
// * Description  : Same functionality with sprintf for 2 variables.
// * Arguments    : uint8_t *p_buffer, uint8_t *p_text, uint8_t buffer_size, int64_t first_value,
// * int64_t second_value
// * Return Value : None
// ******************************************************************************************/
//void jsn_sprintf_2var(uint8_t* p_buffer, uint8_t* p_text, uint8_t buffer_size, int64_t first_value,
//    int64_t second_value)
//{
//    uint32_t text_length = strlen((const char*)p_text);
//
//    for (uint8_t text_index = 0; text_index < text_length; text_index++)
//    {
//        *p_buffer++ = *p_text++;
//    }
//
//    uint8_t buffer[buffer_size];
//    uint8_t first_value_length = 0;
//    uint8_t second_value_length = 0;
//
//    first_value_length = int_to_str(first_value, (char*)buffer);
//
//    for (uint8_t value_index = 0; value_index < first_value_length; value_index++)
//    {
//        *p_buffer++ = buffer[value_index];
//    }
//
//    *p_buffer++ = ';';  // to separate two values.
//    *p_buffer++ = ' ';  // to separate two values.
//
//    second_value_length = int_to_str(second_value, (char*)buffer);
//
//    for (uint8_t value_index = 0; value_index < second_value_length; value_index++)
//    {
//        *p_buffer++ = buffer[value_index];
//    }
//
//    for (uint8_t fresh_index = 0;
//        fresh_index < (buffer_size - (first_value_length + second_value_length + (int)text_length + 2));
//        fresh_index++) // clear the remaining bytes.
//    {
//        *p_buffer++ = 0;
//    }
//}

/******************************************************************************************
 * Function Name: compare_string
 * Description  : Compares the two strings
 * Arguments    : void *string1 void *string2
 * Return Value : unsigned char
 ******************************************************************************************/
unsigned char compare_string(void* string1, void* string2)
{
    unsigned char counter = 0;
    unsigned char continue_compare = 1;
    do
    {
        if (((unsigned char*)string1)[counter] != ((unsigned char*)string2)[counter])
        {
            return 0;
        }
        if ((((unsigned char*)string1)[counter] == 0))
        {
            continue_compare = 0;
        }
        counter++;
    } while (continue_compare);
    return 1;
}

/******************************************************************************************
 * Function Name: strstr_mem_area
 * Description  : Custom strstr function with memory overflow protection.
 * Arguments    : uint8_t *searched_mem: Pointer to memory address where string is searched.
 *                uint8_t *p_searched_str: Pointer to searched string.
 *                uint32_t memsize: Size of the memory where string is searched.
 *                uint32_t strsize: Size of the searched string.
 * Return Value : uint8_t * : Pointer to the first occurrence of searched string if found.
 *                If not found it is returned as null.
 ******************************************************************************************/
uint8_t* strstr_mem_area(uint8_t* searched_mem, uint8_t* p_searched_str, uint32_t memsize, uint32_t strsize)
{
    uint8_t* str_start_adr = searched_mem;
    uint32_t similar_char_num = 0;

    // Check if passed string and search area is valid. If not return str not found before evaluation
    if ((NULL == searched_mem) || (NULL == p_searched_str) || (0 == memsize) || (0 == strsize))
        return NULL;

    for (uint32_t inc = 0; inc < memsize; inc++)
    {
        if (searched_mem[inc] == p_searched_str[similar_char_num])
        {
            similar_char_num++;
            if (1 == similar_char_num)
                str_start_adr = &searched_mem[inc];
            else if (strsize == similar_char_num)
                return str_start_adr;
        }
        else
            similar_char_num = 0;
    }
    return NULL;
}


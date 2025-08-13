/******************************************************************************************
 * © Copyright [2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_ring_buffer.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas RA Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Ring Buffer Module.
 * Project      : VESTEL Common modules project.
 * Creation Date: 21.03.24
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "jsn_ring_buffer.h"

#if(RINGBUFFER)
#include "jsn_utils.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

typedef struct
{
    size_t s_elem;
    size_t n_elem;
    uint8_t* buf;
    uint16_t size_c;
    size_t head;
    size_t tail;

} ring_buffer_t;

typedef struct
{
    bool b_notify_active;
    void (*rb_notify_function)(void);
} ring_buffer_notify_t;

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define MAX_ALLOWED_CHAR                    200
#define BUFFER_SIZE_ROUND(n,s)              (n % s)

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX ringbuffer_mutex =
{ 0 };
static TX_MUTEX rb_mutex =
{ 0 };

ring_buffer_notify_t notification_str[RING_BUFFER_MAX] =
{ 0 };

static ring_buffer_t rb[RING_BUFFER_MAX];
static uint8_t courier_buffer[OPERATION_BUFFER_SIZE];

static uint16_t char_head = 0;
static uint16_t char_tail = 0;
static uint16_t char_size = 0;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static bool ring_buffer_full(ring_buffer_t* rb);
static bool ring_buffer_empty(ring_buffer_t* rb);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: ring_buffer_full
 * Description  : Returns whether the buffer is full or not.
 * Arguments    : ring_buffer_t *p_rb
 * Return Value : uint8_t
 ******************************************************************************************/
static bool ring_buffer_full(ring_buffer_t* p_rb)
{
    return ((p_rb->head - p_rb->tail) == p_rb->n_elem) ? true : false;
}

/******************************************************************************************
 * Function Name: ring_buffer_empty
 * Description  : Returns whether the buffer is empty or not.
 * Arguments    : ring_buffer_t *p_rb
 * Return Value : uint8_t
 ******************************************************************************************/
static bool ring_buffer_empty(ring_buffer_t* p_rb)
{
    return ((p_rb->head - p_rb->tail) == 0U) ? true : false;
}

/*----------------------------------- Public Functions -----------------------------------*/

/*************************** Ring Buffer Implementation Type 1 ****************************/

/******************************************************************************************
 * Function Name: ring_buffer_configure
 * Description  : Configure the ring buffer with desired parameters.
 * Arguments    : rb_attr_t attr_str, uint8_t word_size, uint8_t buff_size, uint8_t *rb_buff
 * Return Value : bool
 ******************************************************************************************/
bool ring_buffer_configure(rb_attr_t* attr_str, uint8_t word_size, uint16_t buff_size, uint8_t* rb_buff)
{
    if (0 == rb_mutex.tx_mutex_id)
    {
        UINT err_log_mutex;
        err_log_mutex = tx_mutex_create(&rb_mutex, (CHAR*)"Ringbuffer Imp Type1 Mutex", TX_INHERIT);
        if (TX_SUCCESS != err_log_mutex)
            return false;
    }

    attr_str->s_elem = (size_t)word_size;
    attr_str->n_elem = (size_t)buff_size;
    attr_str->buffer = rb_buff;

    return true;
}

/******************************************************************************************
 * Function Name: ring_buffer_init
 * Description  : Initialization operations and control of ring buffer (must be called
 * every time that rb_index changed).
 * Arguments    : rb_idx_e rb_idx, rb_attr_t *attr
 * Return Value : bool
 ******************************************************************************************/
bool ring_buffer_init(rb_idx_e rb_idx, rb_attr_t* attr)
{
    tx_mutex_get(&rb_mutex, TX_WAIT_FOREVER);

    bool b_err = false;
    if ((rb_idx < RING_BUFFER_MAX) && (NULL != attr))
    {
        if ((NULL != attr->buffer) && (attr->s_elem > 0))
        {
            /* Check that the size of the ring buffer is a power of 2 */
            if (((attr->n_elem - 1) & attr->n_elem) == 0)
            {
                /* Initialize the ring buffer internal variables */
                rb[rb_idx].head = 0;
                rb[rb_idx].tail = 0;
                rb[rb_idx].buf = attr->buffer;
                rb[rb_idx].s_elem = attr->s_elem;
                rb[rb_idx].n_elem = attr->n_elem;
                rb[rb_idx].size_c = 0;

                b_err = true;
            }
        }
    }

    tx_mutex_put(&rb_mutex);
    return b_err;
}

/******************************************************************************************
 * Function Name: ring_buffer_notify
 * Description  : To get notify when new data put in mechanism.
 * Arguments    : rb_idx_e rb_idx, void (*rb_notify)(void)
 * Return Value : bool
 ******************************************************************************************/
bool ring_buffer_notify(rb_idx_e rb_idx, void (*rb_notify)(void))
{
    if ((rb_idx < RING_BUFFER_MAX) && (NULL != rb_notify))
    {
        notification_str[rb_idx].b_notify_active = true;
        notification_str[rb_idx].rb_notify_function = rb_notify;
        return true;
    }
    else
        return false;
}

/******************************************************************************************
 * Function Name: ring_buffer_get
 * Description  : Get the data from buffer.
 * Arguments    : rb_idx_e rb_idx, void *data
 * Return Value : none
 ******************************************************************************************/
void ring_buffer_get(rb_idx_e rb_idx, void* data)
{
    tx_mutex_get(&rb_mutex, TX_WAIT_FOREVER);

    if ((rb_idx < RING_BUFFER_MAX) && !ring_buffer_empty(&rb[rb_idx]))
    {
        const size_t offset = rb[rb_idx].tail;
        memcpy(data, &(rb[rb_idx].buf[offset]), rb[rb_idx].s_elem);
        rb[rb_idx].tail += rb[rb_idx].s_elem;
        rb[rb_idx].size_c -= (uint16_t)rb[rb_idx].s_elem;

        if (rb[rb_idx].n_elem - BUFFER_SIZE_ROUND(rb[rb_idx].n_elem, rb[rb_idx].s_elem) == rb[rb_idx].tail)
            rb[rb_idx].tail = 0;
    }
    else
    {
        rb[rb_idx].size_c = 0;
    }

    tx_mutex_put(&rb_mutex);
}

/******************************************************************************************
 * Function Name: ring_buffer_put
 * Description  : Put the data to buffer.
 * Arguments    : rb_idx_e rb_idx, const void *data
 * Return Value : none
 ******************************************************************************************/
void ring_buffer_put(rb_idx_e rb_idx, const void* data)
{
    tx_mutex_get(&rb_mutex, TX_WAIT_FOREVER);

    if ((rb_idx < RING_BUFFER_MAX) && !ring_buffer_full(&rb[rb_idx]))
    {
        const size_t offset = rb[rb_idx].head;
        memcpy(&(rb[rb_idx].buf[offset]), data, rb[rb_idx].s_elem);
        rb[rb_idx].head += rb[rb_idx].s_elem;
        rb[rb_idx].size_c += (uint16_t)rb[rb_idx].s_elem;

        if (notification_str[rb_idx].b_notify_active)
        {
            if (NULL != notification_str[rb_idx].rb_notify_function)
                notification_str[rb_idx].rb_notify_function();
        }

        if (rb[rb_idx].n_elem - BUFFER_SIZE_ROUND(rb[rb_idx].n_elem, rb[rb_idx].s_elem) == rb[rb_idx].head)
            rb[rb_idx].head = 0;
    }
    else
    {
        rb[rb_idx].size_c = 0;
    }

    tx_mutex_put(&rb_mutex);
}

/******************************************************************************************
 * Function Name: get_rb_status
 * Description  : Get the size value of the related ring buffer.
 * Arguments    : rb_idx_e rb_number
 * Return Value : uint16_t
 ******************************************************************************************/
uint16_t get_rbsize_status(rb_idx_e rb_number)
{
    tx_mutex_get(&rb_mutex, TX_WAIT_FOREVER);

    uint16_t act_size = rb[rb_number].size_c;

    tx_mutex_put(&rb_mutex);
    return act_size;
}

/*************************** Ring Buffer Implementation Type 2 ****************************/

/******************************************************************************************
 * Function Name: rb_char_put
 * Description  : Put the char to buffer (App specific ring buffer which means it can only
 * be used once from single app purpose).
 * Arguments    : char *data
 * Return Value : None
 ******************************************************************************************/
void rb_char_put(char* data)
{
    if (ringbuffer_mutex.tx_mutex_id == 0)
    {
        UINT err_log_mutex;
        err_log_mutex = tx_mutex_create(&ringbuffer_mutex, (CHAR*)"Ringbuffer Mutex", TX_INHERIT);
        if (TX_SUCCESS != err_log_mutex)
        {
            return;
        }
    }

    tx_mutex_get(&ringbuffer_mutex, TX_WAIT_FOREVER);

    uint16_t data_counter = 0;

    while (*data != 0)
    {
        data_counter++;
        if (data_counter >= LOG_DATA_BUF_SIZE)
            break;

        courier_buffer[char_head] = (uint8_t)*data;
        data++;

        uint16_t head = char_head;
        if (++head >= OPERATION_BUFFER_SIZE)
        {
            head = 0;
        }

        if (head != char_tail)
        {
            if (++char_head >= OPERATION_BUFFER_SIZE)
            {
                char_head = 0;
            }
        }
        else
        {
            char_head = char_tail;
            break;
        }
    }

    tx_mutex_put(&ringbuffer_mutex);
}

/*************************** Ring Buffer Implementation Type 3 ****************************/

/******************************************************************************************
 * Function Name: rb_one_char_put
 * Description  : Put the char to buffer byte by byte (App specific ring buffer which means
 * it can only be used once from single app purpose and uses same buffer with the
 * rb_char_put method [rb_char_get method is used to get the data for both functions]).
 * Arguments    : char *data
 * Return Value : None
 ******************************************************************************************/
void rb_one_char_put(char data)
{
    if (ringbuffer_mutex.tx_mutex_id == 0)
    {
        UINT err_log_mutex;
        err_log_mutex = tx_mutex_create(&ringbuffer_mutex, (CHAR*)"Ringbuffer Mutex", TX_INHERIT);
        if (TX_SUCCESS != err_log_mutex)
        {
            return;
        }
    }

    tx_mutex_get(&ringbuffer_mutex, TX_WAIT_FOREVER);

    courier_buffer[char_head] = (uint8_t)data;

    if (++char_head >= OPERATION_BUFFER_SIZE)
    {
        char_head = 0;
    }

    tx_mutex_put(&ringbuffer_mutex);
}

/******************************************************************************************
 * Function Name: rb_char_get
 * Description  : Get the char from buffer (App specific ring buffer which means it can only
 * be used once from single app purpose).
 * Arguments    : uint8_t *tail_pos, uint16_t *c_size
 * Return Value : uint8_t
 ******************************************************************************************/
uint8_t* rb_char_get(uint16_t* c_size)
{
    if (ringbuffer_mutex.tx_mutex_id == 0)
    {
        UINT err_log_mutex;
        err_log_mutex = tx_mutex_create(&ringbuffer_mutex, (CHAR*)"Ringbuffer Imp Type2 Mutex", TX_INHERIT);
        if (TX_SUCCESS != err_log_mutex)
        {
            return NULL;
        }
    }

    tx_mutex_get(&ringbuffer_mutex, TX_WAIT_FOREVER);
    uint8_t* p_courier_buffer = 0;

    if (char_tail == char_head)
    {
        *c_size = 0;
    }
    else if (char_tail < char_head)
    {
        char_size = (uint16_t)(char_head - char_tail);
        if (char_size > MAX_ALLOWED_CHAR)
        {
            char_size = MAX_ALLOWED_CHAR;
        }
        *c_size = char_size;
        p_courier_buffer = &courier_buffer[char_tail];
        char_tail += char_size;
    }

    else if (char_tail > char_head)
    {
        uint16_t previous_tail = char_tail;
        char_size = (uint16_t)(OPERATION_BUFFER_SIZE - char_tail);
        if (char_size > MAX_ALLOWED_CHAR)
        {
            char_size = MAX_ALLOWED_CHAR;
            char_tail += char_size;
        }
        else
        {
            char_tail = 0;
        }
        *c_size = char_size;
        p_courier_buffer = &courier_buffer[previous_tail];
    }

    tx_mutex_put(&ringbuffer_mutex);

    return p_courier_buffer;
}

#endif

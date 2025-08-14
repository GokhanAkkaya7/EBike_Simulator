/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_crc.c
 * Author       : Gokhan Akkaya
 * Description  : CRC hash module.
 * Project      : VESTEL Common modules project
 * Creation Date: 31.03.2021
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "jsn_crc.h"

#if(CRC)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define CRC8_POLYNOMIAL                 (0x107) // CRC8 Polynomial for TI . "x^8+x^2+x+1"

#define BITS_IN_BYTE_NUM                8
#define MSB_BIT_CHECKER                 (0x80)

// CRC32 DEFINATIONS
#define CRC32_INITIAL_VALUE             ((uint32_t)0xFFFFFFFF)
#define CRC32_XOROUT_VALUE              ((uint32_t)0xFFFFFFFF)

#define CRC32_MPEG2_XOROUT_VALUE        ((uint32_t)0x00000000)
#define CRC32_MPEG2_INITIAL_VALUE       ((uint32_t)0xFFFFFFFF)

#define CRC32_INPUT_REFLECTED           1
#define CRC32_RESULT_REFLECTED          1

/*------------------------------------ Static Variables ----------------------------------*/

static const unsigned short s_crc_tbl[] =
{ 0x0000,
  0x1189,
  0x2312,
  0x329b,
  0x4624,
  0x57ad,
  0x6536,
  0x74bf,
  0x8c48,
  0x9dc1,
  0xaf5a,
  0xbed3,
  0xca6c,
  0xdbe5,
  0xe97e,
  0xf8f7,
  0x1081,
  0x0108,
  0x3393,
  0x221a,
  0x56a5,
  0x472c,
  0x75b7,
  0x643e,
  0x9cc9,
  0x8d40,
  0xbfdb,
  0xae52,
  0xdaed,
  0xcb64,
  0xf9ff,
  0xe876,
  0x2102,
  0x308b,
  0x0210,
  0x1399,
  0x6726,
  0x76af,
  0x4434,
  0x55bd,
  0xad4a,
  0xbcc3,
  0x8e58,
  0x9fd1,
  0xeb6e,
  0xfae7,
  0xc87c,
  0xd9f5,
  0x3183,
  0x200a,
  0x1291,
  0x0318,
  0x77a7,
  0x662e,
  0x54b5,
  0x453c,
  0xbdcb,
  0xac42,
  0x9ed9,
  0x8f50,
  0xfbef,
  0xea66,
  0xd8fd,
  0xc974,
  0x4204,
  0x538d,
  0x6116,
  0x709f,
  0x0420,
  0x15a9,
  0x2732,
  0x36bb,
  0xce4c,
  0xdfc5,
  0xed5e,
  0xfcd7,
  0x8868,
  0x99e1,
  0xab7a,
  0xbaf3,
  0x5285,
  0x430c,
  0x7197,
  0x601e,
  0x14a1,
  0x0528,
  0x37b3,
  0x263a,
  0xdecd,
  0xcf44,
  0xfddf,
  0xec56,
  0x98e9,
  0x8960,
  0xbbfb,
  0xaa72,
  0x6306,
  0x728f,
  0x4014,
  0x519d,
  0x2522,
  0x34ab,
  0x0630,
  0x17b9,
  0xef4e,
  0xfec7,
  0xcc5c,
  0xddd5,
  0xa96a,
  0xb8e3,
  0x8a78,
  0x9bf1,
  0x7387,
  0x620e,
  0x5095,
  0x411c,
  0x35a3,
  0x242a,
  0x16b1,
  0x0738,
  0xffcf,
  0xee46,
  0xdcdd,
  0xcd54,
  0xb9eb,
  0xa862,
  0x9af9,
  0x8b70,
  0x8408,
  0x9581,
  0xa71a,
  0xb693,
  0xc22c,
  0xd3a5,
  0xe13e,
  0xf0b7,
  0x0840,
  0x19c9,
  0x2b52,
  0x3adb,
  0x4e64,
  0x5fed,
  0x6d76,
  0x7cff,
  0x9489,
  0x8500,
  0xb79b,
  0xa612,
  0xd2ad,
  0xc324,
  0xf1bf,
  0xe036,
  0x18c1,
  0x0948,
  0x3bd3,
  0x2a5a,
  0x5ee5,
  0x4f6c,
  0x7df7,
  0x6c7e,
  0xa50a,
  0xb483,
  0x8618,
  0x9791,
  0xe32e,
  0xf2a7,
  0xc03c,
  0xd1b5,
  0x2942,
  0x38cb,
  0x0a50,
  0x1bd9,
  0x6f66,
  0x7eef,
  0x4c74,
  0x5dfd,
  0xb58b,
  0xa402,
  0x9699,
  0x8710,
  0xf3af,
  0xe226,
  0xd0bd,
  0xc134,
  0x39c3,
  0x284a,
  0x1ad1,
  0x0b58,
  0x7fe7,
  0x6e6e,
  0x5cf5,
  0x4d7c,
  0xc60c,
  0xd785,
  0xe51e,
  0xf497,
  0x8028,
  0x91a1,
  0xa33a,
  0xb2b3,
  0x4a44,
  0x5bcd,
  0x6956,
  0x78df,
  0x0c60,
  0x1de9,
  0x2f72,
  0x3efb,
  0xd68d,
  0xc704,
  0xf59f,
  0xe416,
  0x90a9,
  0x8120,
  0xb3bb,
  0xa232,
  0x5ac5,
  0x4b4c,
  0x79d7,
  0x685e,
  0x1ce1,
  0x0d68,
  0x3ff3,
  0x2e7a,
  0xe70e,
  0xf687,
  0xc41c,
  0xd595,
  0xa12a,
  0xb0a3,
  0x8238,
  0x93b1,
  0x6b46,
  0x7acf,
  0x4854,
  0x59dd,
  0x2d62,
  0x3ceb,
  0x0e70,
  0x1ff9,
  0xf78f,
  0xe606,
  0xd49d,
  0xc514,
  0xb1ab,
  0xa022,
  0x92b9,
  0x8330,
  0x7bc7,
  0x6a4e,
  0x58d5,
  0x495c,
  0x3de3,
  0x2c6a,
  0x1ef1,
  0x0f78 };

const uint16_t crc16_table[256] =
{ 0x0000,
  0xC0C1,
  0xC181,
  0x0140,
  0xC301,
  0x03C0,
  0x0280,
  0xC241,
  0xC601,
  0x06C0,
  0x0780,
  0xC741,
  0x0500,
  0xC5C1,
  0xC481,
  0x0440,
  0xCC01,
  0x0CC0,
  0x0D80,
  0xCD41,
  0x0F00,
  0xCFC1,
  0xCE81,
  0x0E40,
  0x0A00,
  0xCAC1,
  0xCB81,
  0x0B40,
  0xC901,
  0x09C0,
  0x0880,
  0xC841,
  0xD801,
  0x18C0,
  0x1980,
  0xD941,
  0x1B00,
  0xDBC1,
  0xDA81,
  0x1A40,
  0x1E00,
  0xDEC1,
  0xDF81,
  0x1F40,
  0xDD01,
  0x1DC0,
  0x1C80,
  0xDC41,
  0x1400,
  0xD4C1,
  0xD581,
  0x1540,
  0xD701,
  0x17C0,
  0x1680,
  0xD641,
  0xD201,
  0x12C0,
  0x1380,
  0xD341,
  0x1100,
  0xD1C1,
  0xD081,
  0x1040,
  0xF001,
  0x30C0,
  0x3180,
  0xF141,
  0x3300,
  0xF3C1,
  0xF281,
  0x3240,
  0x3600,
  0xF6C1,
  0xF781,
  0x3740,
  0xF501,
  0x35C0,
  0x3480,
  0xF441,
  0x3C00,
  0xFCC1,
  0xFD81,
  0x3D40,
  0xFF01,
  0x3FC0,
  0x3E80,
  0xFE41,
  0xFA01,
  0x3AC0,
  0x3B80,
  0xFB41,
  0x3900,
  0xF9C1,
  0xF881,
  0x3840,
  0x2800,
  0xE8C1,
  0xE981,
  0x2940,
  0xEB01,
  0x2BC0,
  0x2A80,
  0xEA41,
  0xEE01,
  0x2EC0,
  0x2F80,
  0xEF41,
  0x2D00,
  0xEDC1,
  0xEC81,
  0x2C40,
  0xE401,
  0x24C0,
  0x2580,
  0xE541,
  0x2700,
  0xE7C1,
  0xE681,
  0x2640,
  0x2200,
  0xE2C1,
  0xE381,
  0x2340,
  0xE101,
  0x21C0,
  0x2080,
  0xE041,
  0xA001,
  0x60C0,
  0x6180,
  0xA141,
  0x6300,
  0xA3C1,
  0xA281,
  0x6240,
  0x6600,
  0xA6C1,
  0xA781,
  0x6740,
  0xA501,
  0x65C0,
  0x6480,
  0xA441,
  0x6C00,
  0xACC1,
  0xAD81,
  0x6D40,
  0xAF01,
  0x6FC0,
  0x6E80,
  0xAE41,
  0xAA01,
  0x6AC0,
  0x6B80,
  0xAB41,
  0x6900,
  0xA9C1,
  0xA881,
  0x6840,
  0x7800,
  0xB8C1,
  0xB981,
  0x7940,
  0xBB01,
  0x7BC0,
  0x7A80,
  0xBA41,
  0xBE01,
  0x7EC0,
  0x7F80,
  0xBF41,
  0x7D00,
  0xBDC1,
  0xBC81,
  0x7C40,
  0xB401,
  0x74C0,
  0x7580,
  0xB541,
  0x7700,
  0xB7C1,
  0xB681,
  0x7640,
  0x7200,
  0xB2C1,
  0xB381,
  0x7340,
  0xB101,
  0x71C0,
  0x7080,
  0xB041,
  0x5000,
  0x90C1,
  0x9181,
  0x5140,
  0x9301,
  0x53C0,
  0x5280,
  0x9241,
  0x9601,
  0x56C0,
  0x5780,
  0x9741,
  0x5500,
  0x95C1,
  0x9481,
  0x5440,
  0x9C01,
  0x5CC0,
  0x5D80,
  0x9D41,
  0x5F00,
  0x9FC1,
  0x9E81,
  0x5E40,
  0x5A00,
  0x9AC1,
  0x9B81,
  0x5B40,
  0x9901,
  0x59C0,
  0x5880,
  0x9841,
  0x8801,
  0x48C0,
  0x4980,
  0x8941,
  0x4B00,
  0x8BC1,
  0x8A81,
  0x4A40,
  0x4E00,
  0x8EC1,
  0x8F81,
  0x4F40,
  0x8D01,
  0x4DC0,
  0x4C80,
  0x8C41,
  0x4400,
  0x84C1,
  0x8581,
  0x4540,
  0x8701,
  0x47C0,
  0x4680,
  0x8641,
  0x8201,
  0x42C0,
  0x4380,
  0x8341,
  0x4100,
  0x81C1,
  0x8081,
  0x4040 };

static const uint32_t s_crc32_tbl[] =
{ 0x00000000,
  0x04C11DB7,
  0x09823B6E,
  0x0D4326D9,
  0x130476DC,
  0x17C56B6B,
  0x1A864DB2,
  0x1E475005,
  0x2608EDB8,
  0x22C9F00F,
  0x2F8AD6D6,
  0x2B4BCB61,
  0x350C9B64,
  0x31CD86D3,
  0x3C8EA00A,
  0x384FBDBD,
  0x4C11DB70,
  0x48D0C6C7,
  0x4593E01E,
  0x4152FDA9,
  0x5F15ADAC,
  0x5BD4B01B,
  0x569796C2,
  0x52568B75,
  0x6A1936C8,
  0x6ED82B7F,
  0x639B0DA6,
  0x675A1011,
  0x791D4014,
  0x7DDC5DA3,
  0x709F7B7A,
  0x745E66CD,
  0x9823B6E0,
  0x9CE2AB57,
  0x91A18D8E,
  0x95609039,
  0x8B27C03C,
  0x8FE6DD8B,
  0x82A5FB52,
  0x8664E6E5,
  0xBE2B5B58,
  0xBAEA46EF,
  0xB7A96036,
  0xB3687D81,
  0xAD2F2D84,
  0xA9EE3033,
  0xA4AD16EA,
  0xA06C0B5D,
  0xD4326D90,
  0xD0F37027,
  0xDDB056FE,
  0xD9714B49,
  0xC7361B4C,
  0xC3F706FB,
  0xCEB42022,
  0xCA753D95,
  0xF23A8028,
  0xF6FB9D9F,
  0xFBB8BB46,
  0xFF79A6F1,
  0xE13EF6F4,
  0xE5FFEB43,
  0xE8BCCD9A,
  0xEC7DD02D,
  0x34867077,
  0x30476DC0,
  0x3D044B19,
  0x39C556AE,
  0x278206AB,
  0x23431B1C,
  0x2E003DC5,
  0x2AC12072,
  0x128E9DCF,
  0x164F8078,
  0x1B0CA6A1,
  0x1FCDBB16,
  0x018AEB13,
  0x054BF6A4,
  0x0808D07D,
  0x0CC9CDCA,
  0x7897AB07,
  0x7C56B6B0,
  0x71159069,
  0x75D48DDE,
  0x6B93DDDB,
  0x6F52C06C,
  0x6211E6B5,
  0x66D0FB02,
  0x5E9F46BF,
  0x5A5E5B08,
  0x571D7DD1,
  0x53DC6066,
  0x4D9B3063,
  0x495A2DD4,
  0x44190B0D,
  0x40D816BA,
  0xACA5C697,
  0xA864DB20,
  0xA527FDF9,
  0xA1E6E04E,
  0xBFA1B04B,
  0xBB60ADFC,
  0xB6238B25,
  0xB2E29692,
  0x8AAD2B2F,
  0x8E6C3698,
  0x832F1041,
  0x87EE0DF6,
  0x99A95DF3,
  0x9D684044,
  0x902B669D,
  0x94EA7B2A,
  0xE0B41DE7,
  0xE4750050,
  0xE9362689,
  0xEDF73B3E,
  0xF3B06B3B,
  0xF771768C,
  0xFA325055,
  0xFEF34DE2,
  0xC6BCF05F,
  0xC27DEDE8,
  0xCF3ECB31,
  0xCBFFD686,
  0xD5B88683,
  0xD1799B34,
  0xDC3ABDED,
  0xD8FBA05A,
  0x690CE0EE,
  0x6DCDFD59,
  0x608EDB80,
  0x644FC637,
  0x7A089632,
  0x7EC98B85,
  0x738AAD5C,
  0x774BB0EB,
  0x4F040D56,
  0x4BC510E1,
  0x46863638,
  0x42472B8F,
  0x5C007B8A,
  0x58C1663D,
  0x558240E4,
  0x51435D53,
  0x251D3B9E,
  0x21DC2629,
  0x2C9F00F0,
  0x285E1D47,
  0x36194D42,
  0x32D850F5,
  0x3F9B762C,
  0x3B5A6B9B,
  0x0315D626,
  0x07D4CB91,
  0x0A97ED48,
  0x0E56F0FF,
  0x1011A0FA,
  0x14D0BD4D,
  0x19939B94,
  0x1D528623,
  0xF12F560E,
  0xF5EE4BB9,
  0xF8AD6D60,
  0xFC6C70D7,
  0xE22B20D2,
  0xE6EA3D65,
  0xEBA91BBC,
  0xEF68060B,
  0xD727BBB6,
  0xD3E6A601,
  0xDEA580D8,
  0xDA649D6F,
  0xC423CD6A,
  0xC0E2D0DD,
  0xCDA1F604,
  0xC960EBB3,
  0xBD3E8D7E,
  0xB9FF90C9,
  0xB4BCB610,
  0xB07DABA7,
  0xAE3AFBA2,
  0xAAFBE615,
  0xA7B8C0CC,
  0xA379DD7B,
  0x9B3660C6,
  0x9FF77D71,
  0x92B45BA8,
  0x9675461F,
  0x8832161A,
  0x8CF30BAD,
  0x81B02D74,
  0x857130C3,
  0x5D8A9099,
  0x594B8D2E,
  0x5408ABF7,
  0x50C9B640,
  0x4E8EE645,
  0x4A4FFBF2,
  0x470CDD2B,
  0x43CDC09C,
  0x7B827D21,
  0x7F436096,
  0x7200464F,
  0x76C15BF8,
  0x68860BFD,
  0x6C47164A,
  0x61043093,
  0x65C52D24,
  0x119B4BE9,
  0x155A565E,
  0x18197087,
  0x1CD86D30,
  0x029F3D35,
  0x065E2082,
  0x0B1D065B,
  0x0FDC1BEC,
  0x3793A651,
  0x3352BBE6,
  0x3E119D3F,
  0x3AD08088,
  0x2497D08D,
  0x2056CD3A,
  0x2D15EBE3,
  0x29D4F654,
  0xC5A92679,
  0xC1683BCE,
  0xCC2B1D17,
  0xC8EA00A0,
  0xD6AD50A5,
  0xD26C4D12,
  0xDF2F6BCB,
  0xDBEE767C,
  0xE3A1CBC1,
  0xE760D676,
  0xEA23F0AF,
  0xEEE2ED18,
  0xF0A5BD1D,
  0xF464A0AA,
  0xF9278673,
  0xFDE69BC4,
  0x89B8FD09,
  0x8D79E0BE,
  0x803AC667,
  0x84FBDBD0,
  0x9ABC8BD5,
  0x9E7D9662,
  0x933EB0BB,
  0x97FFAD0C,
  0xAFB010B1,
  0xAB710D06,
  0xA6322BDF,
  0xA2F33668,
  0xBCB4666D,
  0xB8757BDA,
  0xB5365D03,
  0xB1F740B4 };

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static size_t calculate_crc_n(uint8_t* data_in, size_t length, size_t crc_polynomial, uint8_t crc_byte_size);

// CRC32 PRIVATE FUNCTIONS
static uint32_t CRC32_Reflect32(uint32_t val);
static uint8_t CRC32_Reflect8(uint8_t val);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: calculate_crcn
 * Description  : Calculates the CRC of a given data with the any given crc polynomial and any CRC
 *                calculation given they have byte size bigger than 8.
 * Arguments    : uint8_t* data_in          -Pointer to the inspected byte
 *                size_t length             -Number of bytes in pointed data
 *                size_t crc_polynomial     -Polynomial used for crc calculation
 *                uint8_t crc_byte_size     -Bit size of the CRC operation
 * Return Value : size_t crc                -CRC value of the given data
 *****************************************************************************************/
static size_t calculate_crc_n(uint8_t* data_in, size_t length, size_t crc_polynomial, uint8_t crc_byte_size)
{
    size_t crc_msb_bit_chk = (size_t)(MSB_BIT_CHECKER << (crc_byte_size - BITS_IN_BYTE_NUM));
    size_t inc = crc_msb_bit_chk; // It is done in order to define size of increment value
    size_t crc = crc_msb_bit_chk; // It is done in order to define size of the crc value

    crc = 0;

    for (inc = 0; inc < length; inc++) // Continue XOR operation for
    {
        crc ^= (size_t)(data_in[inc] << (size_t)(crc_byte_size - BITS_IN_BYTE_NUM)); // XOR-in the next input byte

        for (int ind = 0; ind < BITS_IN_BYTE_NUM; ind++)
        {
            if ((crc & crc_msb_bit_chk) != 0)
            {
                // XOR the byte with crc polynomial for crc check
                crc = (uint8_t)((crc << 1) ^ crc_polynomial);
            }
            else
            {
                // Shift bytes until first bit is 1
                crc <<= 1;
            }
        }
    }
    return crc;
}

/******************************************************************************************
 * Function Name: CRC32_Reflect32
 * Description  : It reflect given uint32_t value.
 * Arguments    : uint32_t val
 * Return Value : uint32_t
 ******************************************************************************************/
static uint32_t CRC32_Reflect32(uint32_t val)
{
    uint32_t resVal = 0;

    for (uint8_t i = 0; i < 32; i++)
    {
        if ((val & ((uint32_t)(1 << i))) != 0)
        {
            resVal |= (uint32_t)(1 << (31 - i));
        }
    }
    return resVal;
}

/******************************************************************************************
 * Function Name: CRC32_Reflect8
 * Description  : It reflect given uint8_t value.
 * Arguments    : uint8_t val
 * Return Value : uint8_t
 ******************************************************************************************/
static uint8_t CRC32_Reflect8(uint8_t val)
{
    uint8_t resVal = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        if ((val & (1 << i)) != 0)
        {
            resVal |= (uint8_t)(1 << (7 - i));
        }
    }
    return resVal;
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: crc_apigen
 * Description  : Generate CRC16 with the given data.
 * Arguments    : const unsigned char * const dat_ptr
 *                const unsigned char dat_len
 * Return Value : unsigned short
 ******************************************************************************************/
unsigned short crc_apigen(const unsigned char* const dat_ptr, const unsigned char dat_len)
{
    const unsigned char* l_dat_ptr = dat_ptr;
    unsigned char l_dat_len = dat_len;
    unsigned short l_crc = 0xFFFF;

    while (l_dat_len > 0)
    {
        l_crc = (l_crc >> 8) ^ s_crc_tbl[(unsigned char)(*l_dat_ptr ^ l_crc)];
        ++l_dat_ptr;
        --l_dat_len;
    }
    return (unsigned short)(~l_crc);
}

/******************************************************************************************
 * Function Name: crc_apichk
 * Description  : Check given CRC16.
 * Arguments    : const unsigned char * const dat_ptr
 *                const unsigned char dat_len
 * Return Value : unsigned char
 ******************************************************************************************/
unsigned char crc_apichk(const unsigned char* const dat_ptr, const unsigned char dat_len)
{
    const unsigned char* l_dat_ptr = dat_ptr;
    unsigned char l_dat_len = dat_len;
    unsigned short l_crc = 0xFFFF;

    while (l_dat_len > 0)
    {
        l_crc = (l_crc >> 8) ^ s_crc_tbl[(unsigned char)(*l_dat_ptr ^ l_crc)];
        ++l_dat_ptr;
        --l_dat_len;
    }
    return (l_crc == 0xF0B8) ? true : false;
}

/******************************************************************************************
 * Function Name: calculate_crc8
 * Description  : Calculates the CRC8 of a given data with the previously defined CRC Polynomial .
 * Arguments    : uint8_t* input  -Pointer to the inspected data
 *                size_t length   -Bytesize of the given data
 * Return Value : uint8_t         -Calculated CRC8 value for given data
 *****************************************************************************************/
uint8_t calculate_crc8(uint8_t* input, size_t length)
{
    const uint8_t crc_polynomial = (uint8_t)(CRC8_POLYNOMIAL);
    return (uint8_t)(calculate_crc_n(input, length, crc_polynomial, 8));
}

/******************************************************************************************
 * Function Name: crc32_compute
 * Description  : Generate CRC32 with the given data using CRC32 table.
 * Arguments    : uint8_t const * const data_ptr
 *                const unsigned char dat_len
 * Return Value : size_t const data_len
 ******************************************************************************************/
uint32_t crc32_compute(uint8_t const* const data_ptr, size_t const data_len)
{
    uint32_t crc = CRC32_INITIAL_VALUE;
    for (size_t i = 0; i < data_len; i++)
    {
        /* reflect input byte if specified, otherwise input byte is taken as it is */
        uint8_t curByte;
        if (CRC32_INPUT_REFLECTED)
            curByte = CRC32_Reflect8(data_ptr[i]);
        else
            curByte = data_ptr[i];

        /* XOR-in next input byte into MSB of crc and get this MSB, that's our new intermediate divident */
        uint8_t pos = (uint8_t)((crc ^ ((uint32_t)curByte << 24)) >> 24);
        /* Shift out the MSB used for division per lookuptable and XOR with the remainder */
        crc = (uint32_t)((crc << 8) ^ (uint32_t)(s_crc32_tbl[pos]));
    }
    if (CRC32_RESULT_REFLECTED)
        crc = CRC32_Reflect32(crc);

    return (uint32_t)(crc ^ CRC32_XOROUT_VALUE);
}

/******************************************************************************************
 * Function Name: crc32_mpeg2_compute
 * Description  : Generate CRC32 with the given data using CRC32 table.
 * Arguments    : uint8_t const * const data_ptr
 *                const unsigned char dat_len
 * Return Value : size_t const data_len
 ******************************************************************************************/
uint32_t crc32_mpeg2_compute(uint8_t const* const dat_ptr, size_t const dat_len)
{

    uint32_t crc = CRC32_MPEG2_INITIAL_VALUE;
    for (size_t i = 0; i < dat_len; i++)
    {
        /* reflect input byte if specified, otherwise input byte is taken as it is */
        uint8_t curByte;
        curByte = dat_ptr[i];
        /* XOR-in next input byte into MSB of crc and get this MSB, that's our new intermediate divident */
        uint8_t pos = (uint8_t)((crc ^ ((uint32_t)curByte << 24)) >> 24);
        /* Shift out the MSB used for division per lookuptable and XOR with the remainder */
        crc = (uint32_t)((crc << 8) ^ (uint32_t)(s_crc32_tbl[pos]));
    }
    return (uint32_t)(crc ^ CRC32_MPEG2_XOROUT_VALUE);
}

/******************************************************************************************
 * Function Name: crc16
 * Description  : Generate CRC16 with the given data using CRC16 table.
 * Arguments    : uint8_t *pBuf, int nLen
 *
 * Return Value : uint16_t
 ******************************************************************************************/
uint16_t crc16(uint8_t* pBuf, uint8_t nLen)
{
    uint16_t crc16_res = 0xFFFF;
    int i;

    for (i = 0; i < nLen; i++)
    {
        crc16_res ^= (uint16_t)((*pBuf++) & 0x00FF);
        crc16_res = (uint16_t)(crc16_table[crc16_res & 0x00FF] ^ (crc16_res >> 8));
    }
    return crc16_res;
}

#endif

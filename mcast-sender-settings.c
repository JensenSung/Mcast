/*!
 * @brief
 * @file mcast-sender-settings.c
 * @author T.Ostaszewski
 * @date Jan-2012
 */
#include "pcc.h"
#include "resource.h"
#include "mcast-sender-settings.h"
#include "wave_utils.h"

/*!
 * @brief 
 */
#define DEFAULT_MCASTADDRV4 "234.5.6.7"

/*!
 * @brief 
 */
#define DEFAULT_MCASTADDRV6 "ff12::1"

/*!
 * @brief 
 *
 */
#define DEFAULT_MCASTPORT (25000)

/*!
 * @brief 
 */
#define DEFAULT_CHUNK_SEND_TIMEOUT (85)

/*!
 * @brief
 */
#define DEFAULT_TTL (8)

/*!
 * @brief 
 */
#define DEFAULT_WAV_CHUNK_SIZE    (1024+256+128)

int get_default_settings(HINSTANCE hInst, struct sender_settings * p_settings)
{
	int result;
	result = init_master_riff(&p_settings->chunk_, hInst, MAKEINTRESOURCE(IDR_0_1));
	assert(0 == result);
	if (0 == result) 
	{
		p_settings->send_delay_ = DEFAULT_CHUNK_SEND_TIMEOUT;
		p_settings->ipv4_mcast_group_addr_ = inet_addr(DEFAULT_MCASTADDRV4);
		p_settings->mcast_port_ = DEFAULT_MCASTPORT;
		p_settings->ttl_ = DEFAULT_TTL;
		p_settings->chunk_size_ = DEFAULT_WAV_CHUNK_SIZE;
	}
	return result;
}

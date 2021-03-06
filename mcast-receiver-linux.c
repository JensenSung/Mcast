
#include "pcc.h"

#include <assert.h>
#include "mcast-settings.h"
#include "mcast_utils.h"
#include "wave_utils.h"

#define INTEFACE_BIND_ADDRESS "0.0.0.0"
#define MCAST_GROUP_ADDRESS "239.0.0.1"
#define MCAST_PORT_NUMBER "25000"
#define CHUNK_SIZE (1024)
#define DEFAULT_TTL (5)
#define DEFAULT_SLEEP_TIME ((1000000*1024)/8000)

static uint8_t g_input_buffer[CHUNK_SIZE];

static void dump_addrinfo(FILE * fp, struct addrinfo const * p_addr)
{
    struct addrinfo const * p_idx = p_addr;
    for (; NULL != p_idx; p_idx = p_idx->ai_next)
    {
        struct sockaddr_in const * p_sock_addr_in = (struct sockaddr_in const *)p_idx->ai_addr;
        uint8_t const * in_addr = (uint8_t const *)&p_sock_addr_in->sin_addr;
        printf("%hhu.%hhu.%hhu.%hhu\n", in_addr[0], in_addr[1], in_addr[2], in_addr[3]);
    }
}

void dump_buffer(const uint8_t * p_buffer, size_t length, struct sockaddr * p_from, socklen_t from_length)
{
    fprintf(stderr, "%4.4u %s : %u \n", __LINE__, __func__, from_length);
}

static int set_reuse_addr(SOCKET s)
{
    int optval, rc;
    optval = 1;
    rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
    if (rc == SOCKET_ERROR)
    {
        return -1;
    }
    return 0;
}

volatile sig_atomic_t g_stop_processing;

static void sigint_handle(int signal)
{
    g_stop_processing = 1;
}

int main(int argc, char ** argv)
{
    int result;
    fd_set read_fd;
    struct addrinfo * p_group_address;
    struct addrinfo * p_iface_address;
    struct addrinfo a_hints;
    memset(&a_hints, 0, sizeof(a_hints));
    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
    assert(s>=0); 
    a_hints.ai_family = AF_INET;
    a_hints.ai_protocol = 0;
    a_hints.ai_socktype = SOCK_DGRAM;
    result = getaddrinfo(MCAST_GROUP_ADDRESS, MCAST_PORT_NUMBER, &a_hints, &p_group_address);
    assert(0 == result);
    dump_addrinfo(stderr, p_group_address);
    a_hints.ai_flags = AI_PASSIVE;
    result = getaddrinfo(INTEFACE_BIND_ADDRESS, MCAST_PORT_NUMBER, &a_hints, &p_iface_address);
    assert(0 == result);
    result = set_reuse_addr(s);
    assert(0 == result);
    dump_addrinfo(stderr, p_iface_address);
    result = join_mcast_group_set_ttl(s, p_group_address, p_iface_address, DEFAULT_TTL);
    assert(0 == result);
    {
        struct sigaction query_action;
        memset(&query_action, 0, sizeof(query_action));
        query_action.sa_handler = &sigint_handle;
        if (sigaction (SIGINT, NULL, &query_action) < 0)
            exit(EXIT_FAILURE);
            /* sigaction returns -1 in case of error. */
    }
    while (!g_stop_processing)
    {
        ssize_t bytes_read;
        struct sockaddr recv_from_data;
        socklen_t recv_from_length = sizeof(recv_from_data);
        struct timeval select_timeout = { 1, 0 };
        FD_ZERO(&read_fd);
        FD_SET(s, &read_fd);
        result = select(s+1, &read_fd, NULL, NULL, &select_timeout); 
        switch (result)
        {
            case -1:
                break;
            case 0: 
                fprintf(stdout, "%4.4u %s : Timeout\n", __LINE__, __func__);
                break;
            default:
                if (FD_ISSET(s, &read_fd))
                {
                    bytes_read = recvfrom(s, 
                        &g_input_buffer[0], 
                        sizeof(g_input_buffer), 
                        0, 
                        &recv_from_data, 
                        &recv_from_length);  
                    if (bytes_read >= 0)
                    {
                        fprintf(stdout, "%4.4u %s : %u %s %u %2.2hhx %2.2hhx..\n", __LINE__, __func__, bytes_read, 
                                inet_ntoa((((struct sockaddr_in *)&recv_from_data)->sin_addr)),
                                ntohs((((struct sockaddr_in *)&recv_from_data)->sin_port)),
                                g_input_buffer[0], 
                                g_input_buffer[1]
                               );
                    }
                    else
                    {
                        fprintf(stderr, "%4.4u %s : %d %s\n", __LINE__, __func__, errno, strerror(errno));
                    }
                }
                break;
        }
    }
    return 0;
}


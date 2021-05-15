#ifndef HALLOS_OSAL_H_INCLUDED
#define HALLOS_OSAL_H_INCLUDED

namespace hallos
{
namespace os
{

enum socket_type
{
    TCP,
    UDP
};

int socket_create(socket_type type);

bool socket_bind(int socket, int port);

bool socket_listen(int socket);

void socket_close(int socket);

int socket_accept(int socket);

int socket_receive(int socket, char* buffer, int buffer_size);

int socket_send(int socket, const char* buffer, int buffer_size);

int socket_set_so_linger(int socket, int arg1, int arg2);

}; // namespace os
}; // namespace hallos

#endif
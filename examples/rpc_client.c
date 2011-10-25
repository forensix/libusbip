/**
 * libusbip - rpc_client.h (Remote Procedure Call Client)
 * Copyright (C) 2011 Manuel Gebele
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "libusbip.h"

static int
create_client_socket()
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return -1;
    
    return sock;
}

static int
try_connect(int sock, const char *ip, int port)
{
    struct sockaddr_in server;
    int success;
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    
    success = inet_aton(ip, &(server.sin_addr));
    if (!success)
        return -1;
    
    return connect(sock,
                   (struct sockaddr *)&server,
                   sizeof server);
}

static int
connect_or_die(const char *ip, int port)
{
    int sock = create_client_socket();
    
    if (sock == -1) {
        fprintf(stderr, "Error! Failed to create a client socket: %s\n", ip);
        exit(1);
	}
    
    if (try_connect(sock, ip, port) == -1) {
        fprintf(stderr, "Error! Failed to connect to a server at: %s\n", ip);
        exit(1);
    }
    
    return sock;
}

int main(int argc, char *argv[]) {
    libusbip_error_t error;
    libusbip_ctx_t ctx = LIBUSBIP_CTX_CLIENT;
    struct libusbip_connection_info ci;
    int port;
    
    if (argc < 3) {
        printf("rpc_client: <server-ip> <server-port>\n");
        goto fail;
    }
    
    bzero(&ci, sizeof(struct libusbip_connection_info));

    // Setup session
    sscanf(argv[2], "%d", &port);
    ci.server_sock = connect_or_die(argv[1], port);
    
    error = libusbip_init(&ci, ctx);
    if (error < 0) {
        printf("rpc_client: libusbip_init failed\n");
        goto fail;
    }
    
    libusbip_exit(&ci, ctx);
    
    return 0;
fail:
    return 1;
}

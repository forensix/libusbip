/**
 * libusbip - idev_cmd.c (Example of async transfers.)
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
create_client_socket() {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return -1;
    
    return sock;
}

static int
try_connect(int sock, const char *ip, int port) {
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
connect_or_die(const char *ip, int port) {
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

static void
send_cmd(struct libusbip_connection_info *ci, struct libusbip_device_handle *dh, char *cmd) {
    unsigned int length = strlen(cmd);
	if (length >= 0x100) {
		length = 0xFF;
	}
    
    libusbip_control_transfer(ci, dh, 0x40, 0, 0, 0, cmd, length + 1, 1000);

    printf("[*] idev_cmd: sent command '%s'\n", cmd);
}

int
main(int argc, char *argv[]) {
    libusbip_error_t error;
    struct libusbip_connection_info ci;
    struct libusbip_device_handle dh;
    int vid = 0x05AC;
    int pid = 0x1281;

    int port;
    
    if (argc < 4) {
        printf("idev_cmd: <server-ip> <server-port> <cmd>\n");
        goto fail;
    }
    
    bzero(&ci, sizeof(struct libusbip_connection_info));
    bzero(&dh, sizeof(struct libusbip_device_handle));

    // Setup session
    sscanf(argv[2], "%d", &port);
    ci.server_sock = connect_or_die(argv[1], port);
    ci.ctx = LIBUSBIP_CTX_CLIENT;
    
    error = libusbip_init(&ci);
    if (error < 0) {
        printf("idev_cmd: libusbip_init failed\n");
        goto fail;
    }
    
    libusbip_open_device_with_vid_pid(&ci, &dh, vid, pid);
    if (dh.session_data == 0) {
        printf("idev_cmd: No iDevice found!\n");
        goto exit_fail;

    }
    
    error = libusbip_set_configuration(&ci, &dh, 1);
    if (error < 0) {
        printf("[*] idev_cmd: libusbip_set_configuration failed\n");
        goto close_fail;
    }
    
    error = libusbip_claim_interface(&ci, &dh, 1);
    if (error < 0) {
        printf("[*] idev_cmd: libusbip_claim_interface failed\n");
        goto close_fail;
    }
    
    error = libusbip_set_interface_alt_setting(&ci, &dh, 1, 1); // kRecoveryMode2
    if (error < 0) {
        printf("[*] idev_cmd: libusbip_set_interface_alt_setting failed\n");
        goto release_fail;
    } 
    
    error = libusbip_release_interface(&ci, &dh, 1);
    if (error < 0) {
        printf("[*] idev_cmd: libusbip_release_interface failed\n");
        goto close_fail;
    }
    
    send_cmd(&ci, &dh, argv[3]);
    
    libusbip_close(&ci, &dh);
    libusbip_exit(&ci);
    
    return 0;
release_fail:
    libusbip_release_interface(&ci, &dh, 1);
close_fail:
    libusbip_close(&ci, &dh);
exit_fail:
    libusbip_exit(&ci);
fail:
    return 1;
}

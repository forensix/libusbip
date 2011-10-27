/**
 * libusbip - get_serial.c (Prints the serial string of an attached iDevice.)
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

struct idev_info {
    int index;
    uint16_t serial;
};

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

struct idev_info
idevice_available(struct libusbip_connection_info *ci, libusbip_ctx_t ctx,
                  struct libusbip_device_list *dl) {
    
    int max = dl->n_devices;
    int i;
    struct idev_info info;
    
    bzero(&info, sizeof(struct idev_info));
    
    for (i = 0; i < max; i++) {
        libusbip_error_t error;
        struct libusbip_device *idev = &dl->devices[i];
        struct libusbip_device_descriptor dd;
        error = libusbip_get_device_descriptor(ci, ctx, idev, &dd);
        if (error < 0)
            continue;
        if (dd.idVendor != 0x05AC)
            continue;
        if (dd.idProduct == 0x1280
            || dd.idProduct == 0x1281
            || dd.idProduct == 0x1282
            || dd.idProduct == 0x1283
            || dd.idProduct == 0x1227) {
            printf("[*] get_serial: found iDevice in recovery mode (%04x:%04x %u)\n",
                   dd.idVendor, dd.idProduct, idev->session_data);
            info.serial = dd.iSerialNumber;
            info.index = i;
            goto done;
        }
    }    
done:
    return info;
}


int main(int argc, char *argv[]) {
    libusbip_error_t error;
    libusbip_ctx_t ctx = LIBUSBIP_CTX_CLIENT;
    struct libusbip_device *idev;
    struct libusbip_connection_info ci;
    struct libusbip_device_list dl;
    struct libusbip_device_handle dh;
    struct idev_info info;
    int port, idx, conf;
    
    if (argc < 3) {
        printf("get_serial: <server-ip> <server-port>\n");
        goto fail;
    }
    
    bzero(&ci, sizeof(struct libusbip_connection_info));
    bzero(&dl, sizeof(struct libusbip_device_list));
    bzero(&dh, sizeof(struct libusbip_device_handle));
    bzero(&info, sizeof(struct idev_info));
    
    // Setup session
    sscanf(argv[2], "%d", &port);
    ci.server_sock = connect_or_die(argv[1], port);
    
    error = libusbip_init(&ci, ctx);
    if (error < 0) {
        printf("[*] get_serial: libusbip_init failed\n");
        goto fail;
    }
    
    libusbip_get_device_list(&ci, ctx, &dl);
    
    info = idevice_available(&ci, ctx, &dl); 
    if (!info.index) {
        printf("[*] get_serial: No iDevice found!\n");
        goto exit_fail;
    }
    
    // Open
    idev = &dl.devices[info.index];
    error = libusbip_open(&ci, ctx, idev, &dh);
    if (error < 0) {
        printf("[*] get_serial: libusbip_open failed\n");
        goto exit_fail;
    }
    
    // Get Configuration
    conf = 0;
    error = libusbip_get_configuration(&ci, ctx, &dh, &conf);
    if (error < 0) {
        printf("[*] get_serial: libusbip_get_configuration failed\n");
        goto close_fail;
    }
    
    if (conf != 1) {
        // Set Configuration
        error = libusbip_set_configuration(&ci, ctx, &dh, 1);
        if (error < 0) {
            printf("[*] get_serial: libusbip_set_configuration failed\n");
            goto close_fail;
        }
    }
    
    // Claim
    error = libusbip_claim_interface(&ci, ctx, &dh, 0x0);
    if (error < 0) {
        printf("[*] get_serial: libusbip_claim_interface failed\n");
        goto close_fail;
    } 
    
    // Set Interface
    error = libusbip_set_interface_alt_setting(&ci, ctx, &dh, 0x0, 0x0); // Dfu
    if (error < 0) {
        printf("[*] get_serial: libusbip_set_interface_alt_setting failed\n");
        goto release_fail;
    }    
    
    unsigned char *data = malloc(255 * sizeof(unsigned char));
    if (!data) {
        printf("[*] get_serial: malloc failed\n");
        goto release_fail;
        
    }
    
    error = libusbip_get_string_descriptor_ascii(&ci, ctx, &dh, info.serial, data, 255);
    if (error < 0) {
        printf("[*] get_serial: libusbip_get_string_descriptor_ascii failed\n");
        free(data);
        goto release_fail;
    }
    printf("[*] get_serial: found serial string (%s)\n", data);
    free(data);
    
    // Release
    error = libusbip_release_interface(&ci, ctx, &dh, 0x0);
    if (error < 0) {
        printf("[*] get_serial: libusbip_release_interface failed\n");
        goto close_fail;
    }
    
    libusbip_close(&ci, ctx, &dh);
    libusbip_exit(&ci, ctx);
    
    return 0;
release_fail:
    libusbip_release_interface(&ci, ctx, &dh, 0x0);
close_fail:
    libusbip_close(&ci, ctx, &dh);
exit_fail:
    libusbip_exit(&ci, ctx);
fail:
    return 1;
}

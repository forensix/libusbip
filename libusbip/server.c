/**
 * libusbip - server.c
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

#include "server.h"
#include "proto.h"
#include "warning.h"

#include <strings.h>

#include "libusbi.h"

/*
 * TODO:
 * Currently we're using one static handle.  This implies that we've
 * only one client.  If we need to handle more than one client, we
 * need to use libusbip_device_handle's session_data to identify the
 * corresponding client!
 *
 * There are 2 places where a client can get its handler: server_usb_open
 * and server_usb_open_device_with_vid_pid.
 */
static struct libusb_device_handle *server_hdl = NULL; 

libusbip_rpc_t
server_read_rpc(int sock) {
    libusbip_rpc_t rpc;

    proto_recv_rpc(&rpc, sock);
    
    return rpc;
}

void
server_usb_init(struct libusbip_connection_info *ci,
                struct libusb_context **ctx) {
    libusbip_error_t error = libusb_init(ctx);
    int sock = ci->client_sock;
    
    proto_send_int(&error, sock);
}

void
server_usb_exit(struct libusb_context *ctx) {
    libusb_exit(ctx);
}

void
server_usb_get_device_list(struct libusbip_connection_info *ci,
                           struct libusb_context *ctx) {
    struct libusb_device **list;
    struct libusbip_device_list ilist;
    int sock = ci->client_sock;
    size_t i;
    ssize_t cnt;
    
    bzero(&ilist, sizeof(struct libusbip_device_list));

    cnt =  libusb_get_device_list(ctx, &list);
    for (i = 0; i < cnt && i < LIBUSBIP_MAX_DEVS; i++) {
        struct libusb_device *dev = list[i];
        struct libusbip_device *idev = &ilist.devices[i];
        idev->bus_number = dev->bus_number;
        idev->device_address = dev->device_address;
        idev->num_configurations = dev->num_configurations;
        idev->session_data = dev->session_data;
        ilist.n_devices++;
    }
    
    proto_send_struct_dev_list(&ilist, sock);
    
    libusb_free_device_list(list, 1);
}

void
server_usb_get_device_descriptor(struct libusbip_connection_info *ci,
                                 struct libusb_context *ctx) {
    struct libusb_device **list = NULL;
    struct libusb_device *dev = NULL;
    struct libusbip_device idev;
    struct libusb_device_descriptor desc;
    struct libusbip_device_descriptor idesc;
    int sock = ci->client_sock;
    libusbip_error_t error;
    ssize_t cnt;
    size_t i;

    bzero(&desc, sizeof(struct libusb_device_descriptor));
    bzero(&idesc, sizeof(struct libusbip_device_descriptor));
    bzero(&idev, sizeof(struct libusbip_device));

    proto_recv_struct_dev(&idev, sock);
    
    cnt = libusb_get_device_list(ctx, &list);
    for (i = 0; i < cnt && i < LIBUSBIP_MAX_DEVS; i++) {
        struct libusb_device *tmp_dev = list[i];
        if (tmp_dev->session_data == idev.session_data) {
            dev = tmp_dev;
            break;
        }
    }
    
    if (!dev) {
        warning_no_device_found(__func__);
        error = LIBUSBIP_E_FAILURE;
        goto free;
    }

    error = libusb_get_device_descriptor(dev, &desc);
    if (error < 0) {
        error = LIBUSBIP_E_FAILURE;
        goto free;        
    }
    
    idesc.bLength = desc.bLength;
    idesc.bDescriptorType = desc.bDescriptorType;
    idesc.bcdUSB = desc.bcdUSB;
    idesc.bDeviceClass = desc.bDeviceClass;
    idesc.bDeviceSubClass = desc.bDeviceSubClass;
    idesc.bDeviceProtocol = desc.bDeviceProtocol;
    idesc.bMaxPacketSize0 = desc.bMaxPacketSize0;
    idesc.idVendor = desc.idVendor;
    idesc.idProduct = desc.idProduct;
    idesc.bcdDevice = desc.bcdDevice;
    idesc.iManufacturer = desc.iManufacturer;
    idesc.iProduct = desc.iProduct;
    idesc.iSerialNumber = desc.iSerialNumber;
    idesc.bNumConfigurations = desc.bNumConfigurations;
free:
    libusb_free_device_list(list, 1);
send:
    proto_send_struct_dev_desc(&idesc, sock);
    proto_send_int(&error, sock);
}

void
server_usb_open(struct libusbip_connection_info *ci,
                struct libusb_context *ctx) {
    struct libusb_device **list = NULL;
    struct libusb_device *dev = NULL;
    struct libusbip_device idev;
    struct libusbip_device_handle dh;
    int sock = ci->client_sock;
    libusbip_error_t error;
    ssize_t cnt;
    size_t i;
    
    bzero(&idev, sizeof(struct libusbip_device));
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev(&idev, sock);
    
    // Duplicate code :(((
    cnt = libusb_get_device_list(ctx, &list);
    for (i = 0; i < cnt && i < LIBUSBIP_MAX_DEVS; i++) {
        struct libusb_device *tmp_dev = list[i];
        if (tmp_dev->session_data == idev.session_data) {
            dev = tmp_dev;
            break;
        }
    }
    
    if (!dev) {
        warning_no_device_found(__func__);
        error = LIBUSBIP_E_FAILURE;
        goto free;
    }

    error = libusb_open(dev, &server_hdl);
    if (error < 0) {
        error = LIBUSBIP_E_FAILURE;
        goto free;
    }

    /* Store handler so we can access and identify it later. */
    dh.session_data = server_hdl->dev->session_data;
    
free:
    libusb_free_device_list(list, 1);
send:
    /*
     * NOTE:
     * Here we send the handle back to the client.  Again, to support
     * more handles we need to convert the handle back to get
     * the correct one.
     */
    proto_send_struct_dev_hndl(&dh, sock);
    proto_send_int(&error, sock);
}

void
server_usb_open_device_with_vid_pid(struct libusbip_connection_info *ci,
                                    struct libusb_context *ctx) {
    struct libusbip_device_handle dh;
    int sock = ci->client_sock;
    uint16_t vid, pid;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    proto_recv_uint16(&vid, sock);
    proto_recv_uint16(&pid, sock);
    
    if (server_hdl) {
        libusb_close(server_hdl);
        server_hdl = NULL;
    }
    
    server_hdl = libusb_open_device_with_vid_pid(ctx, vid, pid);
    if (!server_hdl) {
        goto send;
    }
    
    /* Store handler so we can access and identify it later. */
    dh.session_data = server_hdl->dev->session_data;
send:
    proto_send_struct_dev_hndl(&dh, sock);
}

void
server_usb_close(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    int sock = ci->client_sock;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    
    if (server_hdl) {
        libusb_close(server_hdl);
        server_hdl = NULL;
    }
}

void
server_usb_claim_interface(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    libusbip_error_t error;
    int sock = ci->client_sock, intf;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    proto_recv_int(&intf, sock);
    
    error = libusb_claim_interface(server_hdl, intf);
    if (error < 0)
        error = LIBUSBIP_E_FAILURE;
    
    proto_send_int(&error, sock);
}

void
server_usb_release_interface(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    libusbip_error_t error;
    int sock = ci->client_sock, intf;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    proto_recv_int(&intf, sock);
    
    error = libusb_release_interface(server_hdl, intf);
    if (error < 0)
        error = LIBUSBIP_E_FAILURE;
    
    proto_send_int(&error, sock);
}

void
server_usb_get_configuration(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    libusbip_error_t error;
    int sock = ci->client_sock, conf;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));

    proto_recv_struct_dev_hndl(&dh, sock);
    proto_recv_int(&conf, sock);
    
    error = libusb_get_configuration(server_hdl, &conf);
    if (error < 0)
        error = LIBUSBIP_E_FAILURE;
    
    proto_send_int(&error, sock);    
}

void
server_usb_set_configuration(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    libusbip_error_t error;
    int sock = ci->client_sock, conf;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    proto_recv_int(&conf, sock);
    
    error = libusb_set_configuration(server_hdl, conf);
    if (error < 0)
        error = LIBUSBIP_E_FAILURE;
    
    proto_send_int(&error, sock);    
}

void
server_usb_set_interface_alt_setting(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    libusbip_error_t error;
    int sock = ci->client_sock, intf, alt_setting;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    proto_recv_int(&intf, sock);
    proto_recv_int(&alt_setting, sock);
    
    error = libusb_set_interface_alt_setting(server_hdl, intf, alt_setting);
    if (error < 0)
        error = LIBUSBIP_E_FAILURE;
    
    proto_send_int(&error, sock);
}

void
server_usb_reset_device(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    libusbip_error_t error;
    int sock = ci->client_sock;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    
    error = libusb_reset_device(server_hdl);
    if (error < 0)
        error = LIBUSBIP_E_FAILURE;
    
    proto_send_int(&error, sock);    
}

void
server_usb_clear_halt(struct libusbip_connection_info *ci) {
    struct libusbip_device_handle dh;
    libusbip_error_t error;
    int sock = ci->client_sock;
    uint16_t endpoint;
    
    bzero(&dh, sizeof(struct libusbip_device_handle));
    
    proto_recv_struct_dev_hndl(&dh, sock);
    proto_recv_uint16(&endpoint, sock);
    
    error = libusb_clear_halt(server_hdl, endpoint);
    if (error < 0)
        error = LIBUSBIP_E_FAILURE;
    
    proto_send_int(&error, sock);
}


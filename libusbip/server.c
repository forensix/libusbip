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

libusbip_rpc_t
server_read_rpc(int sock) {
    libusbip_rpc_t rpc;

    proto_recv_rpc(&rpc, sock);
    
    return rpc;
}

void
server_usb_init(struct libusbip_connection_info *ci, struct libusb_context **ctx) {
    libusbip_error_t error = libusb_init(ctx);
    
    proto_send_int(&error, ci->client_sock);
}

void
server_usb_exit(struct libusb_context *ctx) {
    libusb_exit(ctx);
}

void
server_usb_get_device_list(struct libusbip_connection_info *ci, struct libusb_context *ctx) {
    struct libusb_device **list;
    struct libusbip_device_list ilist;
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
    
    proto_send_struct_dev_list(&ilist, ci->client_sock);
    
    libusb_free_device_list(list, 1);
}

void server_usb_get_device_descriptor(struct libusbip_connection_info *ci, struct libusb_context *ctx) {
    struct libusb_device **list = NULL;
    struct libusb_device *dev = NULL;
    struct libusbip_device idev;
    struct libusb_device_descriptor desc;
    struct libusbip_device_descriptor idesc;
    libusbip_error_t error;
    
    bzero(&desc, sizeof(struct libusb_device_descriptor));
    bzero(&idesc, sizeof(struct libusbip_device_descriptor));
    bzero(&idev, sizeof(struct libusbip_device));

    proto_recv_struct_dev(&idev, ci->client_sock);
    
    size_t i;
    ssize_t cnt = libusb_get_device_list(ctx, &list);
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
    proto_send_struct_dev_desc(&idesc, ci->client_sock);
    proto_send_int(&error, ci->client_sock);
}

/**
 * libusbip - client.c
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

#include "client.h"
#include "proto.h"

#include "libusbi.h"
#include <string.h>

libusbip_error_t
client_usb_init(struct libusbip_connection_info *ci) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_INIT;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_recv_int(&error, sock);
        
    return error;
}

void
client_usb_exit(struct libusbip_connection_info *ci) {
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_EXIT;
    int sock = ci->server_sock;

    proto_send_rpc(&rpc, sock);
}

void
client_usb_get_device_list(struct libusbip_connection_info *ci,
                           struct libusbip_device_list *dl) {
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_GET_DEVICE_LIST;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_recv_struct_dev_list(dl, sock);
}

libusbip_error_t
client_usb_get_device_descriptor(struct libusbip_connection_info *ci,
                                 struct libusbip_device *dev,
                                 struct libusbip_device_descriptor *dd) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_GET_DEVICE_DESCRIPTOR;
    int sock = ci->server_sock;

    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev(dev, sock);
    proto_recv_struct_dev_desc(dd, sock);
    proto_recv_int(&error, sock);
    
    return error;
}

libusbip_error_t
client_usb_open(struct libusbip_connection_info *ci,
                struct libusbip_device *dev,
                struct libusbip_device_handle *dh) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_OPEN;
    int sock = ci->server_sock;

    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev(dev, sock);
    proto_recv_struct_dev_hndl(dh, sock);
    proto_recv_int(&error, sock);
    
    return error;
}

void
client_usb_open_device_with_vid_pid(struct libusbip_connection_info *ci,
                                    struct libusbip_device_handle *dh,
                                    uint16_t vid,
                                    uint16_t pid) {
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_OPEN_DEVICE_WITH_VID_PID;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_uint16(&vid, sock);
    proto_send_uint16(&pid, sock);
    proto_recv_struct_dev_hndl(dh, sock);
}

void
client_usb_close(struct libusbip_connection_info *ci,
                 struct libusbip_device_handle *dh) {
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_CLOSE;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
}

libusbip_error_t
client_usb_claim_interface(struct libusbip_connection_info *ci,
                           struct libusbip_device_handle *dh, int intf) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_CLAIM_INTERFACE;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_int(&intf, sock);
    proto_recv_int(&error, sock);
    
    return error;
}

libusbip_error_t
client_usb_release_interface(struct libusbip_connection_info *ci,
                             struct libusbip_device_handle *dh, int intf) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_RELEASE_INTERFACE;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_int(&intf, sock);
    proto_recv_int(&error, sock);
    
    return error;
}

libusbip_error_t
client_usb_get_configuration(struct libusbip_connection_info *ci,
                             struct libusbip_device_handle *dh, int *conf) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_GET_CONFIGURATION;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_int(conf, sock);
    proto_recv_int(conf, sock);
    proto_recv_int(&error, sock);

    return error;
}

libusbip_error_t
client_usb_set_configuration(struct libusbip_connection_info *ci,
                             struct libusbip_device_handle *dh, int conf) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_SET_CONFIGURATION;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_int(&conf, sock);
    proto_recv_int(&error, sock);
    
    return error;
}

libusbip_error_t
client_usb_set_interface_alt_setting(struct libusbip_connection_info *ci,
                                     struct libusbip_device_handle *dh,
                                     int intf, int alt_setting) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_SET_INTERFACE_ALT_SETTING;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_int(&intf, sock);
    proto_send_int(&alt_setting, sock);
    proto_recv_int(&error, sock);
    
    return error;
}

libusbip_error_t
client_usb_reset_device(struct libusbip_connection_info *ci,
                        struct libusbip_device_handle *dh) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_RESET_DEVICE;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_recv_int(&error, sock);
    
    return error;
}

libusbip_error_t
client_usb_clear_halt(struct libusbip_connection_info *ci,
                      struct libusbip_device_handle *dh, uint16_t endpoint) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_CLEAR_HALT;
    int sock = ci->server_sock;
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_uint16(&endpoint, sock);
    proto_recv_int(&error, sock);
    
    return error;    
}

libusbip_error_t
client_usb_get_string_descriptor_ascii(struct libusbip_connection_info *ci,
                                       struct libusbip_device_handle *dh,
                                       uint16_t idx, unsigned char *data, 
                                       int length) {
    libusbip_error_t error;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_GET_STRING_DESCRIPTOR_ASCII;
    int sock = ci->server_sock;
    uint16_t buf[LIBUSBIP_MAX_DATA];

    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_uint16(&idx, sock);
    proto_send_int(&length, sock);
    proto_recv_uint16_arr(buf, sock);
    proto_recv_int(&error, sock);
    memcpy(data, buf, length);
    
    return error;
}

int
client_usb_control_transfer(struct libusbip_connection_info *ci,
                            struct libusbip_device_handle *dh,
                            uint16_t req_type, uint16_t req, uint16_t val,
                            uint16_t idx, unsigned char *data, uint16_t len,
                            uint32_t timeout) {
    int bytes;
    libusbip_rpc_t rpc = LIBUSBIP_RPC_USB_CONTROL_TRANSFER;
    int sock = ci->server_sock;
    uint16_t buf[LIBUSBIP_MAX_DATA];
    
    memcpy(buf, data, len);
    
    proto_send_rpc(&rpc, sock);
    proto_send_struct_dev_hndl(dh, sock);
    proto_send_uint16(&req_type, sock);
    proto_send_uint16(&req, sock);
    proto_send_uint16(&val, sock);
    proto_send_uint16(&idx, sock);
    proto_send_uint16_arr(buf, sock);
    proto_send_uint16(&len, sock);
    proto_send_uint32(&timeout, sock);
    proto_recv_uint16_arr(buf, sock);
    proto_recv_int(&bytes, sock);
    
    if ((req_type & LIBUSB_ENDPOINT_DIR_MASK)
        == LIBUSB_ENDPOINT_IN) {
        memcpy(data, buf, len);
    }
    
    return bytes;
}

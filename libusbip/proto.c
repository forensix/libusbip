/**
 * libusbip - proto.c
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

#include "proto.h"
#include "tpl.h"

void proto_send(tpl_node *tn, int sock) {
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_FD, sock);
    tpl_free(tn);
}

void proto_recv(tpl_node *tn, int sock) {
    tpl_load(tn, TPL_FD, sock);
    tpl_unpack(tn, 0);
    tpl_free(tn);
}

void proto_send_int(int *val, int sock) {
    tpl_node *tn = tpl_map(PROTO_INT_FMT, val);
    
    proto_send(tn, sock);
}

void proto_recv_int(int *val, int sock) {
    tpl_node *tn = tpl_map(PROTO_INT_FMT, val);
    
    proto_recv(tn, sock);
}

void proto_send_int_uint16_arr(int *val, uint16_t arr[], int sock) {
    tpl_node *tn
    = tpl_map(PROTO_INT_UINT16_ARR_FMT, val, arr, PROTO_MAX_DATA);
    
    proto_send(tn, sock);
}

void proto_recv_int_uint16_arr(int *val, uint16_t arr[], int sock) {
    tpl_node *tn
    = tpl_map(PROTO_INT_UINT16_ARR_FMT, val, arr, PROTO_MAX_DATA);
    
    proto_recv(tn, sock);
}

void proto_send_int_int_uint16_arr(int *val1, int *val2, uint16_t arr[], int sock) {
    tpl_node *tn
    = tpl_map(PROTO_INT_INT_UINT16_ARR_FMT, val1, val2, arr, PROTO_MAX_DATA);
    
    proto_send(tn, sock);
}

void proto_recv_int_int_uint16_arr(int *val1, int *val2, uint16_t arr[], int sock) {
    tpl_node *tn
    = tpl_map(PROTO_INT_INT_UINT16_ARR_FMT, val1, val2, arr, PROTO_MAX_DATA);
    
    proto_recv(tn, sock);
}

void proto_send_struct_dev(struct libusbip_device *dev, int sock) {
    tpl_node *tn
    = tpl_map(PROTO_STRUCT_DEV_FMT, dev);
    
    proto_send(tn, sock);
}

void proto_recv_struct_dev(struct libusbip_device *dev, int sock) {
    tpl_node *tn
    = tpl_map(PROTO_STRUCT_DEV_FMT, dev);
    
    proto_recv(tn, sock);
}

void proto_send_struct_devlist(struct libusbip_device_list *devlist, int sock) {
    tpl_node *tn
    = tpl_map(PROTO_STRUCT_DEVLIST_FMT, &devlist->n_devices, devlist->devices);
    
    proto_send(tn, sock);
}

void proto_recv_struct_devlist(struct libusbip_device_list *devlist, int sock) {
    tpl_node *tn
    = tpl_map(PROTO_STRUCT_DEVLIST_FMT, &devlist->n_devices, devlist->devices);
    
    proto_recv(tn, sock);
}

void proto_send_rpc(libusbip_rpc_t *rpc, int sock) {
    proto_send_int((int *)rpc, sock);
}

void proto_recv_rpc(libusbip_rpc_t *rpc, int sock) {
    proto_recv_int((int *)rpc, sock);
}


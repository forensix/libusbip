/**
 * libusbip - proto.h (Private libusbip header file)
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

#include "libusbip.h"

void proto_send_int(int *val, int sock);
void proto_recv_int(int *val, int sock);

void proto_send_uint16(uint16_t *val, int sock);
void proto_recv_uint16(uint16_t *val, int sock);

void proto_send_uint16_uint16_int(uint16_t *val1, uint16_t *val2, int *val3, int sock);
void proto_recv_uint16_uint16_int(uint16_t *val1, uint16_t *val2, int *val3, int sock);

void proto_send_uint16_arr(uint16_t arr[], int sock);
void proto_recv_uint16_arr(uint16_t arr[], int sock);

void proto_send_int_uint16_arr(int *val, uint16_t arr[], int sock);
void proto_recv_int_uint16_arr(int *val, uint16_t arr[], int sock);

void proto_send_int_int_uint16_arr(int *val1, int *val2, uint16_t arr[], int sock);
void proto_recv_int_int_uint16_arr(int *val1, int *val2, uint16_t arr[], int sock);

void proto_send_struct_dev(struct libusbip_device *dev, int sock);
void proto_recv_struct_dev(struct libusbip_device *dev, int sock);

void proto_send_struct_dev_list(struct libusbip_device_list *dl, int sock);
void proto_recv_struct_dev_list(struct libusbip_device_list *dl, int sock);

void proto_send_struct_dev_desc(struct libusbip_device_descriptor *dd, int sock);
void proto_recv_struct_dev_desc(struct libusbip_device_descriptor *dd, int sock);

void proto_send_struct_dev_hndl(struct libusbip_device_handle *dh, int sock);
void proto_recv_struct_dev_hndl(struct libusbip_device_handle *dh, int sock);

void proto_send_rpc(libusbip_rpc_t *rpc, int sock);
void proto_recv_rpc(libusbip_rpc_t *rpc, int sock);
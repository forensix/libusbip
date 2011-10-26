/**
 * libusbip - client.h (Private libusbip header file)
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

libusbip_error_t client_usb_init(struct libusbip_connection_info *ci);
libusbip_error_t client_usb_open(struct libusbip_connection_info *ci, struct libusbip_device *dev, struct libusbip_device_handle *dh);
libusbip_error_t client_usb_get_device_descriptor(struct libusbip_connection_info *ci, struct libusbip_device *dev, struct libusbip_device_descriptor *dd);
libusbip_error_t client_usb_claim_interface(struct libusbip_connection_info *ci, struct libusbip_device_handle *dh, int intf); 
libusbip_error_t client_usb_release_interface(struct libusbip_connection_info *ci, struct libusbip_device_handle *dh, int intf);

void client_usb_exit(struct libusbip_connection_info *ci);
void client_usb_close(struct libusbip_connection_info *ci, struct libusbip_device_handle *dh);
void client_usb_get_device_list(struct libusbip_connection_info *ci, struct libusbip_device_list *dl);
void client_usb_open_device_with_vid_pid(struct libusbip_connection_info *ci, struct libusbip_device_handle *dh, int vid, int pid);
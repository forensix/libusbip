Libusbip - C library to support USB over IP
===============================================

Introducing
-----------

libusbip was originally designed to communicate with remote iDevices via USB over IP. It provides<br>
a layer on top of libusb-1.0. libusbip allows you to work with remote USB devices over a local<br>
network or Internet as if they were connected directly to your local Mac/PC.<br>

It is realized as a library so you can write your own individual client-server programs.<br>

How to build
------------

To build libusbip you first need to install libusb-1.0 (http://www.libusb.org/wiki/libusb-1.0) and<br>
tpl (http://tpl.sourceforge.net/).<br>

After that just run <i>make</i> to compile libusbip or <i>make all</i> to compile libusbip and<br>
all example programs.

Supported functions
-------------------

Currently, libusbip provides support for the following functions:

- <b>libusb_init</b>
- <b>libusb_exit</b>
- <b>libusb_get_device_list</b>
- <b>libusb_get_device_descriptor</b>
- <b>libusb_open_device_with_vid_pid</b>
- <b>libusb_close</b>
- <b>libusb_open</b>
- <b>libusb_claim_interface</b>
- <b>libusb_release_interface</b>
- <b>libusb_get_configuration</b>
- <b>libusb_set_configuration</b>
- <b>libusb_set_interface_alt_setting</b>
- <b>libusb_reset_device</b>
- <b>libusb_control_transfer</b>
- <b>libusb_bulk_transfer</b>
- <b>libusb_clear_halt</b>
- <b>libusb_get_string_descriptor_ascii</b>

For a general overview, please refer to the libusb-1.0 API Reference http://libusb.sourceforge.net/api-1.0/

Data structures
---------------

libusbip consists of the following structures:

- <b>struct libusbip_device</b><br>
  Reflection of struct libusb_device

- <b>struct libusbip_device_list</b><br>
  Reflection of libusb_device **

- <b>struct libusbip_device_descriptor</b><br>
  Reflection of libusb_device_descriptor 

- <b>struct libusbip_device_handle</b><br>
  Reflection of libusb_device_handle

- <b>struct libusbip_connection_info</b><br>
  Holds information about the client or server, respectively. And<br>
  if libusbip is used in client or server context.

- <b>struct libusbip_rpc_info</b><br>
  Holds all information about the structures mentioned above. You<br>
  require that structure if you use libusbip through the<br>
  libusbip_rpc_call convenience function.

Typedefs
--------

- <b>libusbip_error_t</b><br>
  libusbip error identifier. Can be <i>LIBUSBIP_E_SUCCESS</i> or <i>LIBUSBIP_E_FAILURE</i>.

- <b>libusbip_rpc_t</b><br>
  Identifies the Remote Procedure Call. For example, <i>LIBUSBIP_RPC_USB_INIT</i><br>
  stands for libusb_init

- <b>libusbip_ctx_t</b><br>
  ibusbip context identifier. Can be <i>LIBUSBIP_CTX_CLIENT</i> or <i>LIBUSBIP_CTX_SERVER</i>.






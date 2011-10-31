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

Supported libusb functions
--------------------------

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

Functions
---------

- <b>libusbip_error_t libusbip_init(struct libusbip_connection_info *ci)</b>
  <br>
  <br>
  Initialize libusbip.
  <br>
  <br>
  This function must be called before calling any other libusbip function.
  <br>
  <br>
  <b>Parameters:</b>
  <br>
  <i>ci</i> connection info pointer.
  <br>
  <br>
  <b>Returns:</b>
  <br>
  <i>LIBUSBIP_E_SUCCESS</i> on success, or <i>LIBUSBIP_E_FAILURE</i> on failure.
  <br>
  <br>
- <b>void libusbip_exit(struct libusbip_connection_info *ci)</b>
  <br>
  <br>
  Deinitialize libusbip.
  <br>
  <br>
  Should be called after closing all open devices and before your application terminates.
  <br>
  <br>
  <b>Parameters:</b>
  <br>
  <i>ci</i> connection info pointer.
  <br>
  <br>
- <b>void libusbip_get_device_list(struct libusbip_connection_info *ci, struct libusbip_device_list *dl)</b>
  <br>
  <br>
  Returns a list of USB devices currently attached to the system.
  <br>
  <br>
  This is your entry point into finding a USB device to operate.
  <br>
  <br>
  <b>Parameters:</b>
  <br>
  <i>ci</i> connection info pointer.
  <br>
  <i>dl</i> pointer as output location for a list of devices.
  <br>
  <br>
- <b>libusbip_get_device_descriptor(struct libusbip_connection_info *ci, struct libusbip_device *dev,<br>
     struct libusbip_device_descriptor *dd)</b>
  <br>
  <br>
  Get the USB configuration descriptor for the currently active configuration.
  <br>
  <br>
  <b>Parameters:</b>
  <br>
  <i>ci</i> connection info pointer.
  <br>
  <i>dev</i> The device.
  <br>
  <i>dd</i> pointer as output location for the descriptor data.
  <br>
  <br>
  <b>Returns:</b>
  <br>
  <i>LIBUSBIP_E_SUCCESS</i> on success, or <i>LIBUSBIP_E_FAILURE</i> on failure.
  <br>
  <br>
- <b>libusbip_error_t libusbip_open(struct libusbip_connection_info *ci, struct libusbip_device *dev,<br>
     struct libusbip_device_handle *dh)</b>
  <br>
  <br>
  Open a device and obtain a device handle.
  <br>
  <br>
  <b>Parameters:</b>
  <br>
  <i>ci</i> connection info pointer.
  <br>
  <i>dev</i> The device.
  <br>
  <i>dh</i> pointer as output location for the returned device handle pointer.
  <br>
  <br>
  <b>Returns:</b>
  <br>
  <i>LIBUSBIP_E_SUCCESS</i> on success, or <i>LIBUSBIP_E_FAILURE</i> on failure.
  <br>
  <br>
- <b>libusbip_error_t libusbip_claim_interface(struct libusbip_connection_info *ci,<br>
     struct libusbip_device_handle *dh, int intf)</b>
  <br>
  <br>
  Claim an interface on a given device handle.
  <br>
  <br>
  <b>Parameters:</b>
  <br>
  <i>ci</i> connection info pointer.
  <br>
  <i>dh</i> The device handle.
  <br>
  <i>intf</i> the number of the interface you wish to claim
  <br>
  <br>
  <b>Returns:</b>
  <br>
  <i>LIBUSBIP_E_SUCCESS</i> on success, or <i>LIBUSBIP_E_FAILURE</i> on failure.
  <br>
  <br>















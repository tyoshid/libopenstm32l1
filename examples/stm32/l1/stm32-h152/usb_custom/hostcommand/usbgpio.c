#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

#define VID 0x2975
#define PID 0x0001

int main(int argc, char *argv[])
{
	unsigned short data;
	int r;
	libusb_context *ctx = NULL;
	int i;
	libusb_device_handle *dev_handle;

	if (argc > 1) {
		if (sscanf(argv[1], "%hx", &data) != 1) {
			fprintf(stderr, "Invalid argument: %s\n", argv[1]);
			return 1;
		}
	}

	r = libusb_init(&ctx);
	if (r < 0) {
		fprintf(stderr, "libusb_init() failed: %s\n",
			libusb_error_name(r));
		return 1;
	}
	//libusb_set_debug(ctx, 3);

	for (i = 0; i < 3; i++) {
		dev_handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
		if (dev_handle)
			break;
		usleep(100000);
	}
	if (!dev_handle) {
		fprintf(stderr, "libusb_open_device_with_vid_pid() failed.\n");
		libusb_exit(ctx);
		return 1;
	}

	if (argc < 2) {
		r = libusb_control_transfer(dev_handle,
					    LIBUSB_REQUEST_TYPE_VENDOR |
					    LIBUSB_RECIPIENT_DEVICE |
					    LIBUSB_ENDPOINT_IN, 0, 0,
					    0, (unsigned char *)&data,
					    sizeof(data), 1000);
		if (r < 0) {
			fprintf(stderr, "libusb_control_transfer() failed: "
				"%s\n", libusb_error_name(r));
			libusb_close(dev_handle);
			libusb_exit(ctx);
			return 1;
		}
		printf("Port: 0x%04x\n", data);
	} else {
		r = libusb_control_transfer(dev_handle,
					    LIBUSB_REQUEST_TYPE_VENDOR |
					    LIBUSB_RECIPIENT_DEVICE |
					    LIBUSB_ENDPOINT_OUT, 1, data,
					    0, NULL, 0, 1000);
		if (r < 0) {
			fprintf(stderr, "libusb_control_transfer() failed: "
				"%s\n", libusb_error_name(r));
			libusb_close(dev_handle);
			libusb_exit(ctx);
			return 1;
		}
		printf("Port <- 0x%04x\n", data);
	}

	libusb_close(dev_handle);
	libusb_exit(ctx);
	return 0;
}

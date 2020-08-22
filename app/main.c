#include <errno.h>
#include <getopt.h>
#include <iio.h>
#include <inttypes.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define IIO_ENSURE(expr) \
{\
	if (!(expr)) {\
		(void)fprintf(stderr, "assertion failed (%s:%d)\n", __FILE__, __LINE__);\
		(void)abort();\
	}\
}
static char *name = "microchip,pac1934";
static char *trigger_str = "instance1";
static int buffer_length = 1;
static int count = -1;

// libiio supports multiple methods for reading data from a buffer
enum {
	BUFFER_POINTER,
	SAMPLE_CALLBACK,
	CHANNEL_READ_RAW,
	CHANNEL_READ,
	MAX_READ_METHOD,
};
static int buffer_read_method = BUFFER_POINTER;

// Streaming devices
static struct iio_device *dev;

/* IIO structs required for streaming */
static struct iio_context *ctx;
static struct iio_buffer *rxbuf;
static struct iio_channel **channels;
static unsigned int channel_count;

static bool stop;
static bool has_repeat;

/* cleanup and exit */
static void shutdown() {
	int ret;

	if (channels) {
		free(channels);
	}

	printf("* Destroying buffers\n");
	if (rxbuf) {
		iio_buffer_destroy(rxbuf);
	}

	printf("* Disassociate trigger\n");
	if (dev) {
		ret = iio_device_set_trigger(dev, NULL);
		if (ret < 0) {
			char buf[256];
			iio_strerror(-ret, buf, sizeof(buf));
			fprintf(stderr, "%s (%d) while Disassociate trigger\n", buf, ret);
		}
	}

	printf("* Destroying context\n");
	if (ctx) {
		iio_context_destroy(ctx);
	}
	exit(0);
}

static void handle_sig(int sig) {
	printf("Waiting for process to finish... got signal : %d\n", sig);
	stop = true;
}

static ssize_t sample_cb(const struct iio_channel *chn, void *src, size_t bytes, __notused void *d) {
	const struct iio_data_format *fmt = iio_channel_get_data_format(chn);
	unsigned int j, repeat = has_repeat ? fmt->repeat : 1;

	printf("%s ", iio_channel_get_id(chn));
	for (j = 0; j < repeat; ++j) {
		if (bytes == sizeof(int16_t))
			printf("%" PRIi16 " ", ((int16_t *)src)[j]);
		else if (bytes == sizeof(int64_t))
			printf("%" PRId64 " ", ((int64_t *)src)[j]);
	}

	return bytes * repeat;
}

static void usage(__notused int argc, char *argv[]) {
	printf("Usage: %s [OPTION]\n", argv[0]);
	printf("  -d\tdevice name (default \"iio_dummy_part_no\")\n");
	printf("  -t\ttrigger name (default \"instance1\")\n");
	printf("  -b\tbuffer length (default 1)\n");
	printf("  -r\tread method (default 0 pointer, 1 callback, 2 read raw, 3 read)\n");
	printf("  -c\tread count (default no limit)\n");
}

static void parse_options(int argc, char *argv[]) {
	int c;

	while ((c = getopt(argc, argv, "d:t:b:r:c:h")) != -1) {
		switch (c) {
			case 'd':
				name = optarg;
				break;
			case 't':
				trigger_str = optarg;
				break;
			case 'b':
				buffer_length = atoi(optarg);
				break;
			case 'r':
				if (atoi(optarg) >= 0 && atoi(optarg) < MAX_READ_METHOD) {
					buffer_read_method = atoi(optarg);
				} else {
					usage(argc, argv);
					exit(1);
				}
				break;
			case 'c':
				if (atoi(optarg) > 0) {
					count = atoi(optarg);
				} else {
					usage(argc, argv);
					exit(1);
				}
				break;
			case 'h':
			default:
				usage(argc, argv);
				exit(1);
		}
	}
}

/* simple configuration and streaming */
int main(int argc, char **argv) {
	// Hardware trigger
	struct iio_device *trigger;
	unsigned int devices_count = 0;
	parse_options(argc, argv);

	// Listen to ctrl+c and assert
	signal(SIGINT, handle_sig);

	unsigned int i, j, major, minor;
	char git_tag[8];
	iio_library_get_version(&major, &minor, git_tag);
	printf("Library version: %u.%u (git tag: %s)\n", major, minor, git_tag);

	/* check for struct iio_data_format.repeat support
	 * 0.8 has repeat support, so anything greater than that */
	has_repeat = ((major * 10000) + minor) >= 8 ? true : false;
	channel_count = 0;
	printf("* Acquiring IIO context\n");
	ctx = iio_create_default_context();
	IIO_ENSURE((ctx) && "No context");
	devices_count=iio_context_get_devices_count(ctx);
	printf("Device count: %d\n", devices_count);
	IIO_ENSURE(devices_count > 0 && "No devices");

	printf("* Acquiring device %s\n", name);
	dev = iio_context_find_device(ctx, name);
	if (!dev) {
		perror("No device found");
		shutdown();
	}
	unsigned int device_channels_count = iio_device_get_channels_count(dev);
	printf("* Initializing IIO streaming channels: %d\n", device_channels_count);
	for (i = 0; i < device_channels_count; ++i) {
		struct iio_channel *chn = iio_device_get_channel(dev, i);
		if (iio_channel_is_scan_element(chn)) {
			printf("%s\n", iio_channel_get_id(chn));
			channel_count++;
		}
	}
	if (channel_count == 0) {
		printf("No scan elements found (make sure the driver built with 'CONFIG_IIO_SIMPLE_DUMMY_BUFFER=y')\n");
		shutdown();
	}
	channels = calloc(channel_count, sizeof *channels);
	if (!channels) {
		perror("Channel array allocation failed");
		shutdown();
	}
	for (i = 0; i < channel_count; ++i) {
		struct iio_channel *chn = iio_device_get_channel(dev, i);
		if (iio_channel_is_scan_element(chn))
			channels[i] = chn;
	}
}

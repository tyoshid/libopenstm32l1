#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <linux/videodev2.h>
#include <jack/jack.h>

#define DEV_RADIO	"/dev/radio0"
#define JACK_SERVER	"/usr/bin/jackd"
#define INPUT_DEV	"hw:Radio"
#define OUTPUT_DEV	"hw:Speaker"
#define IN_PORT1	"system:capture_1"
#define IN_PORT2	"system:capture_2"
#define OUT_PORT1	"system:playback_1"
#define OUT_PORT2	"system:playback_2"

int do_exit;

void termination_handler(int signum)
{
	do_exit = -1;
}

void usage(char *progname)
{
	fprintf(stderr, "Usage: %s [-i radio][-o playback][freq(MHz)]\n",
		progname);
	exit(1);
}

int main(int argc, char *argv[])
{
	int ch;
	extern char *optarg;
	extern int optind;
	struct sigaction new_action;
	int fd = -1;
	struct v4l2_capability caps;
	struct v4l2_tuner tuner;
	struct v4l2_frequency freq;
	double f = 0.;
	struct v4l2_control control;
	pid_t pid = -1;
	char *jackd_argv[] = {
		JACK_SERVER,
		"-R",
		"-n",
		"radio",
		"-d",
		"alsa",
		"-C",
		INPUT_DEV,
		"-P",
		OUTPUT_DEV,
		"-S",
		NULL
	};
	jack_client_t *client;
	jack_status_t status;
	int s;

	while ((ch = getopt(argc, argv, "i:o:")) != -1) {
		switch (ch) {
		case 'i':
			jackd_argv[7] = optarg;
			break;
		case 'o':
			jackd_argv[9] = optarg;
			break;
		default:
			usage(argv[0]);
		}
	}

	if (argc - optind >= 1) {
		if (sscanf(argv[optind], "%lf", &f) != 1)
			usage(argv[0]);
		f *= 1000000.;
	}

	/* Change signal action (Ctrl+C). */
	new_action.sa_handler = termination_handler;
	if (sigemptyset(&new_action.sa_mask) == -1) {
		fprintf(stderr, "sigemptyset() failed\n");
		goto radio_error;
	}
	new_action.sa_flags = 0;
	if (sigaction(SIGINT, &new_action, NULL) == 1) {
		fprintf(stderr, "sigaction() failed\n");
		goto radio_error;
	}

	/* Open radio control device. */
	if ((fd = open(DEV_RADIO, O_RDONLY)) == -1) {
		perror("open() failed");
		goto radio_error;
	}

	/* Check device. */
	memset(&caps, 0, sizeof(caps));
	if (ioctl(fd, VIDIOC_QUERYCAP, &caps) == -1) {
		perror("ioctl() (VIDIOC_QUEYCAP) failed");
		goto radio_error;
	}
	if (!(caps.capabilities & V4L2_CAP_RADIO) ||
	    !(caps.capabilities & V4L2_CAP_TUNER)) {
		fprintf(stderr, "Not a radio tuner (0x%x)\n",
			caps.capabilities);
		goto radio_error;
	}

	memset(&tuner, 0, sizeof(tuner));
	if (ioctl(fd, VIDIOC_G_TUNER, &tuner) == -1) {
		perror("ioctl() (VIDIOC_G_TUNER) failed");
		goto radio_error;
	}
	if (tuner.type != V4L2_TUNER_RADIO) {
		fprintf(stderr, "Not a radio tuner (%d)\n", tuner.type);
		goto radio_error;
	}

	/* Set Frequency. */
	if (f != 0.) {
		memset(&freq, 0, sizeof(freq));
		freq.type = V4L2_TUNER_RADIO;
		if (tuner.capability & V4L2_TUNER_CAP_LOW)
			f /= 62.5;
		else
			f /= 62500.;
		freq.frequency = f;
		if (ioctl(fd, VIDIOC_S_FREQUENCY, &freq) == -1) {
			perror("ioctl() (VIDIOC_S_FREQUENCY) failed");
			goto radio_error;
		}
	}

	/* Get frequency. */
	memset(&freq, 0, sizeof(freq));
	freq.type = V4L2_TUNER_RADIO;
	if (ioctl(fd, VIDIOC_G_FREQUENCY, &freq) == -1) {
		perror("ioctl() (VIDIOC_G_FREQUENCY) failed");
		goto radio_error;
	}
	f = freq.frequency;
	if (tuner.capability & V4L2_TUNER_CAP_LOW)
		f *= 62.5;
	else
		f *= 62500.;

	/* Disable mute. */
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_AUDIO_MUTE;
	if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) {
		perror("ioctl() (VIDIOC_S_CTRL) failed");
		goto radio_error;
	}

	/* Start Jack server. */
	if ((pid = fork()) == -1) {
		perror("fork() failed");
		goto radio_error;
	}

	if (pid == 0) {
		if (execv(JACK_SERVER, jackd_argv) == -1) {
			perror("execv() failed");
			_exit(2);
		}

	}

	/* Start Jack client. */
	sleep(1);
	if ((client = jack_client_open("connect",
				       JackNoStartServer | JackServerName,
				       &status, "radio")) == 0) {
		fprintf(stderr, "jack server not running?\n");
		goto radio_error;
	}

	if (jack_activate(client)) {
		fprintf(stderr, "cannot activate client\n");
		goto radio_error;
	}

	/* Connect ports. */
	if (jack_connect(client, IN_PORT1, OUT_PORT1)) {
		fprintf(stderr, "cannot connect ports\n");
		goto radio_error;
	}

	if (jack_connect(client, IN_PORT2, OUT_PORT2)) {
		fprintf(stderr, "cannot connect ports\n");
		goto radio_error;
	}

	printf("\n");
	while (!do_exit) {
		/* Get signal strength. */
		memset(&tuner, 0, sizeof(tuner));
		if (ioctl(fd, VIDIOC_G_TUNER, &tuner) == -1) {
			perror("ioctl() (VIDIOC_G_TUNER) failed");
			goto radio_error;
		}
		printf("\033[1A\033[32m%6.2lf MHz %3d dBuV %s\033[0m\n",
		       f / 1000000.,
		       (int)(tuner.signal / 873.8),
		       (tuner.rxsubchans & V4L2_TUNER_SUB_STEREO) ?
		       "[STEREO]" : "        ");
		sleep(1);
	}
	printf("\n");

	/* Disconnect ports.*/
	if (jack_disconnect(client, IN_PORT2, OUT_PORT2)) {
		fprintf(stderr, "cannot disconnect ports\n");
		goto radio_error;
	}

	if (jack_disconnect(client, IN_PORT1, OUT_PORT1)) {
		fprintf(stderr, "cannot disconnect ports\n");
		goto radio_error;
	}

	/* Stop Jack client. */
	jack_client_close(client);

	/* Stop Jack server. */
	if (kill(pid, SIGKILL) == -1) {
		perror("kill() failed");
		goto radio_error;
	}

	if (waitpid(pid, &s, 0) == -1) {
		perror("waitpid() failed");
		goto radio_error;
	}

	/* Enable mute. */
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_AUDIO_MUTE;
	control.value = -1;
	if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) {
		perror("ioctl() (VIDIOC_S_CTRL) failed");
		goto radio_error;
	}

	/* Close device. */
	if (close(fd) == -1) {
		perror("close() failed");
		return 1;
	}

	return 0;

radio_error:
	if (fd != -1)
		close(fd);
	if (pid != -1 && pid) {
		kill(pid, SIGKILL);
		waitpid(pid, &s, 0);
	}

	return 1;
}

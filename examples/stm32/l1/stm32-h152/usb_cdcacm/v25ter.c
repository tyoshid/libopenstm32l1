/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Toshiaki Yoshida <yoshida@mpc.net>
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
 */

#include <libopencm3.h>

#include <syscall.h>
#include <ctype.h>
#include <string.h>

#include "usb_cdcacm.h"
#include "v25ter.h"

#define CMDSIZE 64

typedef enum {
	V25TER_STATE_NONE,
	V25TER_STATE_PREFIX,
	V25TER_STATE_BODY
} v25ter_state_t;

static v25ter_state_t state;
static v25ter_state_t state_encap;

/* DTE-DCE local flow control (+IFC) */
static char *ifc(char *cmd)
{
	extern bool rts_enable;
	extern bool cts_enable;

	if (strcmp(cmd, "+IFC?") == 0) {
		/* Read */
		/* +IFC:<DCE_by_DTE>,<DTE_by_DCE> */
		if (rts_enable) {
			if (cts_enable)
				return "\r\n+IFC:2,2\r\nOK\r\n";
			else
				return "\r\n+IFC:2,0\r\nOK\r\n";
		} else {
			if (cts_enable)
				return "\r\n+IFC:0,2\r\nOK\r\n";
			else
				return "\r\n+IFC:0,0\r\nOK\r\n";
		}
	} else if (strcmp(cmd, "+IFC=") == 0) {
		/* No parameter? */
		return "\r\nOK\r\n";
	} else if (strcmp(cmd, "+IFC=?") == 0) {
		/* Test */
		/*
		 * +IFC:(list of supported <DCE_by_DTE> values),
		 *      (list of supported <DTE_by_DCE> values)
		 */
		return "\r\n+IFC:(0,2),(0,2)\r\nOK\r\n";
	} else if (strcmp(cmd, "+IFC=0") == 0) {
		/* Disable flow control (RTS and CTS). */
		usart_set_flow(false, false);
		return "\r\nOK\r\n";
	} else if (strcmp(cmd, "+IFC=2") == 0) {
		/* Enable hardware flow control (RTS and CTS). */
		usart_set_flow(true, true);
		return "\r\nOK\r\n";
	} else if (strcmp(cmd, "+IFC=0,0") == 0) {
		/* Disable RTS and CTS flow control. */
		usart_set_flow(false, false);
		return "\r\nOK\r\n";
	} else if (strcmp(cmd, "+IFC=0,2") == 0) {
		/* Disable RTS flow control and enable CTS flow control. */
		usart_set_flow(false, true);
		return "\r\nOK\r\n";
	} else if (strcmp(cmd, "+IFC=2,0") == 0) {
		/* Enable RTS flow control and disable CTS flow control. */
		usart_set_flow(true, false);
		return "\r\nOK\r\n";
	} else if (strcmp(cmd, "+IFC=2,2") == 0) {
		/* Enable RTS and CTS flow control. */
		usart_set_flow(true, true);
		return "\r\nOK\r\n";
	}

	return "\r\nERROR\r\n";
}

static char *command(char *cmd)
{
	char buf[CMDSIZE];
	int i;
	extern volatile bool command_state;

	/* Delete space. */
	i = 0;
	while (*cmd) {
		if (!isspace((int)*cmd))
			/* Convert the character to upper case. */
			buf[i++] = toupper((int)*cmd);
		cmd++;
	}
	buf[i] = '\0';

	if (buf[0] == '\0') {
		/* No command */
		return "\r\nOK\r\n";
	} else if (buf[0] == 'O') {
		/* Return to online data state */
		command_state = false;
		return "\r\nCONNECT\r\n";
	} else if (strncmp(buf, "+IFC", 3) == 0) {
		/* DTE-DCE local flow control */
		return ifc(buf);
	}

	return "\r\nERROR\r\n";
}

char *v25ter_input(int c, char *echo)
{
	static char prev_cmd[CMDSIZE];
	static char cmd[CMDSIZE];
	static char *cmd_p;

	/*
	 * A command line is made up of three elements: the prefix, the body,
	 * and the terminate character.
	 */
	switch (state) {
	case V25TER_STATE_NONE:
		/* Echo the character. */
		*echo++ = c;
		*echo = '\0';

		/* "AT" or "at" or "A/" or "a/" */
		if (toupper(c) == 'A')
			state = V25TER_STATE_PREFIX;
		break;
	case V25TER_STATE_PREFIX:
		/* Echo the character. */
		*echo++ = c;
		*echo = '\0';

		if (c == '/') {
			/* "A/" or "a/" */
			/* Repeat the execution of the previous command line. */
			state = V25TER_STATE_NONE;
			return command(prev_cmd);
		} else if (toupper(c) == 'T') {
			/* "AT" or "at" */
			cmd_p = cmd;
			state = V25TER_STATE_BODY;
		} else {
			state = V25TER_STATE_NONE;
		}
		break;
	case V25TER_STATE_BODY:
		if (c == '\r') {
			/* the termination character */

			/* no echo */
			*echo = '\0';

			/* EOS */
			*cmd_p = '\0';

			/* Save the command line. */
			strcpy(prev_cmd, cmd);

			/* Execute the command line. */
			state = V25TER_STATE_NONE;
			return command(cmd);
		} else {
			if (c == '\b') {
				/* the command line editing character */

				if (cmd_p != cmd) {
					/* Delete the previous character. */
					*echo++ = '\b';
					*echo++ = ' ';
					*echo++ = '\b';
					*echo = '\0';

					/* Decrement pointer. */
					cmd_p--;
				} else {
					/* no echo */
					*echo = '\0';
				}
			} else {
				if (cmd_p < cmd + CMDSIZE - 1) {
					/* Echo the character. */
					*echo++ = c;
					*echo = '\0';

					/* Save the command character. */
					*cmd_p++ = c;
				} else {
					/* no echo */
					*echo = '\0';
				}
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

char *v25ter_input_encap(int c)
{
	static char prev_cmd[CMDSIZE];
	static char cmd[CMDSIZE];
	static char *cmd_p;

	/*
	 * A command line is made up of three elements: the prefix, the body,
	 * and the terminate character.
	 */
	switch (state_encap) {
	case V25TER_STATE_NONE:
		/* "AT" or "at" or "A/" or "a/" */
		if (toupper(c) == 'A')
			state_encap = V25TER_STATE_PREFIX;
		break;
	case V25TER_STATE_PREFIX:
		if (c == '/') {
			/* "A/" or "a/" */
			/* Repeat the execution of the previous command line. */
			state_encap = V25TER_STATE_NONE;
			return command(prev_cmd);
		} else if (toupper(c) == 'T') {
			/* "AT" or "at" */
			cmd_p = cmd;
			state_encap = V25TER_STATE_BODY;
		} else {
			state_encap = V25TER_STATE_NONE;
		}
		break;
	case V25TER_STATE_BODY:
		if (c == '\r') {
			/* the termination character */

			/* EOS */
			*cmd_p = '\0';

			/* Save the command line. */
			strcpy(prev_cmd, cmd);

			/* Execute the command line. */
			state_encap = V25TER_STATE_NONE;
			return command(cmd);
		} else {
			if (c == '\b') {
				/* the command line editing character */

				if (cmd_p != cmd) {
					/* Decrement pointer. */
					cmd_p--;
				}
			} else {
				if (cmd_p < cmd + CMDSIZE - 1) {
					/* Save the command character. */
					*cmd_p++ = c;
				}
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

void v25ter_reset(void)
{
	/* Reset state. */
	state = V25TER_STATE_NONE;
	state_encap = V25TER_STATE_NONE;
}

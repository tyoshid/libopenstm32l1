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

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#undef errno
extern int errno;

int _chown(const char *path, uid_t owner, gid_t group);
int _close(int fildes);
int _execve(char *name, char **argv, char **env);
int _fork(void);
int _fstat(int fildes, struct stat *st);
int _getpid(void);
int _gettimeofday(struct timeval *ptimeval, void *ptimezone);
int _isatty(int file);
int _kill(int pid, int sig);
int _link(char *existing, char *new);
int _lseek(int file, int ptr, int dir);
int _open(char *file, int flags, int mode);
int _read(int file, char *ptr, int len);
int _readlink(const char *path, char *buf, size_t bufsize);
void *_sbrk(int incr);
int _stat(const char *file, struct stat *st);
int _symlink(const char *path1, const char *path2);
clock_t _times(struct tms *buf);
int _unlink(char *name);
int _wait(int *status);
int _write(int file, char *ptr, int len);

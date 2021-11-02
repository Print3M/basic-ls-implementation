/*
	Super-basic implementation of `ls` command. 
	Probably buggy. For learning purpose only. Not production ready.
	
	Author: Print3M

	Read more about this code on my blog:
		http://print3m.github.io
*/

#define _GNU_SOURCE
#include "utils.h"
#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

const size_t READ_BUF_SIZE = 4096;

int main(int argc, char *argv[]) {
	char pathbuf[PATH_MAX];

	// If not specified, get current directory
	char *path = realpath(argc == 2 ? argv[1] : ".", pathbuf);

	// Get file descriptor of directory
	int fd = open(path, O_RDONLY | O_DIRECTORY);
	if (fd == -1) {
		puts("Error: failed to open the file.");
		exit(1);
	}

	char *direntsbuf = malloc(READ_BUF_SIZE);
	struct linux_dirent64 *dirent;
	struct stat *inode_stat = malloc(sizeof(struct stat));

	while (true) {
		// n_read = number of read bytes
		int n_read = syscall(SYS_getdents64, fd, direntsbuf, READ_BUF_SIZE);

		if (n_read == -1) {
			puts("Error: failed to read data.");
			exit(1);
		}

		// EOF
		if (n_read == 0)
			break;

		// Read all dirent structs
		for (int bytes_offset = 0; bytes_offset < n_read;) {
			// Get dirent struct
			dirent = (struct linux_dirent64 *) (direntsbuf + bytes_offset);

			// Get file_path (2 extra bytes for slash and null-terminator)
			char *file_path = malloc(strlen(path) + strlen(dirent->d_name) + 2);
			get_file_path(file_path, path, dirent->d_name);

			// Get i-node info (stat struct) of file
			stat(file_path, inode_stat);

			// Format and print directory entry
			print_dirent(dirent, inode_stat);

        	// Get offset to next dirent struct
			bytes_offset += dirent->d_reclen;
			free(file_path);
		}
	}

	free(direntsbuf);
	free(inode_stat);
}
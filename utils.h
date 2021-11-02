#pragma once
#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>

// ASCII control codes
#define RESET		 "\033[0m"
#define BOLD_RED	 "\033[1;31m"
#define BOLD_GREEN	 "\033[1;32m"
#define BOLD_YELLOW	 "\033[01;33m"
#define BOLD_BLUE	 "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN	 "\033[1;36m"

// Dirent type
#define D_SYMBOLIC 1
#define D_DIR	   4
#define D_FILE	   8

struct linux_dirent64 {
	ino_t d_ino;				 /* 64-bit inode number */
	off_t d_off;				 /* 64-bit offset to next structure */
	unsigned short int d_reclen; /* size of this dirent */
	unsigned char d_type;		 /* file type */
	char d_name[256];			 /* filename (null-terminated) */
};

void get_file_path(char *pathbuf, const char *path, const char *filename);

void print_dirent(struct linux_dirent64 *dirent, struct stat *file_stat);
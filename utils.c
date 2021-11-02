#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void get_file_path(char *pathbuf, const char *path, const char *filename) {
	strcpy(pathbuf, path);
	strcat(pathbuf, "/");
	strcat(pathbuf, filename);
}

void add_color(char *str, const char *color_code) {
	/*
		Ugly but it works.
	*/
	size_t str_len = strlen(str);
	size_t color_code_len = strlen(color_code);
	size_t reset_code_len = strlen(RESET);

	// COLOR_CODE + str + RESET_CODE + \0
	str = realloc((void *) str, color_code_len + str_len + reset_code_len + 1);

	// Shift str, make space for color code
	memcpy((void *) &str[color_code_len], (void *) str, str_len);

	// Add color code at the beginning
	memcpy((void *) str, (void *) color_code, color_code_len);

	// Add reset code and \0 at the end
	memcpy((void *) &str[color_code_len + str_len], (void *) RESET, reset_code_len);
	str[color_code_len + str_len + reset_code_len] = '\0';
}

void print_dirent(struct linux_dirent64 *dirent, struct stat *file_stat) {
	char *name = malloc(strlen(dirent->d_name) + 1);
	strcat(name, dirent->d_name);

	switch (dirent->d_type) {
		case D_SYMBOLIC:
			add_color(name, BOLD_CYAN);
			break;
		case D_DIR:
			add_color(name, BOLD_BLUE);
			break;
		case D_FILE:
			if (file_stat->st_mode & S_IXUSR) {
				// If file is executable
				add_color(name, BOLD_GREEN);
			}
			break;
	}

	printf("%s\n", name);
}
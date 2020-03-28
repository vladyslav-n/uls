#include "../inc/uls.h"

bool mx_namecmp(void *a, void *b) {
	return mx_strcmp(((t_file*)a)->name, ((t_file*)b)->name) > 0;
}

bool mx_revnamecmp(void *a, void *b) {
	return mx_strcmp(((t_file*)a)->name, ((t_file*)b)->name) < 0;
}


bool mx_modcmp(void *a, void *b) {
	if (((t_file*)b)->statp->st_mtime == ((t_file*)a)->statp->st_mtime) {
		if (MX_MT(((t_file*)b)->statp, ==, ((t_file*)a)->statp))
			return mx_namecmp(a, b);
		else
			return MX_MT(((t_file*)b)->statp, >, ((t_file*)a)->statp);
	}
	return ((t_file*)b)->statp->st_mtime > ((t_file*)a)->statp->st_mtime;
}

bool mx_revmodcmp(void *a, void *b) {
	if (((t_file*)b)->statp->st_mtime == ((t_file*)a)->statp->st_mtime) {
		if (MX_MT(((t_file*)b)->statp, ==, ((t_file*)a)->statp))
			return mx_revnamecmp(a, b);
		else
			return MX_MT(((t_file*)b)->statp, <, ((t_file*)a)->statp);
	}
	return ((t_file*)b)->statp->st_mtime < ((t_file*)a)->statp->st_mtime;
}

bool mx_str_cmp(void *a, void *b) {
    return mx_strcmp(a, b) > 0;
}

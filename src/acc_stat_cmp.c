#include "../inc/uls.h"

bool mx_acccmp(void *a, void *b) {
	if (((t_file*)b)->statp->st_atime == ((t_file*)a)->statp->st_atime) {
		if (MX_AT(((t_file*)b)->statp, ==, ((t_file*)a)->statp))
			return mx_namecmp(a, b);
		else
			return MX_AT(((t_file*)b)->statp, >, ((t_file*)a)->statp);
	}
	return ((t_file*)b)->statp->st_atime > ((t_file*)a)->statp->st_atime;
}

bool mx_revacccmp(void *a, void *b) {
	if (((t_file*)b)->statp->st_atime == ((t_file*)a)->statp->st_atime) {
		if (MX_AT(((t_file*)b)->statp, ==, ((t_file*)a)->statp))
			return mx_revnamecmp(a, b);
		else
			return MX_AT(((t_file*)b)->statp, <, ((t_file*)a)->statp);
	}
	return ((t_file*)b)->statp->st_atime < ((t_file*)a)->statp->st_atime;
}

bool mx_statcmp(void *a, void *b) {
	if (((t_file*)b)->statp->st_ctime == ((t_file*)a)->statp->st_ctime) {
		if (MX_CT(((t_file*)b)->statp, ==, ((t_file*)a)->statp))
			return mx_namecmp(a, b);
		else
			return MX_CT(((t_file*)b)->statp, >, ((t_file*)a)->statp);
	}
	return ((t_file*)b)->statp->st_ctime > ((t_file*)a)->statp->st_ctime;
}

bool mx_revstatcmp(void *a, void *b) {
	if (((t_file*)b)->statp->st_ctime == ((t_file*)a)->statp->st_ctime) {
		if (MX_CT(((t_file*)b)->statp, ==, ((t_file*)a)->statp))
			return mx_revnamecmp(a, b);
		else
			return MX_CT(((t_file*)b)->statp, <, ((t_file*)a)->statp);
	}
	return ((t_file*)b)->statp->st_ctime < ((t_file*)a)->statp->st_ctime;
}

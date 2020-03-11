
#include "ls.h"
// int (*cmp)(FTSENT *, FTSENT *);

void *nalloc(size_t size) {
	void *ptr = malloc(size);

	return mx_memset(ptr, 0, size);
}

void mx_paste_node(t_list **list, void *data) {  // TO BE TESTED!!!
    t_list *save;

    if (!list || !*list || !(save = (*list)->next)) {
        mx_push_back(list, data);
        return;
    }
    mx_push_front(&save, data);
    (*list)->next = save;
}

bool mx_is_dot(char *name) {
    return !mx_strcmp(name, ".") || !mx_strcmp(name, "..");
}


bool namecmp(FTSENT *a, FTSENT *b) {
	return mx_strcmp(a->fts_name, b->fts_name) > 0;
}

bool revnamecmp(FTSENT *a, FTSENT *b) {
	return mx_strcmp(a->fts_name, b->fts_name) < 0;
}


bool modcmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_mtime == a->fts_statp->st_mtime) {
		if (MX_MT(b->fts_statp, ==, a->fts_statp))
			return namecmp(a, b);
		else
			return MX_MT(b->fts_statp, >, a->fts_statp);
	}
	return b->fts_statp->st_mtime > a->fts_statp->st_mtime;
}

bool revmodcmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_mtime == a->fts_statp->st_mtime) {
		if (MX_MT(b->fts_statp, ==, a->fts_statp))
			return revnamecmp(a, b);
		else
			return MX_MT(b->fts_statp, <, a->fts_statp);
	}
	return b->fts_statp->st_mtime < a->fts_statp->st_mtime;
}

bool acccmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_atime == a->fts_statp->st_atime) {
		if (MX_AT(b->fts_statp, ==, a->fts_statp))
			return namecmp(a, b);
		else
			return MX_AT(b->fts_statp, >, a->fts_statp);
	}
	return b->fts_statp->st_atime > a->fts_statp->st_atime;
}

bool revacccmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_atime == a->fts_statp->st_atime) {
		if (MX_AT(b->fts_statp, ==, a->fts_statp))
			return revnamecmp(a, b);
		else
			return MX_AT(b->fts_statp, <, a->fts_statp);
	}
	return b->fts_statp->st_atime < a->fts_statp->st_atime;
}

bool statcmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_ctime == a->fts_statp->st_ctime) {
		if (MX_CT(b->fts_statp, ==, a->fts_statp))
			return namecmp(a, b);
		else
			return MX_CT(b->fts_statp, >, a->fts_statp);
	}
	return b->fts_statp->st_ctime > a->fts_statp->st_ctime;
}

bool revstatcmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_ctime == a->fts_statp->st_ctime) {
		if (MX_CT(b->fts_statp, ==, a->fts_statp))
			return revnamecmp(a, b);
		else
			return MX_CT(b->fts_statp, <, a->fts_statp);
	}
	return b->fts_statp->st_ctime < a->fts_statp->st_ctime;
}

bool sizecmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_size == a->fts_statp->st_size)
		return namecmp(a, b);
	return b->fts_statp->st_size > a->fts_statp->st_size;
}

bool revsizecmp(FTSENT *a, FTSENT *b) {
	if (b->fts_statp->st_size == a->fts_statp->st_size)
		return revnamecmp(a, b);
	return b->fts_statp->st_size < a->fts_statp->st_size;
}

bool mastercmp(FTSENT *a, FTSENT *b) {
	int a_info, b_info;

	a_info = a->fts_info;
	if (a_info == FTS_ERR)
		return 0;
	b_info = b->fts_info;
	if (b_info == FTS_ERR)
		return 0;
	// if (a_info != b_info && a->fts_level == FTS_ROOTLEVEL && !f.listdir) {
	// 	if (a_info == FTS_D)
	// 		return (1);
	// 	if (b_info == FTS_D)
	// 		return (-1);
	// }
	return sortfcn(a, b);
}

bool mx_compar(void *a, void *b) {
	return mastercmp ((FTSENT *)a, (FTSENT *)b);
}

int mx_fts_chlist_size(FTSENT *list) {
    int count = 0;

    if (!list) {
        return count;
    }
    while (list->fts_link) {
        list = list->fts_link;
        count += 1;
    }
    return count + 1;
}

static void swap_nodes(FTSENT **n1, FTSENT **n2) {
    FTSENT *tmp;
    
    tmp = *n1;
    *n1 = *n2;
    *n2 = tmp;
}

static void qsort_fts_chlist(FTSENT **list, bool (*cmp)(FTSENT *, FTSENT *),
int left, int right) {
    int i = left;
    int j = right;
    void *pivot = list[(left + right) / 2];

    while (i <= j) {
        while ((*cmp)(pivot, list[i])) 
            i++;
        while ((*cmp)(list[j], pivot))
            j--;
        if (i <= j) {
            swap_nodes(&list[i], &list[j]);
            i++;
            j--;
        }
    }
    if (left < j)
        qsort_fts_chlist(list, cmp, left, j);
    if (i < right)
        qsort_fts_chlist(list, cmp, i, right);
}


FTSENT *mx_sort_fts_chlist(FTSENT *lst, bool (*cmp)(FTSENT *, FTSENT *)) {
    int size = -1;
    FTSENT **arr = NULL;
    FTSENT *save = NULL;

    if (!lst || !cmp)
        return lst;
    size = mx_fts_chlist_size(lst);
    arr = (FTSENT**) malloc(sizeof(FTSENT*) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = lst;
        lst = lst->fts_link;
    }
    qsort_fts_chlist(arr, cmp, 0, size - 1);
    for (int i = 0; i < size - 1; i++)
        arr[i]->fts_link = arr[i + 1];
    arr[size - 1]->fts_link = NULL;
    save = arr[0];
    free(arr);
    arr = NULL;
    return save;
}


// short mx_fts_level(short opt) {
//     static short level;

//     if (opt > 0)
//         level++;
//     if (opt < 0)
//         level--;
//     return level;
// }

bool mx_islink_stat(char *path) {
    struct stat s_stp;

    lstat(path, &s_stp);
    return S_ISLNK(s_stp.st_mode);
}

short mx_type_stat(char *path) {
    struct stat s_stp;

    lstat(path, &s_stp);
    if S_ISLNK(s_stp.st_mode)
        return DT_LNK;
    if S_ISDIR(s_stp.st_mode)
        return DT_DIR;
    return DT_REG;
}

bool mx_islink_dirent(struct dirent *file) {
    return (file->d_type == DT_LNK);
}

bool mx_file_exist(char *name) {
    struct stat sst;

    return !lstat(name, &sst);
}

int mx_link_info(char *name) {
    struct stat sst;

    if (stat(name, &sst) == -1)
        return FTS_SLNONE;
    if (S_ISDIR(sst.st_mode))
        return FTS_D;
    return FTS_DEFAULT;
}

FTSENT *mx_parent_of_root() { //done
    FTSENT *np = nalloc(sizeof(FTSENT));

    np->fts_name[0] = '\0';
    np->fts_namelen = 0;
    np->fts_path[0] = '\0';
    np->fts_pathlen = 0;
    np->fts_accpath[0] = '\0';
    np->fts_accpathlen = 0;
    np->fts_pointer = NULL;
    np->fts_number = 0;
    np->fts_link = NULL;
    np->fts_level = FTS_ROOTPARENTLEVEL;
    np->fts_statp = NULL;
    np->fts_info = FTS_DEFAULT;
    np->fts_errno = 0;
    return np;
}

FTSENT *mx_enoent(char *name) { //done
    FTSENT *np = nalloc(sizeof(FTSENT));

    mx_strcpy(np->fts_name, name);
    np->fts_namelen = mx_strlen(name); /* name done */
    mx_strcpy(np->fts_accpath, np->fts_name);
    np->fts_accpathlen = np->fts_namelen; /* accpath done */
    np->fts_level = FTS_ROOTLEVEL;
    np->fts_info = FTS_ERR;
    np->fts_errno = ENOENT;
    return np;
}

short mx_fts_root_info(char *name, int options) {
    struct stat sst;

    lstat(name, &sst);
    if (S_ISDIR(sst.st_mode))
        return FTS_D;
    if(!S_ISLNK(sst.st_mode))
        return FTS_DEFAULT;
    if (options & (FTS_COMFOLLOW | FTS_LOGICAL)) {
        return mx_link_info(name);
    }
    return FTS_DEFAULT;
}

short mx_fts_dirent_info(struct dirent *ptr, int options) {
    if (ptr->d_type == DT_DIR)
        return FTS_D;
    if (ptr->d_type != DT_LNK)
        return FTS_DEFAULT;
    if (options & FTS_LOGICAL)
        return mx_link_info(ptr->d_name);
    return FTS_DEFAULT;
}

FTSENT *mx_root_file(char *name, int options) {
    FTSENT *np = nalloc(sizeof(FTSENT));
    struct stat *stsp;

    mx_strcpy(np->fts_name, name);
    np->fts_namelen = mx_strlen(name); /* name done */
    mx_strcpy(np->fts_accpath, np->fts_name);
    np->fts_accpathlen = np->fts_namelen; /* accpath done */
    // np->fts_path[0] = '\0';
    // np->fts_pathlen = 0; /* root path done */
    // np->fts_parent = mx_parent_of_root();
    // np->fts_pointer = NULL;
    // np->fts_number = 0;
    // np->fts_link = NULL;
    np->fts_level = FTS_ROOTLEVEL;
    // np->fts_errno = 0;
    np->d_type = mx_type_stat(name);
    np->fts_info = mx_fts_root_info(name, options);
    if (options & FTS_NAMEONLY) {  //broken link - FIX!!//
        np->fts_statp = NULL;
        return np;
    }
    stsp = malloc(sizeof(struct stat)); 
    if (np->fts_info == FTS_SLNONE) 
        lstat(np->fts_accpath, stsp);
    else
        statp(np->fts_accpath, stsp);
    np->fts_statp = stsp;
    return np;
}

FTSENT *mx_dirent_to_ftsent(FTSENT *p, struct dirent *stdp, int options) {
    FTSENT *np = nalloc(sizeof(FTSENT));
    struct stat *stsp;

    if (!stdp)
        return NULL;
    mx_strcpy(np->fts_name, stdp->d_name);
    np->fts_namelen = stdp->d_namlen;
    if (p) {
        mx_strcpy(np->fts_path, p->fts_accpath);
        mx_strcpy(np->fts_path + p->fts_accpathlen, "/");
        np->fts_pathlen = p->fts_accpathlen + 1; /* root path done */
        mx_strcpy(np->fts_accpath, np->fts_path);
        mx_strcpy(np->fts_accpath + np->fts_pathlen, np->fts_name);
        np->fts_accpathlen = np->fts_pathlen + np->fts_namelen; 
    }
    np->fts_parent = p;
    // np->fts_pointer = NULL;
    // np->fts_number = 0;
    // np->fts_link = NULL;
    np->fts_level = p->fts_level + 1;
    // np->fts_errno = 0;
    np->d_type = stdp->d_type;
    np->fts_info = mx_fts_dirent_info(stdp, options);
    if (options & FTS_NAMEONLY) {  //broken link - FIX!!//
        // np->fts_statp = NULL;
        return np;
    }
    stsp = malloc(sizeof(struct stat)); 
    if (np->fts_info == FTS_SLNONE)
        lstat(np->fts_accpath, stsp);
    else
        statp(np->fts_accpath, stsp);
    np->fts_statp = stsp;
    return np;
}

void mx_follow_link(char *name, char *buf, int size) {
    int len;
    char *bp;
    char tbuf[MAXPATHLEN + 1];

    if (!*buf)
        mx_strcpy(buf, name);
    len = readlink(buf, tbuf, MAXPATHLEN + 1);
    tbuf[len] = '\0';
    if (*tbuf != '/' && (bp = mx_memrchr(buf, '/', size)))
        mx_strcpy(bp + 1, tbuf);
    else
        mx_strcpy(buf, tbuf);
    if (mx_islink_stat(buf))
        mx_follow_link(buf, buf, size);
}

void mx_fts_child_push(FTSENT **last_child, FTSENT *child,
FTSENT **ch_list_start) {
    if (!*last_child) {
        *last_child = child;
        *ch_list_start = child;
        return;
    }
    (*last_child)->fts_link = child;
}


FTSENT *mx_fts_children_1(t_ftslist *list, int options,
                          bool (*cmp)(FTSENT *, FTSENT *)) {
    DIR *dp;
    struct dirent *stdp;
    char path_buf[MAXPATHLEN + 1] = "";
    FTSENT *last_child = NULL;
    FTSENT *ch_list_start = NULL;

    if (!list || list->data->fts_info != FTS_D || (options & FTS_SEEDOT
        && list->data->fts_level != 0 && mx_is_dot(list->data->fts_name)))
        return NULL;
    if (list->data->d_type != DT_LNK)
        dp = opendir(list->data->fts_accpath);
    else {
        mx_follow_link(list->data->fts_accpath, path_buf, MAXPATHLEN + 1);
        dp = opendir(path_buf);
    }
    while ((stdp = readdir(dp))) {
        if (!(options & FTS_SEEDOT) && mx_is_dot(stdp->d_name))
            continue;
        mx_fts_child_push(&last_child,
        mx_dirent_to_ftsent(list->data, stdp, options), &ch_list_start);
        if (last_child && last_child->fts_link)
            last_child = last_child->fts_link;
    }
    closedir(dp);
    return mx_sort_fts_chlist(ch_list_start, cmp);;
}

void mx_children_clear(FTSENT *p) {
    FTSENT *save = p;

    for (FTSENT *tmp = save; tmp; tmp = save->fts_link) {
        save = tmp;
        free(tmp->fts_statp);
        free(tmp);
        // if (tmp->fts_level == FTS_ROOTLEVEL)
        //     free(tmp->fts_parent);
    }
}

short mx_fts_paste_chlist(t_ftslist *parent, int options,
                          bool (*cmp)(FTSENT *, FTSENT *)) {
    FTSENT *ch_list = mx_fts_children_1(parent, options, cmp);

    if (!ch_list)
        return -1;
    for (FTSENT *p = ch_list; p; p = p->fts_link) {
        mx_paste_node((t_list **)&parent, p);
        parent = parent->next;
    }
    return 0;
}

void mx_push_ftsent(t_ftslist **list, FTSENT *ptr) {
    if (!list || !ptr)
        return;
    mx_push_back((t_list**)list, ptr);
    if((*list)->next)
        *list = (*list)->next;
}

short mx_fts_push_name(t_ftslist **list, char *name, int options,
                       t_list **errlist) {
    // t_ftslist *p;
    // t_ftslist *save;
    // short flag;

    if (!list)
        return -2;
    if (!mx_file_exist(name)) {
        mx_push_front(errlist, name);
        return -1;
    }
    mx_push_ftsent(list, mx_root_file(name, options));
    return 0;
    // for (p = *list; p; p = p->next) {
    //     mx_fts_paste_chlist(p, options, cmp);
    //     if (!p->next)
    //         save = p;
    // }
    // return save;
}

void mx_printerr_errnum(char *name, int errnum) {
    mx_printerror("uls: ");
    mx_printerror(name);
    mx_printerror(": ");
    mx_printerror(strerror(errnum));
    mx_printerror("\n");
}

bool mx_str_cmp(void *a, void *b) {
    return mx_strcmp(a, b) > 0;
}

void mx_print_errlist_names(t_list *errlist, bool sort) {
    errlist = mx_sort_list(errlist, sort ? mx_str_cmp : NULL);
    for (; errlist; mx_pop_front(&errlist))
        mx_printerr_errnum(errlist->data, ENOENT);
}

t_ftslist *mx_push_roots(t_ftslist **list, char **argv, int options,
                         bool (*cmp)(FTSENT *, FTSENT *)) {
    t_ftslist *cur = NULL;
    t_list *errlist = NULL;
    char *name;
    short flag = 0;

    if (!list)
        return NULL;
    for (name = *argv; (name = *argv); argv++) {
        if (!(flag = mx_fts_push_name(&cur, name, options, &errlist))
            && !*list)
            *list = cur;
    }
    mx_print_errlist_names(errlist, cmp ? 1 : 0);
    *list = (t_ftslist *)mx_sort_list((t_list *)*list, cmp ? mx_compar : 0);
    if (options & FTS_NORECUR)
        return *list;
    for (cur = *list; cur; cur = cur->next) {
        mx_fts_paste_chlist(cur, options, cmp);
        // if (!cur->next)
        //     save = cur;
    }
    return *list;
}

FTSENT *mx_argv_chlist(char **argv, int options,
                          bool (*cmp)(FTSENT *, FTSENT *)) {
    FTSENT *last_child = NULL;
    FTSENT *ch_list_start = NULL;
    char *name;

    if (!argv || !*argv)
        return NULL;
    for (name = *argv; (name = *argv); argv++) {
        if (mx_file_exist(name))
            mx_fts_child_push(&last_child, mx_root_file(name, options),
                          &ch_list_start);
        if (last_child && last_child->fts_link)
            last_child = last_child->fts_link;
    }
    return mx_sort_fts_chlist(ch_list_start, cmp);;
}


FTS *mx_fts_open(char **path_argv, int options,
              bool (*cmp)(FTSENT *, FTSENT *)) {
    FTS *ftsp = (FTS *)nalloc(sizeof(FTS));

    mx_push_roots(&ftsp->head, path_argv, options, cmp);
    ftsp->cur = ftsp->head;
    ftsp->cmp = cmp;
    ftsp->fts_options = options;
    ftsp->argv = path_argv;
    return ftsp;
}

FTSENT *mx_fts_read(FTS *ftsp) {
    if (!ftsp->cur)
        return NULL;
    if (ftsp->read_launched)
        ftsp->cur = ftsp->cur->next;
    ftsp->read_launched = 1;
    return ftsp->cur ? ftsp->cur->data : NULL;
}

FTSENT *mx_fts_children(FTS *ftsp, int options) {
    if (ftsp->read_launched)
        return mx_fts_children_1(ftsp->cur, options, ftsp->cmp);
    return mx_argv_chlist(ftsp->argv, options, ftsp->cmp);
}

void mx_fts_skip_ch(FTS *ftsp) {
    // FTSENT *p;
    int level;
    t_ftslist *cur;

    if (!ftsp || !ftsp->cur || !ftsp->cur->data)
        return;
    cur = ftsp->cur;
    level = cur->data->fts_level;
    for (; cur->next && cur->next->data->fts_level > level;)
        cur = cur->next;
    ftsp->cur = cur;
}


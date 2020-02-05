#ifndef gnb_dir_h
#define gnb_dir_h

char *gnb_real_dirname_dup(char *path);

int gnb_get_sub_dirname(char *path, int max_filename_len, char **filename_lst, int *filename_lst_len);

#endif

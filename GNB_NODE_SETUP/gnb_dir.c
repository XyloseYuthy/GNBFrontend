#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>

char *gnb_real_dirname_dup(char *path){

    char *string = strdup(path);

    char *resolved_path = (char *)malloc(PATH_MAX);

    size_t string_len = strlen( (const char *)string );

    int i;

    for ( i=string_len-1; i>0; i-- ) {

        if ( '/'==string[i] || '\\'==string[i] ){

            string[i]='\0';
            break;
        }

    }

#ifdef _WIN32
    if ( NULL == _fullpath(resolved_path, string, 1024) ){
        free(string);
        free(resolved_path);
        return NULL;
    }

    free(string);
    return resolved_path;
#else

    if ( NULL == realpath(string,resolved_path) ){
        free(string);
        free(resolved_path);
        return NULL;
    }

    free(string);

    return resolved_path;

#endif

}

int gnb_get_sub_dirname(char *path, int max_filename_len, char **filename_lst, int *filename_lst_len) {

    DIR *dir;

    struct dirent *sub_dirent;

    dir = opendir (path);

    if (NULL==dir){
        *filename_lst_len = 0;
        return -1;
    }


    int list_len = 0;
#ifdef _WIN32
    char testdir_name[1024];
#endif
    while ( NULL !=( sub_dirent = readdir( dir ) ) ) {

#ifdef _WIN32

   snprintf(testdir_name,1024,"%s\\%s",path,sub_dirent->d_name);


    DIR *testdir =  opendir(testdir_name);
    if(NULL==testdir){
        continue;
    }
    closedir( testdir );

#else
        if ( DT_DIR != sub_dirent->d_type ){
          continue;
        }
#endif
        if (strlen(sub_dirent->d_name)>max_filename_len){
            continue;
        }


        if (0==strncmp(sub_dirent->d_name,".",max_filename_len)){
            continue;
        }

        if (0==strncmp(sub_dirent->d_name,"..",max_filename_len)){
            continue;
        }

        memset(filename_lst[list_len],0,max_filename_len);
        strncpy(filename_lst[list_len],sub_dirent->d_name,strlen(sub_dirent->d_name));

        list_len++;

    }

    closedir( dir );

    *filename_lst_len = list_len;

   return 0;
}


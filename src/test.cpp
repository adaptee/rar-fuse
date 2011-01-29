#define FUSE_USE_VERSION 25

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <locale.h>
#include <errno.h>
#include <fcntl.h>

#include <fuse.h>
#include <fuse/fuse_opt.h>

#include "fs.h"

static int rarfs_getattr(   const char * p,
                            struct stat * stbuf )
{

}

static int rarfs_readdir(const char *p, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi)
{
}

static int rarfs_open(const char *p, fuse_file_info *fi)
{
}

static int rarfs_read(const char *p, char *buf, size_t size, off_t offset, fuse_file_info *fi)
{
}


static struct fuse_operations rarfs_oper;
static struct fuse_opt rarfs_opts[] = { } ;
static FileSystem * fs = NULL;

static int rarfs_opt_proc(void *data, const char *arg, int key,
        struct fuse_args *outargs)
{

    // initialize locale
    setlocale(LC_ALL, "");

    char *tmp = (char *)data;

    switch (key) {
        case FUSE_OPT_KEY_NONOPT:
            if( tmp[0] == 0 )
            {
                strcpy(tmp, arg);
                return 0;
            }
            return 1;
    }

}

int main( int argc, char ** argv)
{
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    char archive_name[512] = { 0x0, };
    if (fuse_opt_parse(&args, archive_name, rarfs_opts, rarfs_opt_proc) == -1)
        exit(1);

    //FileSystem fs(argv[1]);
    fs = new FileSystem (argv[1]);

    rarfs_oper.getattr = rarfs_getattr;
    rarfs_oper.readdir = rarfs_readdir;
    rarfs_oper.open = rarfs_open;
    rarfs_oper.read = rarfs_read;

    return fuse_main(args.argc, args.argv, &rarfs_oper);

}

////////////////////////////////////////

//#include <fstream>
//#include <iostream>
//using std::ios;
//using std::wcout;

//#include <locale.h>

//#include "archive.h"
//#include "fs.h"

//int main(int argc, char const* argv[])
//{
    //// initialize locale
    //setlocale(LC_ALL, "");

    //FileSystem fs(argv[1]);
    //std::wcout<<fs.debugRepr3();

    //return 0;
//}




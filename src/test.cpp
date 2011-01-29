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
#include <sys/stat.h>

#include <fuse.h>
#include <fuse/fuse_opt.h>

#include "fs.h"

static FileSystem * fs = NULL;

static int rarfs_getattr(   const char * p,
                            struct stat * stbuf )
{
    wchar_t wcharbuf[1024] = {0x0,};
    mbstowcs(wcharbuf, p, sizeof(wcharbuf)/sizeof(wcharbuf[0]) );
    wstring name(wcharbuf);

    const struct stat * ret = fs->getStatus(name);

    if ( ret)
    {
        // this may be not needed, because fuse  already guarantee the zeroness.
        //memset(stbuf, 0, sizeof(struct stat));

        stbuf->st_mode  = ret->st_mode ;
        stbuf->st_size  = ret->st_size;
        stbuf->st_nlink = ret->st_nlink;
        stbuf->st_atime = ret->st_atime;
        stbuf->st_mtime = ret->st_mtime;

        return 0;
    }

    return -1 ;
}

static int rarfs_readdir(const char * p, void * buf,
                         fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    char charbuf[1024]     = {0x0,};
    wchar_t wcharbuf[1024] = {0x0,};

    mbstowcs(wcharbuf, p, sizeof(wcharbuf)/sizeof(wcharbuf[0]) );
    wstring name( wcharbuf );

    vector<wstring> names = fs->readDir(name);

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    vector<wstring>::const_iterator iter;

    for(iter = names.begin() ; iter != names.end(); iter++ )
    {
        memset(charbuf, 0, sizeof(charbuf) );
        const wchar_t * w_str = iter->c_str();
        wcstombs(charbuf, w_str, sizeof(charbuf) );

        filler(buf, charbuf, NULL, 0);
    }

    return 0;

}

static int rarfs_open(const char *p, fuse_file_info *fi)
{
    // open() is guarenteed to succeed.
    return 0;
}


static int rarfs_read(const char *p, char *buf, size_t size, off_t offset, fuse_file_info *fi)
{
    return 0;
}


static struct fuse_operations rarfs_oper;
static struct fuse_opt rarfs_opts[] = { } ;

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
//#include <string.h>
//#include <assert.h>

//#include <locale.h>
//#include <sys/stat.h>

//#include "archive.h"
//#include "fs.h"

//int main(int argc, char const* argv[])
//{
    //// initialize locale
    //setlocale(LC_ALL, "");

    //FileSystem fs(argv[1]);
    //std::wcout<<fs.debugRepr3();
    //std::wcout<<fs.debugRepr4();

    //std::wcout<<"assertion start!\n";

    //const struct stat * status;

    //status = fs.getStatus( wstring(L"/"));
    //assert( status);

    //status = fs.getStatus( wstring(L"/test"));
    //assert( status);

    //status = fs.getStatus(wstring(L"/test/123中国123.txt"));
    //assert( status);
    //status = fs.getStatus(wstring(L"/test/dir1"));
    //assert( status);
    //status = fs.getStatus(wstring(L"/test/dir1/blocks.py"));
    //assert( status);
    //status = fs.getStatus(wstring(L"/test/dir2"));
    //assert( status);
    //status = fs.getStatus(wstring(L"/test/dir2/flags.py"));
    //assert( status);
    //status = fs.getStatus(wstring(L"/test/xyz.txt"));
    //assert( status);

    ////vector<wstring> names = fs.readDir(L"/test");
    ////vector<wstring> names = fs.readDir(L"/test/dir1");

    //std::wcout<<names[0]<<"\n";
    //std::wcout<<names[1]<<"\n";
    //std::wcout<<names[2]<<"\n";
    //std::wcout<<names[3]<<"\n";

    //return 0;

//}




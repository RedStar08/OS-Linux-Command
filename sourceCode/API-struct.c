typedef struct _WIN32_FIND_DATA {
    DWORD dwFileAttributes; //文件属性
    FILETIME ftCreationTime; // 文件创建时间
    FILETIME ftLastAccessTime; // 文件最后一次访问时间
    FILETIME ftLastWriteTime; // 文件最后一次修改时间
    DWORD nFileSizeHigh; // 文件长度高32位
    DWORD nFileSizeLow; // 文件长度低32位
    DWORD dwReserved0; // 系统保留
    DWORD dwReserved1; // 系统保留
    TCHAR cFileName[ MAX_PATH ]; // 长文件名
    TCHAR cAlternateFileName[ 14 ]; // 8.3格式文件名
} WIN32_FIND_DATA, *PWIN32_FIND_DATA;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef struct _FILETIME {  
    DWORD dwLowDateTime;  
    DWORD dwHighDateTime;  
} FILETIME, *PFILETIME; 

typedef struct _SYSTEMTIME {  
    WORD wYear;  
    WORD wMonth;  
    WORD wDayOfWeek;  
    WORD wDay;  
    WORD wHour;  
    WORD wMinute;  
    WORD wSecond;  
    WORD wMilliseconds;  
} SYSTEMTIME, *PSYSTEMTIME;  

// compare argv to choose which command for executing
if(strcmp(argv[0],"ls")==0 || strcmp(argv[0],"dir")==0) {
    ls_dir(argv, argc);
} else if(strcmp(argv[0],"cat")==0 || strcmp(argv[0],"type")==0) {
    cat_type(argv, argc);
} else if(strcmp(argv[0],"cp")==0 || strcmp(argv[0],"copy")==0) {
    cp_copy(argv, argc);
} else if(strcmp(argv[0],"rm")==0 || strcmp(argv[0],"del")==0) {
    rm_del(argv, argc);
} else if(strcmp(argv[0],"mv")==0 || strcmp(argv[0],"move")==0) {
    mv_move(argv, argc);
} else if(strcmp(argv[0],"mkdir")==0 || strcmp(argv[0],"MKDIR")==0) {
    mkdir(argv, argc);
} else if(strcmp(argv[0],"rmdir")==0 || strcmp(argv[0],"rd")==0) {
    rmdir_rd(argv, argc);
} else if(strcmp(argv[0],"cd")==0 || strcmp(argv[0],"CD")==0) {
    cd(argv, argc);
} else if(strcmp(argv[0],"find")==0 || strcmp(argv[0],"FIND")==0) {
    find(argv, argc);
} else if(strcmp(argv[0],"exit")==0 || strcmp(argv[0],"quit")==0) {
    control = 0;
} else {
    WriteConsole(handle_out,"Not support such command yet.",strlen("Not support such command yet."),&dw,NULL);
}
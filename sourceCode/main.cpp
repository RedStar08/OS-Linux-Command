#include <windows.h>
#define info "@RedStar08 OS course jobs.\n"
#define commands "support: ls, cat, cp, rm, mv, mkdir, rmdir, cd, find \n"

extern "C"
{
    // define handle (Windows API)
    HANDLE	handle_in;
    HANDLE	handle_out;
    DWORD dw;

    // split argv form command
    void getArgv(char *command, char *argv[8], int *argc);

    // command implement
    void ls_dir(char *argv[8], int *argc);
    void cat_type(char *argv[8], int *argc);
    void cp_copy(char *argv[8], int *argc);
    void rm_del(char *argv[8], int *argc);
    void mv_move(char *argv[8], int *argc);
    void mkdir(char *argv[8], int *argc);
    void rmdir_rd(char *argv[8], int *argc);
    void cd(char *argv[8], int *argc);
    void find(char *argv[8], int *argc);

    // 主函数
    int start();
}
int start()
{
    /*  I/O handle  Windows API*/
    handle_in = GetStdHandle(STD_INPUT_HANDLE);
    handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(handle_out, info, strlen(info), &dw, NULL);
    WriteConsole(handle_out, commands, strlen(commands), &dw, NULL);

    int control = 1;
	// get command and argv than execute
    while(control)
    {
        // get current directory
        char *current_directory = (char*)malloc(sizeof(char) * MAX_PATH);
        GetCurrentDirectory(MAX_PATH,current_directory);
        // display current directory
        WriteConsole(handle_out,"\n",strlen("\n"),&dw,NULL);
        WriteConsole(handle_out,current_directory,strlen(current_directory),&dw,NULL);
        WriteConsole(handle_out,">",strlen(">"),&dw,NULL);

        // define a buffer to receive command and argv
        char *command = (char*)malloc(sizeof(char) * 512);  //1KB
        char *argv[8];  //max 8 argv(8*64 = 512)
        int *argc  = (int*)malloc(sizeof(int));

        // get command form console input buffer
        ReadConsole(handle_in, command, 512, &dw, NULL);
        // WriteConsole(handle_out, command, strlen(command), &dw, NULL);
        getArgv(command, argv, argc);
        // WriteConsole(handle_out, argv[0], strlen(argv[0]), &dw, NULL);

        // compare argv to choose which command for executing
        if(strcmp(argv[0],"ls")==0 || strcmp(argv[0],"dir")==0)
		{
			ls_dir(argv, argc);
		}
		else if(strcmp(argv[0],"cat")==0 || strcmp(argv[0],"type")==0)
		{
			cat_type(argv, argc);
		}
		else if(strcmp(argv[0],"cp")==0 || strcmp(argv[0],"copy")==0)
		{
			cp_copy(argv, argc);
		}
		else if(strcmp(argv[0],"rm")==0 || strcmp(argv[0],"del")==0)
		{
			rm_del(argv, argc);
		}
		else if(strcmp(argv[0],"mv")==0 || strcmp(argv[0],"move")==0)
		{
			mv_move(argv, argc);
		}
		else if(strcmp(argv[0],"mkdir")==0 || strcmp(argv[0],"MKDIR")==0)
		{
			mkdir(argv, argc);
		}
		else if(strcmp(argv[0],"rmdir")==0 || strcmp(argv[0],"rd")==0)
		{
			rmdir_rd(argv, argc);
		}
		else if(strcmp(argv[0],"cd")==0 || strcmp(argv[0],"CD")==0)
		{
			cd(argv, argc);
		}
		else if(strcmp(argv[0],"find")==0 || strcmp(argv[0],"FIND")==0)
		{
			find(argv, argc);
		}
		else if(strcmp(argv[0],"exit")==0 || strcmp(argv[0],"quit")==0)
        {
            control = 0;
        }
        else
        {
            WriteConsole(handle_out,"Not support such command yet.",strlen("Not support such command yet."),&dw,NULL);
        }

    }

    // close handle
    CloseHandle(handle_out);
    CloseHandle(handle_in);

    return 0;

}
int APIENTRY WinMain(HINSTANCE h1,HINSTANCE h2,LPSTR l,int n)
{
    start();
    return 0;
}

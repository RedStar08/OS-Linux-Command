// OS jobs Linux_Command
// ls,cat,cp,rm,mv,mkdir,rmdir,cd,find

#include<windows.h>

#define MAX_PATH 255
#define MAX_CAT 1024*1024

// define handle (Windows API)
HANDLE	handle_in;
HANDLE	handle_out;
DWORD dw;

// split argv form command
void getArgv(char *command, char *argv[8], int *argc)
{
    // index for scaning command
    char *command_index = command;

    // scan command string and split to argv
    for((*argc) = 0; (*command_index) != '\0' && (*argc) < 8; (*argc)++ )
    {
        // get argv
        int index = 0;
        // save the split argv
        argv[(*argc)] = (char*)malloc(sizeof(char) * 64);
        // if character is space or enter or alt than move
        while((*command_index) == ' ' || (*command_index) == '\n' || (*command_index) == '\r')
        {
            command_index++;
        }
        // remove space enter alt
        while((*command_index) != ' ' && (*command_index) != '\n' && (*command_index) != '\r' && (*command_index) != '\0')
        {
            // judge command character and save to the split argv
            *(argv[(*argc)] + index) = (*command_index);
            command_index++;
            index++;
        }
        // end of each argv
        *(argv[(*argc)] + index) = '\0';
    }
    (*argc)--;

}

//number to a char* buffer for display
char *NumberToCharBuffer(long long number )
{
    long long num = number;
    char *strBuffer = (char*)malloc(sizeof(char) * 20);
    short index = 0;
    while(num > 0)
    {
        strBuffer[index] = '0' + num%10;
        index++;
        num = num/10;
    }
    strBuffer[index] = '\0';
    char *new_strBuffer = (char*)malloc(sizeof(char) * 20);
    new_strBuffer[index--] = '\0';
    short _index = index;
    while(_index >= 0)
    {
        new_strBuffer[index - _index] = strBuffer[_index];
        _index--;
    }
    if(number == 0)
    {
        new_strBuffer[0] = '0';
        new_strBuffer[1] = '\0';
    }
    return new_strBuffer;
}

// Get a folder size
long long GetFolderSize(char *strDir)
{
	long long nSize = 0; //save folder size
    HANDLE firstFile;
    WIN32_FIND_DATA fileData;
    LARGE_INTEGER fileSize;
    // save path
    char *path = (char*)malloc(sizeof(char) * MAX_PATH);
    strcpy(path, strDir);
    strcat(path,"\\*.*");
    //first folder(.)
    firstFile = FindFirstFile(path,&fileData);
    if(firstFile != INVALID_HANDLE_VALUE)
    {
        while(FindNextFile(firstFile, &fileData))
        {
            if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //directory
            {
                if(strcmp(fileData.cFileName, "..") ==0)
                {
                    continue;
                }
                char *sub_path = (char*)malloc(sizeof(char) * MAX_PATH);
                strcpy(sub_path, strDir);
                strcat(sub_path,"\\");
                strcat(sub_path, fileData.cFileName);
                nSize += GetFolderSize(sub_path);
            }
            else // file
            {
                fileSize.LowPart = fileData.nFileSizeLow;
                fileSize.HighPart = fileData.nFileSizeHigh;
                nSize += fileSize.QuadPart;
            }
        }
    }
    FindClose(firstFile);
	return nSize;
}

// list directory
void ls_dir(char *argv[8], int *argc)
{
    // list current directory
    if((*argc) == 1)
    {
        HANDLE firstFile;
        WIN32_FIND_DATA fileData;
        LARGE_INTEGER fileSize;
        // save time info
        FILETIME lastWriteTime;
		SYSTEMTIME systemTime;

		// get current directory for fingding
        char *directoryPath = (char*)malloc(sizeof(char) * MAX_PATH);
        char *directoryName = (char*)malloc(sizeof(char) * MAX_PATH);
        GetCurrentDirectory(MAX_PATH,directoryPath);
        GetCurrentDirectory(MAX_PATH,directoryName);
		strcat(directoryName,"\\*.*");

		// create the find file handle
        firstFile = FindFirstFile(directoryName,&fileData);

        // file count info
        long file_count = 0;
        long long file_total_size = 0;

        // folder count info
        long folder_count = 0;
        long long folder_total_size = GetFolderSize(directoryPath);

        // list the directory and file
        if(firstFile != INVALID_HANDLE_VALUE) //find it
        {
            //list all
            do
            {
                // time translation
                lastWriteTime = fileData.ftLastWriteTime;
                FileTimeToLocalFileTime(&lastWriteTime, &lastWriteTime);
                FileTimeToSystemTime(&lastWriteTime, &systemTime);
                // time to char
                char *year = (char*)malloc(sizeof(char) * 4);
                short _year = systemTime.wYear;
                short index = 0;
                while(_year > 0)
                {
                    year[3 - index] = '0' + _year%10;
                    index++;
                    _year = _year/10;
                }

                char *month = (char*)malloc(sizeof(char) * 2);
                short _month = systemTime.wMonth;
                index = 0;
                while(_month > 0)
                {
                    if(_month < 10)
                    {
                        month[0] = '0';
                    }
                    month[1 - index] = '0' + _month%10;
                    index++;
                    _month = _month/10;
                }

                char *day = (char*)malloc(sizeof(char) * 2);
                short _day = systemTime.wDay;
                index = 0;
                while(_day > 0)
                {
                    if(_day < 10)
                    {
                        day[0] = '0';
                    }
                    day[1 - index] = '0' + _day%10;
                    index++;
                    _day = _day/10;
                }

                char *hour = (char*)malloc(sizeof(char) * 2);
                short _hour = systemTime.wHour;
                index = 0;
                if(_hour == 0)
                {
                    hour[0] = '0';
                    hour[1] = '0';
                }
                while(_hour > 0)
                {
                    if(_hour < 10)
                    {
                        hour[0] = '0';
                    }
                    hour[1 - index] = '0' + _hour%10;
                    index++;
                    _hour = _hour/10;
                }

                char *minute = (char*)malloc(sizeof(char) * 2);
                short _minute = systemTime.wMinute;
                index = 0;
                if(_minute == 0)
                {
                    minute[0] = '0';
                    minute[1] = '0';
                }
                while(_minute > 0)
                {
                    if(_minute < 10)
                    {
                        minute[0] = '0';
                    }
                    minute[1 - index] = '0' + _minute%10;
                    index++;
                    _minute = _minute/10;
                }

                //file size
                fileSize.LowPart = fileData.nFileSizeLow;
                fileSize.HighPart = fileData.nFileSizeHigh;
                long long file_size = fileSize.QuadPart;
                char *singleFileSize = NumberToCharBuffer(file_size);

                if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //directory
                {
                    folder_count++;

                    //display year/month/day
                    WriteConsole(handle_out,year,4,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,month,2,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,day,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display hours:minute
                    WriteConsole(handle_out,hour,2,&dw,NULL);
                    WriteConsole(handle_out,":",strlen(":"),&dw,NULL);
                    WriteConsole(handle_out,minute,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display fileName
                    WriteConsole(handle_out,"<DIR>  \t\t",strlen("<DIR>  \t\t"),&dw,NULL);
                    WriteConsole(handle_out,fileData.cFileName,strlen(fileData.cFileName),&dw,NULL);
                    WriteConsole(handle_out,"\n",strlen("\n"),&dw,NULL);

                }
                else // file
                {
                    file_count++; //the number of file
                    file_total_size += fileSize.QuadPart;

                    //display year/month/day
                    WriteConsole(handle_out,year,4,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,month,2,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,day,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display hours:minute
                    WriteConsole(handle_out,hour,2,&dw,NULL);
                    WriteConsole(handle_out,":",strlen(":"),&dw,NULL);
                    WriteConsole(handle_out,minute,2,&dw,NULL);
                    WriteConsole(handle_out,"\t",strlen("\t"),&dw,NULL);

                    //display fileName size
                    WriteConsole(handle_out,singleFileSize,strlen(singleFileSize),&dw,NULL);
                    WriteConsole(handle_out," bytes\t",strlen(" bytes\t"),&dw,NULL);
                    WriteConsole(handle_out,fileData.cFileName,strlen(fileData.cFileName),&dw,NULL);
                    WriteConsole(handle_out,"\n",strlen("\n"),&dw,NULL);
                }
            }while(FindNextFile(firstFile, &fileData));
            // file count and file total size
            char *fileCount = NumberToCharBuffer(file_count);
            char *fileTotalSize = NumberToCharBuffer(file_total_size);
            char *folderCount = NumberToCharBuffer(folder_count);
            char *folderTotalSize = NumberToCharBuffer(folder_total_size);

            // display file count
            WriteConsole(handle_out,"\t\tTotal files: ",strlen("\t\tTotal files: "),&dw,NULL);
            WriteConsole(handle_out,fileCount,strlen(fileCount),&dw,NULL);
            WriteConsole(handle_out,"\t\tTotal size: ",strlen("\t\tTotal size: "),&dw,NULL);
            WriteConsole(handle_out,fileTotalSize,strlen(fileTotalSize),&dw,NULL);
            WriteConsole(handle_out," bytes\n",strlen(" bytes\n"),&dw,NULL);
            // display folder count
            WriteConsole(handle_out,"\t\tTotal folders: ",strlen("\t\tTotal folders: "),&dw,NULL);
            WriteConsole(handle_out,folderCount,strlen(folderCount),&dw,NULL);
            WriteConsole(handle_out,"\tTotal size: ",strlen("\tTotal size: "),&dw,NULL);
            WriteConsole(handle_out,folderTotalSize,strlen(folderTotalSize),&dw,NULL);
            WriteConsole(handle_out," bytes\n",strlen(" bytes\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"ls: List directory failed.\n",strlen("ls: List directory failed.\n"),&dw,NULL);
        }
        // close handle
        FindClose(firstFile);
    }
    else if((*argc) == 2)
    {
        HANDLE firstFile;
        WIN32_FIND_DATA fileData;
        LARGE_INTEGER fileSize;
        // save time info
        FILETIME lastWriteTime;
		SYSTEMTIME systemTime;

		// get current directory for fingding
        char *directoryPath = (char*)malloc(sizeof(char) * MAX_PATH);
        GetCurrentDirectory(MAX_PATH,directoryPath);
		strcat(argv[1],"\\*.*");
		// create the find file handle
        firstFile = FindFirstFile(argv[1],&fileData);

        // file count info
        long file_count = 0;
        long long file_total_size = 0;

        // folder count info
        long folder_count = 0;
        long long folder_total_size = GetFolderSize(directoryPath);

        // list the directory and file
        if(firstFile != INVALID_HANDLE_VALUE) //find it
        {
            //list all
            while(FindNextFile(firstFile, &fileData))
            {
                // time translation
                lastWriteTime = fileData.ftLastWriteTime;
                FileTimeToLocalFileTime(&lastWriteTime, &lastWriteTime);
                FileTimeToSystemTime(&lastWriteTime, &systemTime);
                // time to char
                char *year = (char*)malloc(sizeof(char) * 4);
                short _year = systemTime.wYear;
                short index = 0;
                while(_year > 0)
                {
                    year[3 - index] = '0' + _year%10;
                    index++;
                    _year = _year/10;
                }

                char *month = (char*)malloc(sizeof(char) * 2);
                short _month = systemTime.wMonth;
                index = 0;
                while(_month > 0)
                {
                    if(_month < 10)
                    {
                        month[0] = '0';
                    }
                    month[1 - index] = '0' + _month%10;
                    index++;
                    _month = _month/10;
                }

                char *day = (char*)malloc(sizeof(char) * 2);
                short _day = systemTime.wDay;
                index = 0;
                while(_day > 0)
                {
                    if(_day < 10)
                    {
                        day[0] = '0';
                    }
                    day[1 - index] = '0' + _day%10;
                    index++;
                    _day = _day/10;
                }

                char *hour = (char*)malloc(sizeof(char) * 2);
                short _hour = systemTime.wHour;
                index = 0;
                if(_hour == 0)
                {
                    hour[0] = '0';
                    hour[1] = '0';
                }
                while(_hour > 0)
                {
                    if(_hour < 10)
                    {
                        hour[0] = '0';
                    }
                    hour[1 - index] = '0' + _hour%10;
                    index++;
                    _hour = _hour/10;
                }

                char *minute = (char*)malloc(sizeof(char) * 2);
                short _minute = systemTime.wMinute;
                index = 0;
                if(_minute == 0)
                {
                    minute[0] = '0';
                    minute[1] = '0';
                }
                while(_minute > 0)
                {
                    if(_minute < 10)
                    {
                        minute[0] = '0';
                    }
                    minute[1 - index] = '0' + _minute%10;
                    index++;
                    _minute = _minute/10;
                }

                //file size
                fileSize.LowPart = fileData.nFileSizeLow;
                fileSize.HighPart = fileData.nFileSizeHigh;
                long long file_size = fileSize.QuadPart;
                char *singleFileSize = NumberToCharBuffer(file_size);

                if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //directory
                {
                    folder_count++;

                    //display year/month/day
                    WriteConsole(handle_out,year,4,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,month,2,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,day,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display hours:minute
                    WriteConsole(handle_out,hour,2,&dw,NULL);
                    WriteConsole(handle_out,":",strlen(":"),&dw,NULL);
                    WriteConsole(handle_out,minute,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display fileName
                    WriteConsole(handle_out,"<DIR>  \t\t",strlen("<DIR>  \t\t"),&dw,NULL);
                    WriteConsole(handle_out,fileData.cFileName,strlen(fileData.cFileName),&dw,NULL);
                    WriteConsole(handle_out,"\n",strlen("\n"),&dw,NULL);

                }
                else // file
                {
                    file_count++; //the number of file
                    file_total_size += fileSize.QuadPart;

                    //display year/month/day
                    WriteConsole(handle_out,year,4,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,month,2,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,day,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display hours:minute
                    WriteConsole(handle_out,hour,2,&dw,NULL);
                    WriteConsole(handle_out,":",strlen(":"),&dw,NULL);
                    WriteConsole(handle_out,minute,2,&dw,NULL);
                    WriteConsole(handle_out,"\t",strlen("\t"),&dw,NULL);

                    //display fileName size
                    WriteConsole(handle_out,singleFileSize,strlen(singleFileSize),&dw,NULL);
                    WriteConsole(handle_out," bytes\t",strlen(" bytes\t"),&dw,NULL);
                    WriteConsole(handle_out,fileData.cFileName,strlen(fileData.cFileName),&dw,NULL);
                    WriteConsole(handle_out,"\n",strlen("\n"),&dw,NULL);
                }
            }
            // file count and file total size
            char *fileCount = NumberToCharBuffer(file_count);
            char *fileTotalSize = NumberToCharBuffer(file_total_size);
            char *folderCount = NumberToCharBuffer(folder_count);
            char *folderTotalSize = NumberToCharBuffer(folder_total_size);

            // display file count
            WriteConsole(handle_out,"\t\tTotal files: ",strlen("\t\tTotal files: "),&dw,NULL);
            WriteConsole(handle_out,fileCount,strlen(fileCount),&dw,NULL);
            WriteConsole(handle_out,"\t\tTotal size: ",strlen("\t\tTotal size: "),&dw,NULL);
            WriteConsole(handle_out,fileTotalSize,strlen(fileTotalSize),&dw,NULL);
            WriteConsole(handle_out," bytes\n",strlen(" bytes\n"),&dw,NULL);
            // display folder count
            WriteConsole(handle_out,"\t\tTotal folders: ",strlen("\t\tTotal folders: "),&dw,NULL);
            WriteConsole(handle_out,folderCount,strlen(folderCount),&dw,NULL);
            WriteConsole(handle_out,"\tTotal size: ",strlen("\tTotal size: "),&dw,NULL);
            WriteConsole(handle_out,folderTotalSize,strlen(folderTotalSize),&dw,NULL);
            WriteConsole(handle_out," bytes\n",strlen(" bytes\n"),&dw,NULL);

        }
        else
        {
            WriteConsole(handle_out,"ls: List directory failed.\n",strlen("ls: List directory failed.\n"),&dw,NULL);
            WriteConsole(handle_out,"ls: No such directory.\n",strlen("ls: No such directory.\n"),&dw,NULL);
        }

        // close handle
        FindClose(firstFile);
    }
    else
    {
        WriteConsole(handle_out,"ls|dir: Command error.\n",strlen("ls|dir: Command error.\n"),&dw,NULL);
    }
}
// Cat file
void cat_type(char *argv[8], int *argc)
{
    if((*argc) == 2)
    {
        char *directoryPath = (char*)malloc(sizeof(char) * MAX_PATH);
        GetCurrentDirectory(MAX_PATH,directoryPath);
        strcat(directoryPath,"\\");
        strcat(directoryPath,argv[1]);

        char *catBuffer = (char*)malloc(sizeof(char) * MAX_CAT);;
        HANDLE catFile = CreateFile(directoryPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
        if (catFile == INVALID_HANDLE_VALUE)
        {
            catFile = NULL;
            WriteConsole(handle_out,"Error,Can not open file!\n",strlen("Error,Can not open file!\n"),&dw,NULL);
        }
        else
        {
            if( ReadFile(catFile, catBuffer, GetFileSize(catFile, NULL), &dw, NULL) )
            {
                WriteConsole(handle_out,catBuffer,strlen(catBuffer),&dw,NULL);
            }
            else
            {
                WriteConsole(handle_out,"Failed file too big!",strlen("Failed file too big!"),&dw,NULL);
            }

        }
        CloseHandle(catFile);
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"cat|type: Copy file.\n",strlen("cat|type: Copy file.\n"),&dw,NULL);
    }
    else
    {
        WriteConsole(handle_out,"cat|type: Command error.\n",strlen("cat|type: Command error.\n"),&dw,NULL);
    }
}

// Copy file
void cp_copy(char *argv[8], int *argc)
{
     if((*argc) == 3)
    {
        if(CopyFile(argv[1], argv[2], 1)) //(filename|directory, directory) | (filename, filename)
        {
            WriteConsole(handle_out,"Copy successful.\n",strlen("Copy successful.\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"Copy failed.\n",strlen("Copy failed.\n"),&dw,NULL);
        }
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"cp|copy: Copy file.\n",strlen("cp|copy: Copy file.\n"),&dw,NULL);
    }
    else
    {
        WriteConsole(handle_out,"cp|copy: Command error.\n",strlen("cp|copy: Command error.\n"),&dw,NULL);
    }
}

// delete files first
int deleteFiles(char *strDir)
{
    HANDLE firstFile;
    WIN32_FIND_DATA fileData;
    LARGE_INTEGER fileSize;
    // save path
    char *path = (char*)malloc(sizeof(char) * MAX_PATH);
    strcpy(path, strDir);
    strcat(path,"\\*.*");
    int flag = 1;
    //first folder(.)
    firstFile = FindFirstFile(path,&fileData);
    if(firstFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //directory
            {
                if(strcmp(fileData.cFileName, ".") ==0 || strcmp(fileData.cFileName, "..") ==0)
                {
                    continue;
                }
                char *sub_path = (char*)malloc(sizeof(char) * MAX_PATH);
                strcpy(sub_path, strDir);
                strcat(sub_path,"\\");
                strcat(sub_path, fileData.cFileName);
                deleteFiles(sub_path);
            }
            else // file
            {
                char *sub_path = (char*)malloc(sizeof(char) * MAX_PATH);
                strcpy(sub_path, strDir);
                strcat(sub_path,"\\");
                strcat(sub_path, fileData.cFileName);
                if(!DeleteFile(sub_path))
                {
                    flag = 0;
                }
            }
        }while(FindNextFile(firstFile, &fileData));
    }
    else
    {
        WriteConsole(handle_out,"Open folder files failed.\n",strlen("Open folder files failed.\n"),&dw,NULL);
        flag = 0;
    }
    FindClose(firstFile);
	return flag;
}

// delete folder last
int deleteFolders(char *strDir)
{
    HANDLE firstFile;
    WIN32_FIND_DATA fileData;
    LARGE_INTEGER fileSize;
    // save path
    char *path = (char*)malloc(sizeof(char) * MAX_PATH);
    strcpy(path, strDir);
    strcat(path,"\\*.*");
    int flag = 1;
    //first folder(.)
    firstFile = FindFirstFile(path,&fileData);
    if(firstFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //directory
            {
                if(strcmp(fileData.cFileName, ".") ==0 || strcmp(fileData.cFileName, "..") ==0)
                {
                    continue;
                }
                char *sub_path = (char*)malloc(sizeof(char) * MAX_PATH);
                strcpy(sub_path, strDir);
                strcat(sub_path,"\\");
                strcat(sub_path, fileData.cFileName);
                deleteFolders(sub_path);
                if(!RemoveDirectory(sub_path))
                {
                    flag = 0;
                }
            }
            else // file
            {
                char *sub_path = (char*)malloc(sizeof(char) * MAX_PATH);
                strcpy(sub_path, strDir);
                strcat(sub_path,"\\");
                strcat(sub_path, fileData.cFileName);
                if(!DeleteFile(sub_path))
                {
                    flag = 0;
                }
            }
        }while(FindNextFile(firstFile, &fileData));
    }
    else
    {
        WriteConsole(handle_out,"Open folders failed.\n",strlen("Open folders  failed.\n"),&dw,NULL);
        flag = 0;
    }
    FindClose(firstFile);
	return flag;
}

// Remove file or directory
void rm_del(char *argv[8], int *argc)
{
    if((*argc) == 2)
    {
        if(DeleteFile(argv[1])) //(filename|directory, directory) | (filename, filename)
        {
            WriteConsole(handle_out,"Remove successful.\n",strlen("Remove successful.\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"Remove failed.\n",strlen("Remove failed.\n"),&dw,NULL);
        }
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"rm|del: Remove file.\n",strlen("rm|del: Remove file.\n"),&dw,NULL);
    }
    else if((*argc) == 3 && (strcmp(argv[1], "-r") ==0))
    {
        char *path = (char*)malloc(sizeof(char) * MAX_PATH);
        GetCurrentDirectory(MAX_PATH,path);
		strcat(path,"\\");
        strcat(path, argv[2]);
        if(deleteFiles(path) && deleteFolders(path) && RemoveDirectory(argv[2]))
        {
            WriteConsole(handle_out,"rm|del: Delete folder success.\n",strlen("rm|del: Delete folder success.\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"rm|del: Delete folder failed.\n",strlen("rm|del: Delete folder failed.\n"),&dw,NULL);
        }
    }
    else
    {
        WriteConsole(handle_out,"rm|del: Command error.\n",strlen("rm|del: Command error.\n"),&dw,NULL);
    }
}

// Move file or directory
void mv_move(char *argv[8], int *argc)
{
    if((*argc) == 3)
    {
        if(MoveFile(argv[1],argv[2])) //(filename|directory, directory) | (filename, filename)
        {
            WriteConsole(handle_out,"Move successful.\n",strlen("Move successful.\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"Move failed.\n",strlen("Move failed.\n"),&dw,NULL);
        }
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"mv|move: Move file.\n",strlen("mv|move: Move file.\n"),&dw,NULL);
    }
    else
    {
        WriteConsole(handle_out,"mv|move: Command error.\n",strlen("mv|move: Command error.\n"),&dw,NULL);
    }
}

// Make directory
void mkdir(char *argv[8], int *argc)
{
    if((*argc) == 2)
    {
        if(CreateDirectory(argv[1],NULL)) //null is the safe argv
        {
            WriteConsole(handle_out,"Create directory successful.\n",strlen("Create directory successful.\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"Create directory failed.\n",strlen("Create directory failed.\n"),&dw,NULL);
        }
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"mkdir: Create directory.\n",strlen("mkdir: Create directory.\n"),&dw,NULL);
    }
    else
    {
        WriteConsole(handle_out,"mkdir: Command error.\n",strlen("mkdir: Command error.\n"),&dw,NULL);
    }
}

// Remove directory
void rmdir_rd(char *argv[8], int *argc)
{
    if((*argc) == 2)
    {
        if(RemoveDirectory(argv[1])) //null is the safe argv
        {
            WriteConsole(handle_out,"Remove directory successful.\n",strlen("Remove directory successful.\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"Remove directory failed.\n",strlen("Remove directory failed.\n"),&dw,NULL);
        }
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"rmdir|rd: Remove directory.\n",strlen("rmdir|rd: Remove directory.\n"),&dw,NULL);
    }
    else
    {
        WriteConsole(handle_out,"rmdir|rd: Command error.\n",strlen("rmdir|rd: Command error.\n"),&dw,NULL);
    }
}

// Change directory
void cd(char *argv[8], int *argc)
{
    if((*argc) == 2)
    {
        if(SetCurrentDirectory(argv[1]))
        {
            WriteConsole(handle_out,"Change directory successful.\n",strlen("Change directory successful.\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"Change directory failed.\n",strlen("Change directory failed.\n"),&dw,NULL);
        }
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"cd: Change directory.\n",strlen("cd: Change directory.\n"),&dw,NULL);
    }
    else
    {
        WriteConsole(handle_out,"cd: Command error.\n",strlen("cd: Command error.\n"),&dw,NULL);
    }
}

// Find directory or file
void find(char *argv[8], int *argc)
{
    if((*argc) == 2)
    {
        HANDLE firstFile;
        WIN32_FIND_DATA fileData;
        LARGE_INTEGER fileSize;
        // save time info
        FILETIME lastWriteTime;
		SYSTEMTIME systemTime;

		// get current directory for fingding
        char *directoryPath = (char*)malloc(sizeof(char) * MAX_PATH);
        char *directoryName = (char*)malloc(sizeof(char) * MAX_PATH);
        GetCurrentDirectory(MAX_PATH,directoryPath);
        GetCurrentDirectory(MAX_PATH,directoryName);
        strcat(directoryName,"\\");
		strcat(directoryName,argv[1]);

		// create the find file handle
        firstFile = FindFirstFile(directoryName,&fileData);

        // file count info
        long file_count = 0;
        long long file_total_size = 0;

        // folder count info
        long folder_count = 0;
        long long folder_total_size = GetFolderSize(directoryPath);

        // list the directory and file
        if(firstFile != INVALID_HANDLE_VALUE) //find it
        {
            //list all
            do
            {
                // time translation
                lastWriteTime = fileData.ftLastWriteTime;
                FileTimeToLocalFileTime(&lastWriteTime, &lastWriteTime);
                FileTimeToSystemTime(&lastWriteTime, &systemTime);
                // time to char
                char *year = (char*)malloc(sizeof(char) * 4);
                short _year = systemTime.wYear;
                short index = 0;
                while(_year > 0)
                {
                    year[3 - index] = '0' + _year%10;
                    index++;
                    _year = _year/10;
                }

                char *month = (char*)malloc(sizeof(char) * 2);
                short _month = systemTime.wMonth;
                index = 0;
                while(_month > 0)
                {
                    if(_month < 10)
                    {
                        month[0] = '0';
                    }
                    month[1 - index] = '0' + _month%10;
                    index++;
                    _month = _month/10;
                }

                char *day = (char*)malloc(sizeof(char) * 2);
                short _day = systemTime.wDay;
                index = 0;
                while(_day > 0)
                {
                    if(_day < 10)
                    {
                        day[0] = '0';
                    }
                    day[1 - index] = '0' + _day%10;
                    index++;
                    _day = _day/10;
                }

                char *hour = (char*)malloc(sizeof(char) * 2);
                short _hour = systemTime.wHour;
                index = 0;
                if(_hour == 0)
                {
                    hour[0] = '0';
                    hour[1] = '0';
                }
                while(_hour > 0)
                {
                    if(_hour < 10)
                    {
                        hour[0] = '0';
                    }
                    hour[1 - index] = '0' + _hour%10;
                    index++;
                    _hour = _hour/10;
                }

                char *minute = (char*)malloc(sizeof(char) * 2);
                short _minute = systemTime.wMinute;
                index = 0;
                if(_minute == 0)
                {
                    minute[0] = '0';
                    minute[1] = '0';
                }
                while(_minute > 0)
                {
                    if(_minute < 10)
                    {
                        minute[0] = '0';
                    }
                    minute[1 - index] = '0' + _minute%10;
                    index++;
                    _minute = _minute/10;
                }

                //file size
                fileSize.LowPart = fileData.nFileSizeLow;
                fileSize.HighPart = fileData.nFileSizeHigh;
                long long file_size = fileSize.QuadPart;
                char *singleFileSize = NumberToCharBuffer(file_size);

                if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //directory
                {
                    folder_count++;

                    //display year/month/day
                    WriteConsole(handle_out,year,4,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,month,2,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,day,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display hours:minute
                    WriteConsole(handle_out,hour,2,&dw,NULL);
                    WriteConsole(handle_out,":",strlen(":"),&dw,NULL);
                    WriteConsole(handle_out,minute,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display fileName
                    WriteConsole(handle_out,"<DIR>  \t\t",strlen("<DIR>  \t\t"),&dw,NULL);
                    WriteConsole(handle_out,fileData.cFileName,strlen(fileData.cFileName),&dw,NULL);
                    WriteConsole(handle_out,"\n",strlen("\n"),&dw,NULL);

                }
                else // file
                {
                    file_count++; //the number of file
                    file_total_size += fileSize.QuadPart;

                    //display year/month/day
                    WriteConsole(handle_out,year,4,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,month,2,&dw,NULL);
                    WriteConsole(handle_out,"/",strlen("/"),&dw,NULL);
                    WriteConsole(handle_out,day,2,&dw,NULL);
                    WriteConsole(handle_out,"  ",strlen("  "),&dw,NULL);

                    //display hours:minute
                    WriteConsole(handle_out,hour,2,&dw,NULL);
                    WriteConsole(handle_out,":",strlen(":"),&dw,NULL);
                    WriteConsole(handle_out,minute,2,&dw,NULL);
                    WriteConsole(handle_out,"\t",strlen("\t"),&dw,NULL);

                    //display fileName size
                    WriteConsole(handle_out,singleFileSize,strlen(singleFileSize),&dw,NULL);
                    WriteConsole(handle_out," bytes\t",strlen(" bytes\t"),&dw,NULL);
                    WriteConsole(handle_out,fileData.cFileName,strlen(fileData.cFileName),&dw,NULL);
                    WriteConsole(handle_out,"\n",strlen("\n"),&dw,NULL);
                }
            }while(FindNextFile(firstFile, &fileData));
            // file count and file total size
            char *fileCount = NumberToCharBuffer(file_count);
            char *fileTotalSize = NumberToCharBuffer(file_total_size);
            char *folderCount = NumberToCharBuffer(folder_count);
            char *folderTotalSize = NumberToCharBuffer(folder_total_size);

            // display file count
            WriteConsole(handle_out,"\t\tTotal files: ",strlen("\t\tTotal files: "),&dw,NULL);
            WriteConsole(handle_out,fileCount,strlen(fileCount),&dw,NULL);
            WriteConsole(handle_out,"\t\tTotal size: ",strlen("\t\tTotal size: "),&dw,NULL);
            WriteConsole(handle_out,fileTotalSize,strlen(fileTotalSize),&dw,NULL);
            WriteConsole(handle_out," bytes\n",strlen(" bytes\n"),&dw,NULL);
            // display folder count
            WriteConsole(handle_out,"\t\tTotal folders: ",strlen("\t\tTotal folders: "),&dw,NULL);
            WriteConsole(handle_out,folderCount,strlen(folderCount),&dw,NULL);
            WriteConsole(handle_out,"\tTotal size: ",strlen("\tTotal size: "),&dw,NULL);
            WriteConsole(handle_out,folderTotalSize,strlen(folderTotalSize),&dw,NULL);
            WriteConsole(handle_out," bytes\n",strlen(" bytes\n"),&dw,NULL);
        }
        else
        {
            WriteConsole(handle_out,"find: No such directory or file.\n",strlen("find: No such directory or file.\n"),&dw,NULL);
        }
        // close handle
        FindClose(firstFile);
    }
    else if((*argc) == 1)
    {
        WriteConsole(handle_out,"find: Find directory or file.\n",strlen("find: Find directory or file.\n"),&dw,NULL);
    }
    else
    {
        WriteConsole(handle_out,"find: Command error.\n",strlen("find: Command error.\n"),&dw,NULL);
    }
}



#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include "gcc_compiler_fun.h"
using namespace std;
int GCC_compiler_fun::compileFile()                        // 编译成功 返回1 编译失败 返回0
{
    char cmdline[2 * PATH_LEN] = { '\0' };
    sprintf(cmdline, "\"%sGCC\\bin\\gcc.exe\" %s -o %s%s 2> %s%s",
        this->app_path, this->file_path, this->dir_path, this->file_name, this->dir_path, this->errorfile);
    printf_s("01   %s\n %d\n",cmdline,sizeof(cmdline));
    system(cmdline);
    char resfilepath[PATH_LEN] = { '\0' };
    QString a = this->dir_path;
    QString b = cmdline;
    qDebug()<<b<<"this->dir_path"<<a;
    sprintf(resfilepath, "%s%s", this->dir_path, this->errorfile);
    printf_s("02%s\n",resfilepath);
    ifstream resfile(resfilepath, ios::in);
    if (!resfile) {
        printf_s("ERROR:Open compileRes file failed!\n");
        exit(-1);
    }
    char temp;
    while (resfile.get(temp)) {
        error_info.append(1, temp);
    }
    resfile.close();
    fstream renewResFile(resfilepath, fstream::out | ios_base::trunc);
    renewResFile.close();
    return 1;
}


void GCC_compiler_fun::changeFilePath(char filePath[])     // 更改文件路径
{
    memset(this->file_path, '\0', PATH_LEN);
    memset(this->file_name, '\0', PATH_LEN);
    memset(this->dir_path, '\0', PATH_LEN);
    strcpy_s(file_path, filePath);
    this->separatePath();
}

void GCC_compiler_fun::createErrorInfo()                   // 未完善前测试返回信息用
{
    this->error_info = "just for test\n";
}

string GCC_compiler_fun::getErrorInfo()                    // 编译失败的话提供错误信息
{
    return this->error_info;
}

void GCC_compiler_fun::getAppFilePath()                     // 获取程序所在路径
{
    int i, len;
    wchar_t app_patht[PATH_LEN];
    wmemset(app_patht, '\0', PATH_LEN);
    GetModuleFileName(NULL, app_patht, PATH_LEN);
    len = wcslen(app_patht);
    for (i = len - 1; i >= 0; i--) {
        if (app_patht[i] == '\\') {
            break;
        }
        app_patht[i] = '\0';
    }
    WideCharToMultiByte(CP_ACP, 0, app_patht, wcslen(app_patht), app_path, PATH_LEN, NULL, NULL);
}

void GCC_compiler_fun::separatePath()                       // 分离路径的文件夹路径和文件名
{
    int i, len, ns, nd, j;
    ns = nd = 0;
    len = i = strlen(this->file_path);
    for (i = len - 1; i >= 0; i--) {
        if (this->file_path[i] == '.') {
            nd = i - 1;
            break;
        }
    }
    for (i = i - 1; i >= 0; i--) {             // 获取文件名
        if (this->file_path[i] == '\\') {
            ns = i + 1;
            break;
        }
    }
    for (; i >= 0; i--) {                      // 获取文件路径(\\结尾)
        this->dir_path[i] = this->file_path[i];
    }
    j = 0;
    for (i = ns; i <= nd; i++) {
        this->file_name[j] = this->file_path[i];
        j++;
    }
}

void GCC_compiler_fun::getDirPath(string& dirpath)  // 获取文件夹路径(\\结尾)
{
    dirpath.append(this->dir_path);
}

void GCC_compiler_fun::getFileName(string& filename)       // 获取文件名(不含后缀名)
{
    filename.append(this->file_name);
}

GCC_compiler_fun::GCC_compiler_fun()                       // 类默认构造函数
{
    memset(this->file_path, '\0', PATH_LEN);
    memset(this->file_name, '\0', PATH_LEN);
    memset(this->dir_path, '\0', PATH_LEN);
    memset(this->errorfile, '\0', 15);
    memset(this->app_path, '\0', PATH_LEN);
    strcpy_s(errorfile, "compileRes.txt");
    this->getAppFilePath();
}

GCC_compiler_fun::GCC_compiler_fun(char filePath[])        // 根据filePath构造类
{
    strcpy_s(this->file_path, filePath);                   // 用filepath给file_path赋值
    memset(this->dir_path, '\0', PATH_LEN);
    memset(this->file_name, '\0', PATH_LEN);
    memset(this->errorfile, '\0', 15);
    memset(this->app_path, '\0', PATH_LEN);
    strcpy_s(errorfile, "compileRes.txt");
    this->separatePath();
    this->getAppFilePath();
}
// 判断是否编译通过的函数定义，放到类源文件中
int GCC_compiler_fun::judgeError()           // 判断有没有编译通过，通过判断编译结果是否有error
{
    const char *ress =  strstr(error_info.c_str(), (char *)"error");
    if (ress == NULL)
        return 1;
    else
        return 0;
}

// 执行编译结果函数定义，放到类源文件中
 void GCC_compiler_fun::executeRes()         // 执行编译结果
{
    char execute_cmd[PATH_LEN] = { '\0' };
    sprintf(execute_cmd, "%s%s.exe", this->dir_path, this->file_name);
    system(execute_cmd);
}
GCC_compiler_fun::~GCC_compiler_fun()                      // 默认析构函数
{
}

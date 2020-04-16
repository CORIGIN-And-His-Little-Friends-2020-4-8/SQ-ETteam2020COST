#ifndef GCC_COMPILER_FUN_H
#define GCC_COMPILER_FUN_H

#pragma once
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <QString>
#include <QProcess>
#include <QDebug>

#define PATH_LEN 1024                   // 文件路径的最大长度
#define ERROR_INFO_LEN 2048             // 错误信息文本的最大长度

using namespace std;

class GCC_compiler_fun
{
private :
    char errorfile[15];
    char file_path[PATH_LEN];           // 文件路径
    string error_info;                  // 错误信息文本
    char dir_path[PATH_LEN];            // 源文件文件夹所在路径
    char file_name[PATH_LEN];           // 文件名称
    char app_path[PATH_LEN];            // 程序所在的文件夹
public :
    int compileFile();                  // 根据路径编译文件
    void changeFilePath(char file_path[]);       // 更改编译源文件的路径
    string getErrorInfo();                      // 返回错误信息
    void createErrorInfo();                      // 添加错误信息(测试用)
    void getFileName(string& filename);          // 获取文件名(后缀除外)
    void getDirPath(string& dirpath);            // 获取文件所在文件夹路径(\\结尾)
    int judgeError();                            // 判断有没有编译通过
    void executeRes();                           // 执行编译结果
private:
    void separatePath();                         // 读取源文件所在文件夹的路径
    void getAppFilePath();                       // 获取程序所在的路径
public:
    GCC_compiler_fun(char filePath[]);           // 根据需编译的源文件路径创建类
    GCC_compiler_fun();                          // 默认构造函数
    ~GCC_compiler_fun();
};


#endif // GCC_COMPILER_FUN_H

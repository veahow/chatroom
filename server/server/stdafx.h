// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件

#include <WinSock2.h>    // Windows Socket编程必备头文件
#include <cstring>
#include <thread>    // 线程引入出了问题 注意命名空间std 注释掉该行不会阻塞
#include "mysql.h"
#include "DataBase.h"
#include "User.h"

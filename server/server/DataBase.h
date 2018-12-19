#pragma once
#ifndef __DATABASE__
#define __DATABASE__

#include "stdafx.h"

class User;

class DataBase {
public:
	DataBase(const char *user,
		const char *pwd,
		const char *host,
		const char *database,
		const unsigned int port)
		:user(user), pwd(pwd), host(host), database(database), port(port),
		pMySqlConn(NULL) {}
	~DataBase() {}

	bool Connect();    // 连接MySQL 成功:true 失败:false
	bool Login(const User&);    // 匹配MySQL数据库test中的user表 登录成功:true 登录失败:false
	bool Register(const User&);    // 插入MySQL数据库test中user表新的用户信息 成功:true 失败:false
	bool Update(const User&);    // 更新MySQL数据库test中user表的用户信息 成功:true 失败:false
private:
	const char *user;
	const char *pwd;
	const char *host;
	const char *database;
	const unsigned int port;

	MYSQL* pMySqlConn;
};

#endif // !__DATABASE__
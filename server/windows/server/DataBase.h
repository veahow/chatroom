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

	bool Connect();    // ����MySQL �ɹ�:true ʧ��:false
	bool Login(const User&);    // ƥ��MySQL���ݿ�test�е�user�� ��¼�ɹ�:true ��¼ʧ��:false
	bool Register(const User&);    // ����MySQL���ݿ�test��user���µ��û���Ϣ �ɹ�:true ʧ��:false
	bool Update(const User&);    // ����MySQL���ݿ�test��user����û���Ϣ �ɹ�:true ʧ��:false
private:
	const char *user;
	const char *pwd;
	const char *host;
	const char *database;
	const unsigned int port;

	MYSQL* pMySqlConn;
};

#endif // !__DATABASE__
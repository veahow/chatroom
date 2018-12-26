#include "stdafx.h"

bool DataBase::Connect()
{
	try {
		pMySqlConn = mysql_init(NULL);

		// C++ ��ʼ��mysqlָ��
		if (pMySqlConn == NULL) {
			std::cout << "MySQL Init Failed!" << std::endl;

			return false;
		}
		else {
			std::cout << "MySQL Init Sucessful!" << std::endl;
		}

		// C++����MySQL C-API:mysql_real_connect()
		if (mysql_real_connect(pMySqlConn, host, user, pwd, database, port, NULL, 0))
		{
			std::cout << "MySQL Connect Successful!" << std::endl;

			return true;
		}
		else {
			std::cout << "MySQL Connect Error: " << mysql_error(pMySqlConn) << std::endl;

			return false;
		}
	}
	catch (...) {
		std::cout << "MySQL Connect Exception!" << std::endl;

		return false;

	}
}

bool DataBase::Login(const User& usr)
{
	//string str =
	//	"select username, password from user where username=\'" 
	//	+ usr.getUsername() 
	//	+ "\' and password=\'" 
	//	+ usr.getPassword() + "\'";
	//cout << str.c_str() << endl;

	//char str[100];
	//strcpy(str, "select username, password from user where username=\'");
	//strcat(str, usr.getUsername());
	//strcat(str, "\' and password=\'");
	//strcat(str, usr.getPassword());
	//strcat(str, "\'");

	//cout << str << endl;

	if (mysql_query(pMySqlConn, "select * from user")) {
		std::cout << "Query Error!" << std::endl;
		return false;
	}
	else {
		std::cout << "Query Successful" << std::endl;

		MYSQL_RES *result;
		result = mysql_store_result(pMySqlConn);
		if (result)
		{
			MYSQL_ROW row;

			while (row = mysql_fetch_row(result))    //��ȡ��������� row[0]:id row[1]:username row[2]:password row[3]:type
			{
				if (!strcmp(usr.getUsername(), row[1]))    // �û���ƥ��ɹ�
				{
					//cout << "row[1]" << endl;
					if (!strcmp(usr.getPassword(), row[2])) {    // ����ƥ��ɹ�
						std::cout << "y" << std::endl;
						return true;
					}
					else continue;

				}
				//else cout << "n" << endl;

			}
		}

		mysql_free_result(result);    // �ͷŽ����

		return false;
	}
}

bool DataBase::Register(const User& usr)
{
	// û�������ݿ⺯���û����ж� ���ܻ����������û��� �����ٸĽ�
	char str[100];
	snprintf(str, 100,
		"insert into user values(null, '%s', '%s', '%s');",
		usr.getUsername(), usr.getPassword(), usr.getType());
	if (mysql_real_query(pMySqlConn, str, strlen(str))) {
		printf("Register Error!\n");
		return false;
	}
	else {
		printf("Register Successful!\n");
		return true;
	}

}

bool DataBase::Update(const User& usr)
{
	return true;
}
#pragma once
#ifndef __USER__
#define __USER__

class User {
public:
	User(const char* username,
		const char* password,
		const char* type = "user")
		:username(username), password(password), type(type) {}
	~User() {}

	const char* getUsername() const { return username; }
	const char* getPassword() const { return password; }
	const char* getType() const { return type; }
private:
	const char* username;    // 用户名显示
	const char* password;
	const char* type;
};

#endif // !__USER__


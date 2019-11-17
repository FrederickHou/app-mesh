#ifndef APMANAGER_USER_H
#define APMANAGER_USER_H
#include <string>
#include <set>
#include <map>
#include <cpprest/json.h>
#include "Role.h"

class User
{
public:
	explicit User(std::string name);
	virtual ~User();

	// seriarize
	virtual web::json::value AsJson();
	static std::shared_ptr<User> FromJson(std::string userName, const web::json::object& obj, std::shared_ptr<Roles> roles);

	// user update
	void lock();
	void unlock();
	void updateRoles(std::set<std::shared_ptr<Role>> roles);

	// get user info
	bool locked() const;
	const std::string& getKey() const;
	const std::set<std::shared_ptr<Role>>& getRoles();
	bool hasPermission(std::string permission) const;

private:
	std::string m_key;
	bool m_locked;
	std::string m_name;
	std::set<std::shared_ptr<Role>> m_roles;
};

class Users
{
public:
	Users();
	virtual ~Users();

	virtual web::json::value AsJson();
	static std::shared_ptr<Users> FromJson(const web::json::object& obj, std::shared_ptr<Roles> roles);

	// find user
	std::map<std::string, std::shared_ptr<User>> getUsers();
	std::shared_ptr<User> getUser(std::string name);

	// manage users
	void addUser(const web::json::object& obj, std::shared_ptr<Roles> roles);
	void delUser(std::string name);
private:
	std::map<std::string, std::shared_ptr<User>> m_users;
};

#endif
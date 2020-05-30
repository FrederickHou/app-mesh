#pragma once

#include <string>
#include <set>
#include <map>
#include <mutex>
#include <cpprest/json.h>
#include "Role.h"

//////////////////////////////////////////////////////////////////////////
/// User
//////////////////////////////////////////////////////////////////////////
class User
{
public:
	explicit User(std::string name);
	virtual ~User();

	// seriarize
	web::json::value AsJson() const;
	static std::shared_ptr<User> FromJson(std::string userName, const web::json::value& obj, const std::shared_ptr<Roles> roles) noexcept(false);

	// user update
	void lock();
	void unlock();
	void updateUser(std::shared_ptr<User> user);
	void updateKey(std::string passswd);

	// get user info
	bool locked() const;
	const std::string getKey();
	const std::string& getExecUser() const { std::lock_guard<std::recursive_mutex> guard(m_mutex); return m_execUser; }
	const std::string& getGroup() const { std::lock_guard<std::recursive_mutex> guard(m_mutex); return m_group; }
	const std::string& getName() const { std::lock_guard<std::recursive_mutex> guard(m_mutex); return m_name; }
	const std::set<std::shared_ptr<Role>> getRoles();
	bool hasPermission(std::string permission);

private:
	std::string m_key;
	bool m_locked;
	std::string m_name;
	std::string m_group;
	std::string m_metadata;
	std::string m_execUser;
	mutable std::recursive_mutex m_mutex;
	std::set<std::shared_ptr<Role>> m_roles;
};

class Users
{
public:
	Users();
	virtual ~Users();

	web::json::value AsJson() const;
	static std::shared_ptr<Users> FromJson(const web::json::value& obj, std::shared_ptr<Roles> roles) noexcept(false);

	// find user
	std::map<std::string, std::shared_ptr<User>> getUsers();
	std::shared_ptr<User> getUser(std::string name) const;
	std::set<std::string> getGroups() const;

	// manage users
	void addUsers(const web::json::value& obj, std::shared_ptr<Roles> roles);
	std::shared_ptr<User> addUser(const std::string userName, const web::json::value& userJson, std::shared_ptr<Roles> roles);
	void delUser(std::string name);
private:
	std::map<std::string, std::shared_ptr<User>> m_users;
	mutable std::recursive_mutex m_mutex;
};

#ifndef REST_HANDLER_H
#define REST_HANDLER_H

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h> // HTTP server 

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

//////////////////////////////////////////////////////////////////////////
// REST service
//////////////////////////////////////////////////////////////////////////
class RestHandler
{
public:
	explicit RestHandler(std::string ipaddress, int port);
	virtual ~RestHandler();

protected:
	void open();
	void close();

private:
	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_error(pplx::task<void>& t);
	bool verifyAdminToken(const http_request& message, const std::string& token);
	bool verifyUserToken(const http_request& message, const std::string& token);
	bool verifyToken(const http_request& message, const std::string& token, const std::string& user, const std::string& key);
	std::string getToken(const http_request& message);
	std::string createToken(const std::string uname, const std::string passwd);

	void  registerShellApp(const http_request& message);

private:
	std::shared_ptr<http_listener> m_listener;

};
#endif

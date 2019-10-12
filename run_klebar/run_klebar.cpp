#include <cstdio>
#include <thread>
#include "path.h"
#include "canbus.h"
#include "../cpp-httplib/httplib.h"


CCanbus canbus;

std::string execute_action(std::string action, std::string param)
{
    std::string ret;
	printf("execute action: {%s} param: {%s}\n", action.c_str(), param.c_str());
	if( action == "halt")
	{	ret = "Action \"halt\" executed";
	}
	else
	{	ret = "Unknown action : " + action;	
	}
    return ret;
}

int main(void)
{   std::string ret;

    printf("Klebar server started\n\n");
    class CPath path(2,2);
    path.set_position(0, 100);
    path.set_position(1, 200);   
    path.start(10);


    httplib::Server server;

    server.Get("/halt", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content("ok, j'ai fini", "text/plain");
        path.stop();
        server.stop();
    });

    server.Get("/do", [&](const httplib::Request& req, httplib::Response& res) {
        //res.set_content("ok, post", "text/plain");
        //printf("post received body= {%s} name = {%s}\n", req.body.c_str(), req.get_param_value("name").c_str());
        ret = execute_action(req.get_param_value("action"), req.get_param_value("param"));
        res.set_content(ret, "text/plain");
    });

    server.Get("/can", [&](const httplib::Request& req, httplib::Response& res) {
        //res.set_content("ok, post", "text/plain");
        //printf("post received body= {%s} name = {%s}\n", req.body.c_str(), req.get_param_value("name").c_str());
        std::string node        = req.get_param_value("node");
        std::string command     = req.get_param_value("command");
        std::string param       = req.get_param_value("param");
        printf("can : node=%s   command=%s    param=%s\n", node.c_str(), command.c_str(), param.c_str());
        ret = canbus.execute_command_can(node, command, param);
        res.set_content(ret, "text/plain");
    });

    server.Post("/do", [&](const httplib::Request& req, httplib::Response& res) {
        ret = execute_action(req.get_param_value("action"), req.get_param_value("param"));
        res.set_content(ret, "text/plain");
    });

    server.listen("192.168.0.25", 8080);

    printf("Bye!\n\n");
}  
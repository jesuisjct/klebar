#include <stdio.h>
#include <string.h>
#include <cstdio>

#include <fcntl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <string>

class CCanbus
{   
public:
    CCanbus();
    int open_canbus(std::string port);
    int write_canbus(int id, int len, char data[8]);
    int read_canbus(int& id, int& len, int data[8]);
    void close_canbus();

    std::string execute_command_can(std::string node, std::string command, std::string param);
    std::string execute_can(std::string req);  
  private:
    bool                is_connected;  
    struct can_frame    frame;
    int                 socket_can;


};
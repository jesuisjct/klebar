#include "canbus.h"

int CCanbus::open_canbus(std::string port)
{
    struct ifreq        ifr;
    struct sockaddr_can addr;

    socket_can = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(socket_can < 0)        return (-1);

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port.c_str());

    if (ioctl(socket_can, SIOCGIFINDEX, &ifr) < 0) return (-1);

    addr.can_ifindex = ifr.ifr_ifindex;
    fcntl(socket_can, F_SETFL, O_NONBLOCK);

    if (bind(socket_can, (struct sockaddr *)&addr, sizeof(addr)) < 0)  return (-1);
    
    return 0;
}

int CCanbus::write_canbus(int id, int len, char data[8])
{
    frame.can_id = id;
    frame.can_dlc = len;
    mempcpy(&frame.data, data, 8);

    int ret = write(socket_can, &frame, sizeof(struct can_frame));
    if (ret != sizeof(struct can_frame))    return (-1);
    else                                    return (0);
}

int CCanbus::read_canbus(int& id, int& len, int data[8])
{
    int ret = read(socket_can, &frame, sizeof(struct can_frame));
    if(ret)
    {   id = frame.can_id;
        len = frame.can_dlc;
        memcpy(data, frame.data, len);
    }
    return ret;
}

void CCanbus::close_canbus()
{
    close(socket_can);
}

std::string CCanbus::execute_command_can(std::string node, std::string command, std::string param)
{
    //std::cout << "node" << node;
    printf("execute can command id=%s command=%s\n", node.c_str(), command.c_str());
    return "can bus comman executed";
}
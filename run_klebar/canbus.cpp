#include "canbus.h"

CCanbus::CCanbus()
{
    socket_can = 0;
    is_connected = false;
}

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
    is_connected = true;
    printf("canbus opened\n");
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
    if(!is_connected)   open_canbus("can0");
    printf("execute canbus command node=%s command=%s\n", node.c_str(), command.c_str());
    frame.can_id = (std::stoi(node) << 5) + std::stoi(command);
    printf("can_id=%d\n",frame.can_id);
    frame.can_dlc = 8;
    int signal = std::stoi(param);
    frame.data[0] = (uint32_t) signal;
	frame.data[1] = (uint32_t) signal >> 8;
	frame.data[2] = (uint32_t) signal >> 16;
	frame.data[3] = (uint32_t) signal >> 24;
	frame.data[4] = (uint32_t) 0;
	frame.data[5] = (uint32_t) 0;
	frame.data[6] = (uint32_t) 0;
	frame.data[7] = (uint32_t) 0;
    int ret = write(socket_can, &frame, sizeof(struct can_frame));
    printf("canbus return %d\n", ret);
    if (ret != sizeof(struct can_frame))    return "erreur"; //(std::string("Error executing CAN command ") + command);
    else                                    return "ok"; //(std::string("CAN command ") + command + " executed");
}

std::string CCanbus::execute_can(const httplib::Request& req)
{   int signal = 0;
    //std::cout << "node" << node;
    std::string node    = req.get_param_value("node");
    std::string command = req.get_param_value("command");
    std::string rtr     = req.get_param_value("rtr");
    std::string param   = req.get_param_value("param");

    if(!is_connected)   open_canbus("can0");
    printf("execute canbus command node=%s command=%s\n", node.c_str(), command.c_str());
    frame.can_id = (std::stoi(node) << 5) + std::stoi(command);
    if(rtr != "") frame.can_id &= CAN_RTR_FLAG;
    printf("can_id=%d\n",frame.can_id);
    frame.can_dlc = 8;
    //if(param != "") signal = std::stoi(param);
    //else            signal = 0;
    frame.data[0] = (uint32_t) signal;
	frame.data[1] = (uint32_t) signal >> 8;
	frame.data[2] = (uint32_t) signal >> 16;
	frame.data[3] = (uint32_t) signal >> 24;
	frame.data[4] = (uint32_t) 0;
	frame.data[5] = (uint32_t) 0;
	frame.data[6] = (uint32_t) 0;
	frame.data[7] = (uint32_t) 0;
    int retw = write(socket_can, &frame, sizeof(struct can_frame));
    if(rtr != "")
    {   printf("rtr\n");
        int retr = read(socket_can, &frame, sizeof(struct can_frame));
        if(retr == sizeof(struct can_frame))
        {   int id = frame.can_id;
            int len = frame.can_dlc;
            memcpy((void *)&signal, frame.data, len);
            printf("signal returned %d %d\n", signal, retr);
        }

    }
    printf("canbus return %d\n", retr);
    if (retw != sizeof(struct can_frame))   return "erreur"; //(std::string("Error executing CAN command ") + command);
    else                                    return "ok"; //(std::string("CAN command ") + command + " executed");
}
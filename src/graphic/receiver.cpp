#include "../../includes/graphic/receiver.hpp"

bool graphic::Receiver::startReceiver(std::string receiver_path)
{
    int result = -1;
#ifdef __APPLE__
    std::string cmd = "open \'" + bashEncode(receiver_path) + "\'";
    result = system(cmd.c_str());
#elif __linux__
    std::string cmd = "gnome-open \'" + bashEncode(receiver_path) + "\'";
    result = system(cmd.c_str());
#elif _WIN32
    std::string cmd = "start \'" + bashEncode(receiver_path) + "\'";
    result = system(cmd.c_str());
#endif

    return result == 0;
}
#include<iostream>
#include <opencv2/opencv.hpp>
#include <fcntl.h>  
#include <sys/stat.h>
#include <mqueue.h>
#include <string>

int main() {
    struct mq_attr attr;
    attr.mq_maxmsg = 16;
    attr.mq_msgsize = 16;
    attr.mq_flags = 0;
    mqd_t BC_queue = mq_open("/BC_queue", O_RDONLY | O_CREAT, 0664, &attr);
    // std::cout << EACCES << std::endl;
    // std::cout << EEXIST << std::endl;
    // std::cout << EINVAL << std::endl;
    // std::cout << EMFILE << std::endl;
    // std::cout << ENAMETOOLONG << std::endl;
    // std::cout << ENFILE << std::endl;
    // std::cout << ENOENT << std::endl;
    // std::cout << ENOMEM << std::endl;
    // std::cout << ENOSPC << std::endl;
    std::cout << BC_queue << std::endl;
    std::cout << ((errno==EINVAL)? "EINVAL" : "something else") << std::endl;
    std::cout << errno << std::endl;
    char msg_ptr[10];
    unsigned int priority = 1;
    int bytes;
    while(mq_receive(BC_queue, msg_ptr, 16, &priority) != -1) {
        std::string message = msg_ptr;
        std::cout << bytes << " " << message << std::endl;
    }
    std::cout << errno << std::endl;
    std::cout << ((errno==EAGAIN)? "EAGAIN" : " ") << std::endl;
    std::cout << ((errno==EBADF)? "EBADF" : " ") << std::endl;
    std::cout << ((errno==EINTR)? "EINTR" : " ") << std::endl;
    std::cout << ((errno==EINVAL)? "EINVAL" : " ") << std::endl;
    std::cout << ((errno==EMSGSIZE)? "EMSGSIZE" : " ") << std::endl;
    std::cout << ((errno==ETIMEDOUT)? "ETIMEDOUT" : " ") << std::endl;
    mq_close(BC_queue);
}
#include <zmq.h>
#include <unistd.h>
#include <string.h>
#include <stack>
#include <ctime>
#include <iostream>
#include <vector>

const int CREATE = 1;
const int EXEC = 0;
const int REMOVE = -1;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        std::cerr << "Not enough parameters" << std::endl;
        exit(-1);
    }
    int act;
    int id = atoi(argv[1]);
    std::string port = "tcp://*:" + std::to_string(id);
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    act = zmq_bind(responder, port.c_str());
    if (act == -1) {
        return -1;
    }
    std::stack <long long> stack;
    while (true) 
    {
        
        zmq_msg_t msg;
        act = zmq_msg_init(&msg);
        if (act == -1) {
            return -1;
        }

        act = zmq_msg_recv(&msg, responder, 0);
        if (act == -1) {
            return -1;
        }

        int* data = (int*)zmq_msg_data(&msg);
        int t = *data;
        
        switch (t)
        {
            case CREATE:
            {
                int n = *(++data);
                std::string id_str = std::to_string(n);
                int pid = fork();
                if (pid == -1){
                    return -1;
                }
                if (pid == 0){
                    execl("server", "server", id_str.c_str(), NULL);
                } else {
                    act = zmq_send(responder, &pid, sizeof(int), 0);
                    if (act == -1) {
                        return -1;
                    }
                }
                break;
            }

            case EXEC:
            {
                int commandNumber = *(++data);
                long long timer =  0;
                if (commandNumber == 0){
                    long long start = std::time(nullptr);
                    stack.push(start);
                    timer = -1;
                }
                if ((commandNumber == 1) && !stack.empty()){
                    long long end = std::time(nullptr);
                    timer = (end - stack.top()) * 1000;              
                }
                if (commandNumber == 2){
                    stack.pop();
                    timer = -1;
                }
                act = zmq_send(responder, &timer, sizeof(long long), 0);
                if (act == -1) {
                    return -1;
                }
                break;
            }

            case REMOVE:
            {
                zmq_send(responder, &id, sizeof(int), 0);
                zmq_close(responder);
                zmq_ctx_destroy(context);
                return 0;
            }
        }
        act = zmq_msg_close(&msg);
        if (act == -1) {
            return -1;
        }
    }
}
#ifndef NODE_H
#define NODE_H

#include "zmq.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#include <iostream>
#include <queue>
#include <vector>

std::string protocol = "tcp://localhost:";
const int MIN_PORT = 1024;
void* async_node_thd(void*);

const int CREATE = 1;
const int EXEC = 0;
const int REMOVE = -1;

struct async_node
{
    int id, act;
    std::string port;
    bool active;
    async_node* L;
    async_node* R;
    pthread_mutex_t mutex;
    pthread_t thd;
    std::queue <std::vector <int>> q;

    async_node(int i)
    {
        id = i;
        port = protocol + std::to_string(i);
        active = true;
        L = nullptr;
        R = nullptr;
        act = pthread_mutex_init(&mutex, NULL);
        if (act != 0){
            std::cout << "Error:" << id - MIN_PORT << ": Gateway mutex error\n";
            return;
        }

        act = pthread_create(&thd, NULL, async_node_thd, this);
        if (act != 0){
            std::cout << "Error:" << id - MIN_PORT << ": Gateway thread error\n";
            return;
        }

        act = pthread_detach(thd);
        if(act != 0){
            std::cout << "Error:" << id << ": Gateway thread error\n";
            return;
        }
    }

    void make_query(std::vector <int> v)
    {
        act = pthread_mutex_lock(&mutex);
        if (act != 0){
            std::cout << "Error:" << id - MIN_PORT << ": Gateway mutex lock error\n";
            active = false; 
            return;
        }

        q.push(v);
        act = pthread_mutex_unlock(&mutex);
        if (act != 0){
            std::cout << "Error:" << id - MIN_PORT << ": Gateway mutex unlock error\n";
            active = false;
        }
        
    }

    ~async_node()
    {
        pthread_mutex_destroy(&mutex);
    }
};


void* async_node_thd(void* ptr)
{
    int act;
    async_node* node = (async_node*)ptr;
    void* context = zmq_ctx_new();
    void *req = zmq_socket(context, ZMQ_REQ);
    act = zmq_connect(req, node->port.c_str());
    if (act == -1){
        std::cout << "Error: Connection with" << node->id - MIN_PORT << "\n";
    }
    while (node->active || !node->q.empty())
    {
        if (node->q.empty()) {
            continue;
        }

        act = pthread_mutex_lock(&node->mutex);
        if (act != 0) {
            std::cout << "Error:" << node->id - MIN_PORT << ": Gateway mutex lock error\n", 
            node->active = false;
            break;
        }

        std::vector <int> vectData = node->q.front();
        node->q.pop();
        act = pthread_mutex_unlock(&node->mutex);
        if (act != 0){
            std::cout << "Error:" << node->id - MIN_PORT << ": Gateway mutex unlock error\n";
            node->active = false;
            break;
        }

        switch (vectData[0])
        {   
            case CREATE:
            {
                zmq_msg_t msg;
                act = zmq_msg_init_size(&msg, 2 * sizeof(int));
                if (act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n";
                    break;
                }

                memcpy(zmq_msg_data(&msg), &vectData[0], 2 * sizeof(int));
                act = zmq_msg_send(&msg, req, 0);
                if (act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n";
                    break;
                }

                int pid;
                act = zmq_recv(req, &pid, sizeof(int), 0);
                if(act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n";
                    break;
                }

                if (vectData[1] < node->id){
                    node->L = new async_node(vectData[1]);
                } else {
                    node->R = new async_node(vectData[1]);
                }

                std::cout << "Ok: " << pid << '\n';
                zmq_msg_close(&msg);
                break;
            }

            case EXEC:
            {
                zmq_msg_t msg;
                int len = sizeof(int) * 2;
                act = zmq_msg_init_size(&msg, len);
                if (act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n";
                    break;
                }
                                
                memcpy(zmq_msg_data(&msg), &vectData[0], len);
                act = zmq_msg_send(&msg, req, 0);
                if (act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n";
                    break;
                }    

                long long ans;
                act = zmq_recv(req, &ans, sizeof(long long), 0);
                if(act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n";
                    break;
                }

                if (ans >= 0 ) {
                    std::cout << "Ok:" << node->id - MIN_PORT << ':' << ans << '\n';
                } else {
                    std::cout << "Ok:" << node->id - MIN_PORT << '\n';
                }
                zmq_msg_close(&msg);
                break;
            }

            case REMOVE:
            {
                act = zmq_send(req, &vectData[0], sizeof(int), 0);
                if (act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n";
                    break;
                }
                int ans;
                act = zmq_recv(req, &ans, sizeof(int), 0);
                if (act == -1){
                    std::cout << "Error:" << node->id - MIN_PORT << ": Message error\n"; 
                    break;
                }
                break;
            }
        }
    }
    zmq_close(req);
    zmq_ctx_destroy(context);
    delete node;
    return NULL;
}

#endif
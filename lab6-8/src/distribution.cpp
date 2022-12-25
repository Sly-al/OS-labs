#include "node.h"

int ConvertStrToNum(const std::string& commString){
    if (commString == "start"){
            return 0;
        }
    if (commString == "time"){
        return 1;
    }
    return 2;
}  

async_node* find_node_exec(async_node* ptr, int id)
{
    if (ptr == nullptr)
        return nullptr;
    if (ptr->id > id)
        return find_node_exec(ptr->L, id);
    if (ptr->id < id)
        return find_node_exec(ptr->R, id);
    return ptr;
}

async_node* find_node_create(async_node* ptr, int id)
{
    if (ptr == nullptr)
        return nullptr;
    if (ptr->L == nullptr && ptr->id > id)
        return ptr;
    if (ptr->R == nullptr && ptr->id < id)
        return ptr;
    if (ptr->id > id)
        return find_node_create(ptr->L, id);
    if (ptr->id < id)
        return find_node_create(ptr->R, id);
    return nullptr;
}

bool destroy_node(async_node*& ptr, int id)
{
    if (ptr == nullptr)
        return false;
    if (ptr->id > id)
        return destroy_node(ptr->L, id);
    if (ptr->id < id)
        return destroy_node(ptr->R, id);
    ptr->active = false;
    ptr->make_query({REMOVE});
    if (ptr->L != nullptr)
        destroy_node(ptr->L, ptr->L->id);
    if (ptr->R != nullptr)
        destroy_node(ptr->R, ptr->R->id);
    ptr = nullptr;
    return true;
}

bool ping(int id)
{
    std::string port = protocol + std::to_string(id);
    std::string ping = "inproc://ping" + std::to_string(id);
    void* context = zmq_ctx_new();
    void *req = zmq_socket(context, ZMQ_REQ);

    zmq_socket_monitor(req, ping.c_str(), ZMQ_EVENT_CONNECTED | ZMQ_EVENT_CONNECT_RETRIED);
    void *soc = zmq_socket(context, ZMQ_PAIR);
    zmq_connect(soc, ping.c_str());
    zmq_connect(req, port.c_str());

    zmq_msg_t msg;
    zmq_msg_init(&msg);
    zmq_msg_recv(&msg, soc, 0);
    uint8_t* data = (uint8_t*)zmq_msg_data(&msg);
    uint16_t event = *(uint16_t*)(data);

    zmq_close(req);
    zmq_close(soc);
    zmq_msg_close(&msg);
    zmq_ctx_destroy(context);
    return event % 2;
}

async_node* tree = nullptr;

int main()
{
    while (true)
    {

        std::string command;
        std::cin >> command;
        if (command == "create")
        {
            int id;
            std::cin >> id;
            id += MIN_PORT;
            if (tree == nullptr)
            {
                std::string id_str = std::to_string(id);
                int pid = fork();
                if (pid == 0)
                    execl("server", "server", id_str.c_str(), NULL);
                std::cout << "Ok: " << pid << '\n';
                tree = new async_node(id);
            }
            else
            {
                async_node* node = find_node_create(tree, id);
                
                if (node != nullptr){
                    if (!ping(node->id))
                    {
                        std::cerr << "Error:" << id - MIN_PORT << ": Parent is unavailable\n";
                        continue;
                    }
                    node->make_query({CREATE, id});
                }
                else
                    std::cerr << "Error: Already exists\n";
            }
        }

        if (command == "exec")
        {
            int id, commandNumber;
            std::string commandString;
            std::cin >> id >> commandString;
            id += MIN_PORT;
            commandNumber = ConvertStrToNum(commandString);
            std::vector <int> vectData(2);
            vectData[0] = EXEC;
            vectData[1] = commandNumber;
            async_node* node = find_node_exec(tree, id);
            if (node != nullptr){
                node->make_query(vectData);
                if (!ping(id)) {
                    std::cerr << "Error:" << id - MIN_PORT << ": Node is unavailable\n";
                    break;
                }
            } else {
                std::cerr << "Error:" << id - MIN_PORT << ": Not found\n";
            }
        }

        if (command == "remove")
        {
            int id;
            std::cin >> id;
            id += MIN_PORT;
            if (!ping(id))
            {
                std::cerr << "Error:" << id - MIN_PORT << ": Node is unavailable\n";
                continue;
            }
            bool state = destroy_node(tree, id);
            if (state)
                std::cout << "Ok\n";
            else
                std::cerr << "Error: Not found\n";
        }

        if (command == "ping")
        {
            int id;
            std::cin >> id;
            id += MIN_PORT;
            async_node* node = find_node_exec(tree, id);
            if (node != nullptr){
                if (ping(id)) {
                    std::cout << "Ok: 1"<< '\n';
                } else {
                    std::cout << "Ok: 0"<< '\n';
                }
            } else {
                std::cerr << "Error:" << id - MIN_PORT << ": Not found\n";
            }
        }
    }
}
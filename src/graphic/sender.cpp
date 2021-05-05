#include "../../includes/graphic/sender.hpp"

#ifdef _WIN32
int graphic::Sender::instance_number = 0;
#endif

graphic::Sender::Sender(int port) : connection{nullptr}, messageHistory{}
{
    if (port <= 0)
    {
        throw std::invalid_argument("The port specified is " 
                + std::to_string(port) 
                + " which is <= 0!");
    }

#ifdef _WIN32
    if (instance_number == 0)
    {
        ix::initNetSystem();
    }
    instance_number++;
#endif
    
    startServer(port);
}

graphic::Sender::~Sender()
{
    stopServer();

#ifdef _WIN32
    instance_number--;

    if (instance_number == 0)
    {
        ix::uninitNetSystem();
    }
#endif
}

void graphic::Sender::stopServer()
{
    if (connection != nullptr)
    {
        closeConnection();
    }

    server->stop();
}

void graphic::Sender::closeConnection()
{
    connection->close();
    connection = nullptr;
}

void graphic::Sender::setCallback()
{
    SharedQueue<std::string>* messageHistoryRef = &(this->messageHistory);

    // Set the actions to perform when the server receive a message
    server->setOnClientMessageCallback([this, messageHistoryRef](std::shared_ptr<ix::ConnectionState> connectionState,
            ix::WebSocket &webSocket, const ix::WebSocketMessagePtr &msg)
    {
        
        // If the message is about opening a new connection..
        if (msg->type == ix::WebSocketMessageType::Open)
        {
            if (!this->isConnected())
            {
                connection = &webSocket;
            }
            
            int messagesToSend = messageHistoryRef->size();
            if (messagesToSend > 0)
            {
                for (int i = 0; i < messagesToSend; i++)
                {
                    webSocket.send(messageHistoryRef->front());
                    messageHistoryRef->pop();
                }
            }
        }
        // If the message is about closing the connection..
        else if (msg->type == ix::WebSocketMessageType::Close)
        {
            connection = nullptr;
        }

    });
}

void graphic::Sender::initializeServer(int port)
{
    server = std::make_unique<ix::WebSocketServer>(port, ix::SocketServer::kDefaultHost, 
            ix::SocketServer::kDefaultTcpBacklog, 1);
    setCallback();
}

void graphic::Sender::startServer(int port)
{
    initializeServer(port);

    // Start the server in background
    server->listenAndStart();
}

void graphic::Sender::send(std::string data)
{
    if (connection != nullptr && messageHistory.empty())
    {
        connection->send(data);
    }
    else
    {
        messageHistory.push(data);
    }
}

bool graphic::Sender::isConnected()
{
    return connection != nullptr;
}
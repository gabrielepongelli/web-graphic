#pragma once

#include "../utilities/shared_queue.hpp"

#ifdef _WIN32
    #include <ixwebsocket/IXNetSystem.h>
#endif
#include <ixwebsocket/IXWebSocketServer.h>
#include <memory>
#include <string>

namespace graphic
{
    #define DEFAULT_PORT 8888

    /**
     * A Sender object is responsible for communications with the client.
     */
    class Sender
    {
        public:
            /**
             * @param port: port that this sender have to use. Must be > 0.
             */
            Sender(int port=DEFAULT_PORT);
            ~Sender();

            /**
             * Indicates whether this sender is connected with a receiver or not.
             * @return true if it is, false otherwise.
             */
            bool isConnected();

            /**
             * Send data to the receiver.
             * @param data: data to be sent to the receiver.
             */
            void send(std::string data);
        
        private:
            // Represent the connection with the client.
            ix::WebSocket *connection;
            std::unique_ptr<ix::WebSocketServer> server;

            // Represent the ordered list of messages to be sent when a Receiver connects
            SharedQueue<std::string> messageHistory;
            #ifdef _WIN32
            // Represent the number of instances of Sender in use.
            static int instance_number;
            #endif
            
            /**
             * Initialize the server attribute.
             * @param port: port to open the listening server. Must be > 0.
             */
            void initializeServer(int port);

            /**
             * Initialize the server attribute and start listening in 
             * non-blocking mode.
             */
            void startServer(int port);

            /**
             * Set the callback action to be performed when a message is being 
             * received by the server attribute.
             */
            void setCallback();

            /**
             * Close the connection with the client if it is yet open.
             */
            void closeConnection();

            /**
             * Stop the server from listening.
             */
            void stopServer();
    };

}
#pragma once

#ifdef _WIN32
    #include <ixwebsocket/IXNetSystem.h>
#endif
#include <ixwebsocket/IXWebSocketServer.h>
#include <memory>
#include <exception>

namespace graphic
{
    #define DEFAULT_PORT 8888

    class NotConnectedException;

    /**
     * A Sender object is responsible for communications with the client.
     */
    class Sender
    {
        public:
            /**
             * @param port: port that this sender have to use. Must be > 0.
             * @throws invalid_argument if the value of port is <= 0.
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
             * @throws NotConnectedException if the receiver is not connected.
             */
            void send(std::string data);
        
        private:
            // Represent the connection with the client.
            ix::WebSocket *connection;
            std::unique_ptr<ix::WebSocketServer> server;
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

    /**
     * This exception is thrown when a Sender object try to send some data to a 
     * Receiver object without an open connection.
     */
    class NotConnectedException: public std::runtime_error
    {
        public:
            NotConnectedException(const std::string &what_arg) 
                    : std::runtime_error(what_arg) { };
            
            NotConnectedException(const char* what_arg) 
                    : std::runtime_error(what_arg) { };
    };
}
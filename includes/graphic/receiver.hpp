#pragma once

#include "../utilities/string_utilities.hpp"

#include <string>

namespace graphic
{

    /**
     * This class represent the receiver which interacts with a Sender object.
     */
    class Receiver
    {
        public:
            /**
             * Start the receiver present in the position specified.
             * @param receiver_path: path where the receiver is saved.
             * @return true if the operation is completed successfully, false otherwise.
             */
            static bool startReceiver(std::string receiver_path);
    };

}
#pragma once

#include "sender.hpp"
#include "receiver.hpp"

#include <string>
#include <set>
#include <vector>
#include <exception>
#include <utility>

namespace graphic
{
    struct record_t 
    {
        double xValue;
        double yValue;
        double deviation;
    }
    typedef Record;
    
    /**
     * A Color in the format rgba.
     */
    struct Color
    {
        static constexpr Color red() { return Color(255, 0, 0); }
        static constexpr Color green() { return Color(0, 255, 0); }
        static constexpr Color blue() { return Color(0, 0, 255); }
        static constexpr Color black() { return Color(0, 0, 0); }

        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : r(r), g(g), b(b), a(a) {}

        uint8_t r, g, b, a;
    };

    /**
     * List of possible graphic types.
     */
    enum class Type
    {
        /**
         * Standard graphic without any features.
         */
        STANDARD,

        /**
         * Standard graphic that can display also the standard deviation of the data.
         */
        WITH_DEVIATION,

        /**
         * Standard graphic that can also switch from linear scale to logaritmic ones.
         */
        STANDARD_LOGARITMIC,

        /**
         * Is a combination of WITH_DEVIATION and STANDARD_LOGARITMIC types.
         */
        LOGARITMIC_WITH_DEVIATION
    };

    /**
     * This exception is thrown when an invalid name for a line is specified.
     */
    class InvalidLineNameException;

    /**
     * This exception is thrown when it is not possible to start the client located
     * in the position specified by the public attribute client_path.
     */
    class ErrorStartingReceiverException;

    /**
     * A WebGraphic is a real-time graphic which is displayed in the user's browser. It can 
     * be of one of the types specified by Type.
     */
    class WebGraphic
    {
        public:

            /**
             * Path where the client is saved.
             */
            static std::string client_path;

            /**
             * Create a new real-time graphic of a specified type.
             * @param title: name to assign to the main title of the graphic.
             * @param xName: name to assign to the x axis.
             * @param yName: name to assign to the y axis.
             * @param t: type of the graphic. If not specified it will be created
             *      of type STANDARD.
             * @throws ErrorStartingReceiverException if there is an error on 
             *      starting the Receiver.
             */
            WebGraphic(std::string title, std::string xName, std::string yName, 
                    Type t = Type::STANDARD);

            /**
             * Add a new line to this graphic.
             * @param name: the name of the new line that has to be added. It must 
             *      be different from names of other lines.
             * @param c: color of the new line to add.
             * @throws InvalidLineNameException if the line name is not valid. 
             *      This could happen when the name specified is already used.
             */
            void addLine(std::string name, Color c);

            /**
             * @return a const iterator for all the line's names of this graphic.
             */
            auto getLines() const;

            /**
             * Add a new record to this WebGraphic on the line specified.
             * @param r: record to be added to the graphic.
             * @param l: line on which to add the record.
             * @throws InvalidLineNameException if the line name specified doesn't 
             *      exist.
             */
            void addRecord(Record r, std::string l);

        private:
            Sender s;
            std::set<std::string> lines;
            bool withDeviation;

            /**
             * Send to the Receiver the type of graphic to create together with 
             * its axis names and its title.
             */
            void specifyGraphicProperties(std::string title, std::string xName, 
                    std::string yName, graphic::Type t);
            
            /**
             * Send the new line type to the Receiver.
             */
            void sendNewLineType(std::string name, Color c);

            /**
             * Send the new record to the Receiver.
             */
            void sendNewRecord(Record r, std::string l);
    };

    /**
     * Builder pattern to facilitize the creation of a WebGraphic.
     */
    class WebGraphicBuilder
    {
        public:
            /**
             * Create a new real-time graphic builder with an empty title, empty 
             * axis names and of standard type.
             */
            WebGraphicBuilder();

            /**
             * Specify the type of the new graphic.
             * @param t: type of the graphic.
             */
            WebGraphicBuilder& ofType(Type t);

            /**
             * Specify the title of the new graphic.
             * @param title: name to assign to the main title of the graphic.
             */
            WebGraphicBuilder& withTitle(std::string title);

            /**
             * Specify the x axis' name of the new graphic.
             * @param name: name to assign to the x axis.
             */
            WebGraphicBuilder& withXName(std::string name);

            /**
             * Specify the y axis' name of the new graphic.
             * @param name: name to assign to the y axis.
             */
            WebGraphicBuilder& withYName(std::string name);

            /**
             * Specify a new line to add to the new graphic.
             * @param name: the name of the new line that has to be added. It must 
             *      be different from names of other lines.
             * @param c: color of the new line to add.
             * @throws InvalidLineNameException if the line name is not valid. 
             *      This could happen when the name specified is already used.
             */
            WebGraphicBuilder& withLine(std::string name, Color c);

            /**
             * Build the new graphic with the properties specified.
             */
            WebGraphic* build();

        private:
            std::string title;
            std::string xName;
            std::string yName;
            Type type;
            std::vector<std::pair<std::string, Color>> lines;
            std::set<std::string> lineTitles;
    };

    class InvalidLineNameException: std::invalid_argument
    {
        public:
            InvalidLineNameException(const std::string &what_arg) 
                    : std::invalid_argument(what_arg) { };
            
            InvalidLineNameException(const char* what_arg) 
                    : std::invalid_argument(what_arg) { };
    };

    class ErrorStartingReceiverException: std::runtime_error
    {
        public:
            ErrorStartingReceiverException(const std::string &what_arg) 
                    : std::runtime_error(what_arg) { };
            
            ErrorStartingReceiverException(const char* what_arg) 
                    : std::runtime_error(what_arg) { };
    };

}
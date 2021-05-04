#include "../../includes/graphic/web_graphic.hpp"

#include <unistd.h>
#include <nlohmann/json.hpp>

std::string graphic::WebGraphic::client_path = "../assets/graphic.html";

graphic::WebGraphic::WebGraphic(graphic::Type t)
            : s{}, lines{}
{
    if (!Receiver::startReceiver(client_path))
    {
        throw ErrorStartingReceiverException("Error starting the client located in " + client_path);
    }

    withDeviation = (t == Type::LOGARITMIC_WITH_DEVIATION || t == Type::WITH_DEVIATION);

    usleep(100000);    // Wait 100 ms for the connection
    specifyGraphicType(t);
}

void graphic::WebGraphic::specifyGraphicType(graphic::Type t)
{
    if (!s.isConnected())
    {
        return;
    }

    std::string type;
    switch (t)
    {
        case Type::STANDARD:
            type = "standard";
            break;
        case Type::WITH_DEVIATION:
            type = "with deviation";
            break;
        case Type::STANDARD_LOGARITMIC:
            type = "standard logaritmic";
            break;
        case Type::LOGARITMIC_WITH_DEVIATION:
            type = "logaritmic with deviation";
            break;
    }

    s.send(type);
}

void graphic::WebGraphic::addLine(std::string name, graphic::Color c)
{
    if (name.empty())
    {
        throw InvalidLineNameException("The name specified is empty!");
    }

    if (lines.find(name) != lines.end())
    {
        throw InvalidLineNameException("The name " + name + " is already in use!");
    }

    lines.insert(name);
    sendNewLineType(name, c);
}

void graphic::WebGraphic::sendNewLineType(std::string name, graphic::Color c)
{
    if (!s.isConnected())
    {
        return;
    }

    auto newLine = R"(
        {
            "name": "",
            "type": "line",
            "color": "",
            "connectNullData": true,
            "showInLegend": true,
            "dataPoints": []
        }
    )"_json;
    newLine["name"] = name;
    newLine["color"] = "rgba(" 
            + std::to_string(c.r) + "," 
            + std::to_string(c.g) + ","
            + std::to_string(c.b) + ","
            + std::to_string((double)c.a * 100.0 / 255.0) + ")";
    
    s.send(newLine.dump());

    if (withDeviation)
    {
        newLine = R"(
            {
                "name": "",
                "type": "rangeArea",
                "color": "",
                "connectNullData": true,
                "showInLegend": true,
                "dataPoints": []
            }
        )"_json;
        newLine["name"] = name + " Deviation";
        newLine["color"] = "rgba(" 
            + std::to_string(c.r) + "," 
            + std::to_string(c.g) + ","
            + std::to_string(c.b) + ","
            + std::to_string(((double)c.a * 100.0 / 255.0) - 0.4) + ")";
        s.send(newLine.dump());
    }
}

auto graphic::WebGraphic::getLines() const
{
    return lines.cbegin();
}

void graphic::WebGraphic::addRecord(graphic::Record r, std::string l)
{
    if (!s.isConnected())
    {
        return;
    }

    if (lines.find(l) == lines.end())
    {
        throw InvalidLineNameException("Could not find a line in the graphic with name: " + l);
    }

    auto newRecord = R"(
        {
            "x": 0.0,
            "y": 0.0
        }
    )"_json;
    newRecord["x"] = r.xValue;
    newRecord["y"] = r.yValue;
    s.send(newRecord.dump());

    if (withDeviation)
    {
        newRecord = R"(
            {
                "x": 0.0,
                "y": []
            }
        )"_json;
        newRecord["x"] = r.xValue;
        newRecord["y"][0] = r.yValue + r.deviation;
        newRecord["y"][1] = r.yValue - r.deviation;
        s.send(newRecord.dump());
    }
}
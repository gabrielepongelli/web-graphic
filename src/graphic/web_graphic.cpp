#include "../../includes/graphic/web_graphic.hpp"

#include <nlohmann/json.hpp>

#ifndef _WIN32
std::string graphic::WebGraphic::client_path = "../assets/graphic.html";
#else
std::string graphic::WebGraphic::client_path = "..\\assets\\graphic.html";
#endif

graphic::WebGraphic::WebGraphic(std::string title, std::string xName, std::string yName, 
            graphic::Type t) : s{}, lines{}
{
    if (!Receiver::startReceiver(client_path))
    {
        throw ErrorStartingReceiverException("Error starting the client located in " + client_path);
    }

    withDeviation = (t == Type::LOGARITMIC_WITH_DEVIATION || t == Type::WITH_DEVIATION);

    specifyGraphicProperties(title, xName, yName, t);
}

void graphic::WebGraphic::specifyGraphicProperties(std::string title, std::string xName, 
            std::string yName, graphic::Type t)
{
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

    auto properties = R"(
        {
            "title": "",
            "xName": "",
            "yName": "",
            "type": ""
        }
    )"_json;
    properties["title"] = title;
    properties["xName"] = xName;
    properties["yName"] = yName;
    properties["type"] = type;

    s.send(properties.dump());
}

void graphic::WebGraphic::addLine(std::string name, graphic::Color c)
{
    if (lines.find(name) != lines.end())
    {
        throw InvalidLineNameException("The name " + name + " is already in use!");
    }

    lines.insert(name);
    sendNewLineType(name, c);
}

void graphic::WebGraphic::sendNewLineType(std::string name, graphic::Color c)
{
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
            + std::to_string((double)c.a / 255.0) + ")";
    
    s.send(newLine.dump());

    if (withDeviation)
    {
        double deviationOpacity = ((double)c.a / 255.0) - 0.5;

        if (deviationOpacity <= 0)
        {
            deviationOpacity = 0.1;
        }

        newLine = R"(
            {
                "name": "",
                "type": "rangeArea",
                "color": "",
                "connectNullData": true,
                "showInLegend": false,
                "dataPoints": []
            }
        )"_json;
        newLine["name"] = name + " Deviation";
        newLine["color"] = "rgba(" 
            + std::to_string(c.r) + "," 
            + std::to_string(c.g) + ","
            + std::to_string(c.b) + ","
            + std::to_string(deviationOpacity) + ")";
        s.send(newLine.dump());
    }
}

auto graphic::WebGraphic::getLines() const
{
    return lines.cbegin();
}

void graphic::WebGraphic::addRecord(graphic::Record r, std::string l)
{
    if (lines.find(l) == lines.end())
    {
        throw InvalidLineNameException("Could not find a line in the graphic with name: " + l);
    }

    auto newRecord = R"(
        {
            "name": "",
            "x": 0.0,
            "y": 0.0
        }
    )"_json;
    newRecord["name"] = l;
    newRecord["x"] = r.xValue;
    newRecord["y"] = r.yValue;
    s.send(newRecord.dump());

    if (withDeviation)
    {
        newRecord = R"(
            {
                "name": "",
                "x": 0.0,
                "y": []
            }
        )"_json;
        newRecord["name"] = l + " Deviation";
        newRecord["x"] = r.xValue;
        newRecord["y"][0] = r.yValue + r.deviation;
        newRecord["y"][1] = r.yValue - r.deviation;
        s.send(newRecord.dump());
    }
}

graphic::WebGraphicBuilder::WebGraphicBuilder() : title{""}, xName{""}, yName{""}, 
            type{Type::STANDARD}, lines{}, lineTitles{} { };

graphic::WebGraphicBuilder& graphic::WebGraphicBuilder::ofType(Type t)
{
    type = t;
    return *this;
}

graphic::WebGraphicBuilder& graphic::WebGraphicBuilder::withTitle(std::string title)
{
    this->title = title;
    return *this;
}

graphic::WebGraphicBuilder& graphic::WebGraphicBuilder::withXName(std::string name)
{
    xName = name;
    return *this;
}

graphic::WebGraphicBuilder& graphic::WebGraphicBuilder::withYName(std::string name)
{
    yName = name;
    return *this;
}

graphic::WebGraphicBuilder& graphic::WebGraphicBuilder::withLine(std::string name, graphic::Color c)
{
    if (lineTitles.find(name) != lineTitles.end())
    {
        throw InvalidLineNameException("The name " + name + " is already in use!");
    }

    lineTitles.emplace(name);
    lines.emplace_back(name, c);
    return *this;
}

graphic::WebGraphic* graphic::WebGraphicBuilder::build()
{
    WebGraphic *result = new WebGraphic(title, xName, yName, type);

    for (auto line : lines)
    {
        result->addLine(line.first, line.second);
    }

    return result;
}
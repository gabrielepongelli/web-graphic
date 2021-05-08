# WebGraphic
A simple c++ library to create and manage real-time line graphic.

![Presentation Image](/images/Presentation_Image.png)

<details open="open">
    <summary>Table of Contents</summary>
    <ol>
        <li>
            <a href="#about-the-project">About The Project</a>
            <ul>
                <li><a href="#built-with">Built With</a></li>
            </ul>
        </li>
        <li>
            <a href="#getting-started">Getting Started</a>
            <ul>
                <li><a href="#prerequisites">Prerequisites</a></li>
                <li><a href="#installation">Installation</a></li>
            </ul>
        </li>
        <li><a href="#usage">Usage</a></li>
        <li><a href="#examples">Example</a></li>
        <li>
            <a href="#notes">Notes</a>
            <ul>
                <li><a href="#logarithmic-scale">Logarithmic scale</a></li>
                <li><a href="#issue-with-safari">Issue with Safari</a></li>
            </ul>
        </li>
    </ol>
</details>
<br>

## About The Project

This is a little project that allows to create a simple and easy-to-use line graphic through c++ which will be updated in real-time. The graphic will be displayed inside the local browser. More specifically it allows to:
* Create a real-time line graphic with custom axis names and title
* Add lines to be displayed on it
* Show the standard deviation of a point in the graphic
* Switch between linear and logarithmic scale

Just this and nothing else.

### Built With

This project is built using:
* [IXWebSocket](https://machinezone.github.io/IXWebSocket/)
* [Nlohmann Json](https://github.com/nlohmann/json)
* [Bootstrap](https://getbootstrap.com)
* [JQuery](https://jquery.com)
* [CanvasJS](https://canvasjs.com)


<br>

## Getting Started

The code of this project is compatible with Linux, MacOS and Windows.

### Prerequisites

To be able to use this project you need:
* Make
* Cmake

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/gabrielepongelli/web-graphic.git && cd web-graphic
   ```
2. Create a build directory
   ```sh
   mkdir build && cd build
   ```
3. Generate the make file
   ```sh
   cmake ..
   ```
4. Build the project
   ```sh
   cmake --build .
   ```


<br>

## Usage

After completing the [installation procedure](#installation) you can modify the code changing the project name in the CMakeLists.txt file and add your personal source and header files. Remember to add the source files also in the CMakeLists.txt file!
To make use of the library you will use the WebGraphic class located inside `includes/graphic/web_graphic.hpp`. The header will provide a namespace called `graphic` with:
* A Type enumeration to specify the type of the graphic that you want to build (choose between `STANDARD`, `WITH_DEVIATION`, `STANDARD_LOGARITMIC` and `LOGARITMIC_WITH_DEVIATION`)
* A Color struct to specify the color of the lines in the graphic
* A Record struct that will contain information about a new point to add to the graphic 
* A class named WebGraphicBuilder to facilitize the creation of the graphic. It will provde the following methods:
  * `ofType(graphic::Type t)` to specify the graphic type
  * `withTitle(std::string title)` to specify the title
  * `withXName(std::string name)` to specify the title of the x axis
  * `withYName(std::string name)` to specify the title of the y axis
  * `withLine(std::string name, graphic::Color c)` to add a line with a specified name and a specified color
  * `build()` to build the graphic and obtain its correspondent `WebGraphic` object
* A class named WebGraphic that represent the graphic. It will provde the following methods:
  * `addLine(std::string name, graphic::Color c)` to add a new line with a specified name and a specified color
  * `getLines()` to obtain a list of all the line names used
  * `addRecord(graphic::Record r, std::string l)` to add a new point to the specified line


<br>

## Example

This is an example for a basic usage:
```cpp
#include <unistd.h>

#include "includes/graphic/web_graphic.hpp"

int main()
{
    graphic::WebGraphic *wg = graphic::WebGraphicBuilder()
            .ofType(graphic::Type::LOGARITMIC_WITH_DEVIATION)
            .withTitle("Prova")
            .withXName("Length")
            .withYName("Time")
            .withLine("Test 1", graphic::Color::red())
            .withLine("Test 2", graphic::Color::blue())
            .build();

    graphic::Record r;

    for (int i = 0; i < 50; i++)
    {
        r.xValue = i+1;
        r.yValue = rand() % 101;
        r.deviation = 0.4;
        wg->addRecord(r, "Test 1");
        
        r.xValue = i+1;
        r.yValue = rand() % 101;
        r.deviation = 0.4;
        wg->addRecord(r, "Test 2");

        usleep(1000000);
    }

    return 0;
}
```


<br>

## Notes

### Logarithmic scale

If you need to use a logarithmic scale in your graphic make sure to submit only values greater than zero since the logarithmic function is defined only for those values, otherwise it won't displat anything.


### Issue with Safari

At the time of writing (6 May 2021), if the points are added too quickly, Safari will freeze until the speed decreases. Don't worry, the points will be added, but you won't be able to see them in real time.
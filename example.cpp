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
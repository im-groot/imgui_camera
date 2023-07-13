#include "App.h"
#include "implot.h"

struct ImPlotDemo : App {
    using App::App;
    void Update() override {
        ImPlot::ShowDemoWindow();   
    }
};

int main(int argc, char const *argv[])
{
    ImPlotDemo app("ImPlot Demo",1920,1080,argc,argv);
    app.Run();

    return 0;
}
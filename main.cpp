#include "App.h"
#include "implot.h."
#include "mycamera.hpp"

struct ImPlotDemo : App {
    using App::App;
    Mycamera camera_;
    cv::Mat image_;
    //ImTextureID image_texture_;
    GLuint image_texture_;
    ~ImPlotDemo() {
        camera_.stop();
    }
    void Start() {
        camera_.start();
    }
    void Update() override {
        //ImPlot::ShowDemoWindows();
        ImGui::Begin("ImPlot Demo");

        if (ImPlot::BeginPlot("##image",ImVec2(480,360))) {

            static int counter = 0;

            static ImVec2 bmin(0,0);
            static ImVec2 bmax(1,1);
            static ImVec2 uv0(0,0);
            static ImVec2 uv1(1,1);
            static ImVec4 tint(1,1,1,1);

            static int out_width = 640;
            static int out_height = 480;

            if(camera_.do_update()) {
                camera_.get_current_image(image_);
                out_width = image_.size().width;
                out_height = image_.size().height;
            }

            if (counter == 0) glGenTextures(1, &image_texture_);
            glBindTexture(GL_TEXTURE_2D, image_texture_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, out_width, out_height, 0, GL_BGR, GL_UNSIGNED_BYTE, image_.data);

            ImPlot::PlotImage("my camera",(void*)(intptr_t)image_texture_, bmin, bmax, uv0, uv1, tint);
            ImPlot::EndPlot();

            counter++;

            //glDeleteTextures(1, &image_texture_);
        }

        ImGui::End();
    }
};

int main(int argc, char const *argv[])
{
    ImPlotDemo app("ImPlot Demo",1920,1080,argc,argv);
    app.Run();

    return 0;
}
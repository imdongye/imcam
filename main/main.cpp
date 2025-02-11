#include <iostream>
#include <opencv2/opencv.hpp>
#include <limbrary/application.h>
#include <imgui.h>

using namespace lim;

class AppTemplate : public AppBase
{
public:
    inline static constexpr CStr APP_NAME = "template";
    inline static constexpr CStr APP_DIR  = "im_tests/";
    inline static constexpr CStr APP_INFO = "limbrary template application";

public:
    AppTemplate(): AppBase(1200, 780, APP_NAME)
    {
        key_callbacks[this] = [](int key, int scancode, int action, int mods) {
            // log::pure("%d\n", key);
            // log::pure("%d\n", key);
            // log::warn("%d\n", key);
            // log::err("%d\n", key);
        };
    }
    ~AppTemplate()
    {
    }
    virtual void update() override
    {
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, fb_width, fb_height);
        glClearColor(0.05f, 0.09f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    virtual void updateImGui() override
    {
        ImGui::DockSpaceOverViewport();

        ImGui::Begin("test window2##template");
        ImGui::End();
    }
};


int main() {
    AppTemplate app;
    app.run();

    return 0;
}
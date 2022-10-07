#include <cstring>
#include <vector>
#include <algorithm>
#include "object.h"
#include "vector.h"
#include "matrix.h"
#include "platform.h"
#include "graphics.h"
#include "program.h"

// Window
static const char *const WindowITLE = "SoftRenderer";
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;

// Camera
Camera camera;
double preX = static_cast<double>(WINDOW_WIDTH) / 2.0;
double preY = static_cast<double>(WINDOW_HEIGHT) / 2.0;
bool firstMouse = true;

// Timing
double deltaTime = 0.0;
double preTime = 0.0;

void keyboardCallback(Window* window, double deltaTime);
void mouseCallback(Window* window);

int main(int argc, char** argv) {
    platformInitialize();

    // Create window
    Window* window = windowCreate(WindowITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
        200, 100);
    if (window == nullptr) {
        std::cerr << "Falied to create window" << std::endl;
        return -1;
    }
    Framebuffer* framebuffer = new Framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
    float aspect = static_cast<float>(WINDOW_WIDTH) /
        static_cast<float>(WINDOW_HEIGHT);

    // double preTime = platform_get_time();
    // double printTime = preTime;
    // int num_frames = 0;
    // const int text_size = 500;
    // char screen_text[text_size];
    // int show_num_frames = 0;
    // int show_avg_millis = 0;
    // double refresh_screen_text_timer = 0;
    // const double REFRESH_SCREEN_TEXT_TIME = 0.1;
    // snprintf(screen_text, text_size, "fps: - -, avg: - -ms\n");

    // Load model
    Object cup("D:/Code/Graphics/SoftRenderer/assets/helmet.obj",
        "D:/Code/Graphics/SoftRenderer/assets/helmet_basecolor.tga");
    Object floor("../assets/floor.obj", "../assets/floor_diffuse.tga");
    Program program;
    program.light = new DirectionalLight(true);

    while (!windowShouldClose(window)) {
        double curTime = platformGetTime();
        double deltaTime = curTime - preTime;

        keyboardCallback(window, deltaTime);
        mouseCallback(window);

        // Shadow mapping
        // Pass 1: render from light source
        // drawObject(cup, program);
        // drawObject(floor, program);

        // Pass 2: render from camera
        // 计算变换矩阵
        Matrix4 model = Matrix4::identity();
        Matrix4 view = getViewMatrix(camera.position, camera.front,
            camera.worldup);
        Matrix4 projection = getPerspectiveMatrix(camera.zoom, aspect, -0.01, -100);
        program.vertexShader = new VertexShader(model, view, projection);

        // // 计算帧率和耗时
        // num_frames += 1;
        // if (curTime - printTime >= 1) {
        //     int sum_millis = (int)((curTime - printTime) * 1000);
        //     int avg_millis = sum_millis / num_frames;

        //     show_num_frames = num_frames;
        //     show_avg_millis = avg_millis;
        //     num_frames = 0;
        //     printTime = curTime;
        // }
        // preTime = curTime;

        drawObject(cup, program, framebuffer);
        drawObject(floor, program, framebuffer);
        windowDrawBuffer(window, framebuffer);
        // std::cout << "Camera position: " << camera.position << std::endl;

        // //更新显示文本信息
        // refresh_screen_text_timer += deltaTime;
        // if (refresh_screen_text_timer > REFRESH_SCREEN_TEXT_TIME)
        // {
        //     snprintf(screen_text, text_size, "");

        //     char line[50] = "";

        //     snprintf(line, 50, "fps: %3d, avg: %3d ms\n\n", show_num_frames, show_avg_millis);
        //     strcat(screen_text, line);

        //     window_draw_text(window, screen_text);
        //     refresh_screen_text_timer -= REFRESH_SCREEN_TEXT_TIME;
        // }

        framebuffer->clearColor(Color(0, 0, 0, 1));
        framebuffer->clearDepth(-10000);

        inputPollEvents();
    }

    windowDestroy(window);
    delete framebuffer;

    return 0;
}

void keyboardCallback(Window* window, double deltaTime)
{
    if (inputKeyPressed(window, KEY_A)) {
        camera.processKeyboard(LEFT, deltaTime);
    }
    if (inputKeyPressed(window, KEY_D)) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
    if (inputKeyPressed(window, KEY_W)) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if (inputKeyPressed(window, KEY_S)) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (inputKeyPressed(window, KEY_Q)) {
        camera.processKeyboard(DOWNWARD, deltaTime);
    }
    if (inputKeyPressed(window, KEY_E)) {
        camera.processKeyboard(UPWARD, deltaTime);
    }
}

void mouseCallback(Window* window) {
    double xpos, ypos;
    inputQueryCursor(window, &xpos, &ypos);

    if (firstMouse)
    {
        preX = xpos;
        preY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - preX;
    double yoffset = preY - ypos; // reversed since y-coordinates go from bottom to top

    preX = xpos;
    preY = ypos;

    camera.processMouseMove(xoffset, yoffset);
}
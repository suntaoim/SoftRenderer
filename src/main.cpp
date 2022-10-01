#include <cstring>
#include <vector>
#include <algorithm>
#include "model.h"
#include "vector.h"
#include "matrix.h"
#include "platform.h"
#include "graphics.h"

// Window
static const char *const WindowITLE = "Viewer";
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

Model* ourModel = nullptr;

void keyboard_callback(Window* window, double deltaTime);
void mouse_callback(Window* window);
void scroll_callback(Window* window, double offset);

int main(int argc, char** argv) {
    platform_initialize();

    // Create window
    Window* window = window_create(WindowITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
        200, 100);
    if (window == nullptr) {
        std::cerr << "Falied to create window" << std::endl;
        return -1;
    }
    Framebuffer* framebuffer = new Framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

    // // 处理输入
    // callbacks_t callbacks;
    // memset(&callbacks, 0, sizeof(callbacks_t));
    // // callbacks.button_callback = button_callback;
    // callbacks.scroll_callback = scroll_callback;
    // callbacks.mouse_callback = mouse_callback;
    // input_set_callbacks(window, callbacks);

    // Load model
    ourModel = new Model("../assets/african_head.obj");
    Image* texture = new Image("../assets/african_head_diffuse.png");
    Light light;

    float preTime = platform_get_time();
    float printTime = preTime;
    int num_frames = 0;
    const int text_size = 500;
    char screen_text[text_size];
    int show_num_frames = 0;
    int show_avg_millis = 0;
    float refresh_screen_text_timer = 0;
    const float REFRESH_SCREEN_TEXT_TIME = 0.1;
    snprintf(screen_text, text_size, "fps: - -, avg: - -ms\n");


    while (!window_should_close(window)) {
        float curTime = platform_get_time();
        double deltaTime = curTime - preTime;

        // 测试输入
        keyboard_callback(window, deltaTime);
        mouse_callback(window);

        // 计算变换矩阵
        Matrix4 model = Matrix4::identity();
        Matrix4 view = getViewMatrix(camera);
        Matrix4 projection = getPerspectiveMatrix(radians(camera.zoom),
            static_cast<double>(WINDOW_WIDTH) / static_cast<double>(WINDOW_HEIGHT),
            -0.1, -100);
        Matrix4 viewport = getViewportMatrix(WINDOW_WIDTH, WINDOW_HEIGHT);
        Shader shader(model, view, projection, viewport, light, texture,
            framebuffer);

        // 计算帧率和耗时
        num_frames += 1;
        if (curTime - printTime >= 1) {
            int sum_millis = (int)((curTime - printTime) * 1000);
            int avg_millis = sum_millis / num_frames;

            show_num_frames = num_frames;
            show_avg_millis = avg_millis;
            num_frames = 0;
            printTime = curTime;
        }
        preTime = curTime;

        ourModel->draw(shader);
        window_draw_buffer(window, framebuffer);

        //更新显示文本信息
        refresh_screen_text_timer += deltaTime;
        if (refresh_screen_text_timer > REFRESH_SCREEN_TEXT_TIME)
        {
            snprintf(screen_text, text_size, "");

            char line[50] = "";

            snprintf(line, 50, "fps: %3d, avg: %3d ms\n\n", show_num_frames, show_avg_millis);
            strcat(screen_text, line);

            window_draw_text(window, screen_text);
            refresh_screen_text_timer -= REFRESH_SCREEN_TEXT_TIME;
        }

        framebuffer->clearColor(Color(0, 0, 0, 1));
        framebuffer->clearDepth(-10000);

        input_poll_events();
    }

    window_destroy(window);
    delete framebuffer;
    delete ourModel;

    return 0;
}

void keyboard_callback(Window* window, double deltaTime)
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
    // if (inputKeyPressed(window, KEY_Q)) {
    //     camera.processKeyboard(DOWNWARD, deltaTime);
    // }
    // if (inputKeyPressed(window, KEY_E)) {
    //     camera.processKeyboard(UPWARD, deltaTime);
    // }
}

void mouse_callback(Window* window) {
    float xpos, ypos;
    input_query_cursor(window, &xpos, &ypos);

    if (firstMouse)
    {
        preX = xpos;
        preY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - preX;
    float yoffset = preY - ypos; // reversed since y-coordinates go from bottom to top

    preX = xpos;
    preY = ypos;

    camera.processMouseMove(xoffset, yoffset);
}

void scroll_callback(Window* window, double offset)
{
    camera.processMouseScroll(offset);
}
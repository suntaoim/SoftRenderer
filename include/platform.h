#ifndef PLATFORM_H
#define PLATFORM_H

#include "vector.h"
#include "image.h"

class Framebuffer {
public:
    int width, height;
    unsigned char* colorBuffer;
    float* depthBuffer;
public:
    Framebuffer(int _width = 800, int _height = 600);
    ~Framebuffer();
    void clearColor(const Vector4& color);
    void clearDepth(float depth);
};

using Window = struct Window;
typedef enum {KEY_A, KEY_D, KEY_S, KEY_W, KEY_SPACE, KEY_NUM} keycode_t;
typedef enum {BUTTON_L, BUTTON_R, BUTTON_NUM} button_t;
typedef struct {
    void (*key_callback)(Window *window, keycode_t key, int pressed);
    void (*button_callback)(Window *window, button_t button, int pressed);
    void (*scroll_callback)(Window *window, double offset);
    void (*mouse_callback)(Window* window);
} callbacks_t;

/* platform initialization */
void platform_initialize(void);
void platform_terminate(void);

/* window related functions */
Window *window_create(const char *title, int width, int height, int text_width, int text_height);
void window_destroy(Window *window);
int window_should_close(Window *window);
void window_set_userdata(Window *window, void *userdata);
void *window_get_userdata(Window *window);
void window_draw_buffer(Window *window, Framebuffer *buffer);
void window_draw_text(Window* window, char* text);

/* input related functions */
void input_poll_events(void);
int inputKeyPressed(Window *window, keycode_t key);
int input_button_pressed(Window *window, button_t button);
void input_query_cursor(Window *window, float *xpos, float *ypos);
void input_set_callbacks(Window *window, callbacks_t callbacks);

/* misc platform functions */
float platform_get_time(void);

#endif

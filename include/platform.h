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
typedef enum {KEY_A, KEY_D, KEY_S, KEY_W, KEY_Q, KEY_E, KEY_SPACE, KEY_NUM} Keycode;
typedef enum {BUTTON_L, BUTTON_R, BUTTON_NUM} Button;
typedef struct {
    void (*keyCallback)(Window *window, Keycode key, int pressed);
    void (*buttonCallback)(Window *window, Button button, int pressed);
    void (*scrollCallback)(Window *window, double offset);
    void (*mouseCallback)(Window* window);
} Callbacks;

/* platform initialization */
void platformInitialize(void);
void platformTerminate(void);

/* window related functions */
Window *windowCreate(const char *title, int width, int height, int text_width, int text_height);
void windowDestroy(Window *window);
int windowShouldClose(Window *window);
void windowSetUserdata(Window *window, void *userdata);
void *windowGetUserdata(Window *window);
void windowDrawBuffer(Window *window, Framebuffer *buffer);
void windowDrawText(Window* window, char* text);

/* input related functions */
void inputPollEvents(void);
int inputKeyPressed(Window *window, Keycode key);
int inputButtonPressed(Window* window, Button button);
void inputQueryCursor(Window* window, double* xpos, double* ypos);
void inputSetCallbacks(Window* window, Callbacks callbacks);

/* misc platform functions */
double platformGetTime(void);

#endif

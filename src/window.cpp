// #include "window.h"

// void mouse_callback(Window* window) {
//     float xpos, ypos;
//     input_query_cursor(window, &xpos, &ypos);

//     if (firstMouse)
//     {
//         preX = xpos;
//         preY = ypos;
//         firstMouse = false;
//     }

//     float xoffset = xpos - preX;
//     float yoffset = preY - ypos; // reversed since y-coordinates go from bottom to top

//     preX = xpos;
//     preY = ypos;

//     camera.processMouseMove(xoffset, yoffset);
// }

// void keyboard_callback(Window* window, double deltaTime)
// {
//     if (inputKeyPressed(window, KEY_A)) {
//         camera.processKeyboard(LEFT, deltaTime);
//     }
//     if (inputKeyPressed(window, KEY_D)) {
//         camera.processKeyboard(RIGHT, deltaTime);
//     }
//     if (inputKeyPressed(window, KEY_W)) {
//         camera.processKeyboard(FORWARD, deltaTime);
//     }
//     if (inputKeyPressed(window, KEY_S)) {
//         camera.processKeyboard(BACKWARD, deltaTime);
//     }
//     // if (inputKeyPressed(window, KEY_Q)) {
//     //     camera.processKeyboard(DOWNWARD, deltaTime);
//     // }
//     // if (inputKeyPressed(window, KEY_E)) {
//     //     camera.processKeyboard(UPWARD, deltaTime);
//     // }
// }

// void scroll_callback(Window* window, double offset)
// {
//     camera.processMouseScroll(offset);
// }
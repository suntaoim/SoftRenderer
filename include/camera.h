#ifndef CAMERA_H
#define CAMERA_H

#include "platform.h"

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const double SPEED = 2.5;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

class Camera {
public:
    Point3 position;
    Vector3 front;
    Vector3 up;
    Vector3 right;
    Vector3 worldup;

    double yaw;
    double pitch;
    double speed;
    double mouseSensitivity;
    float zoom;
public:
    Camera(Point3 _position = Point3(0.0, 0.0, 100.0),
           Vector3 _worldup = Vector3(0.0, 1.0, 0.0),
           double _yaw = YAW,
           double _pitch = PITCH) :
           front(0.0, 0.0, -1.0),
           speed(SPEED),
           mouseSensitivity(SENSITIVITY),
           zoom(ZOOM) {
        position = _position;
        worldup = _worldup;
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }

    void processKeyboard(CameraMovement direction, double deltaTime) {
        double distance = speed * deltaTime;
        if (direction == FORWARD)
            position += front * distance;
        if (direction == BACKWARD)
            position -= front * distance;
        if (direction == RIGHT)
            position += right * distance;
        if (direction == LEFT)
            position -= right * distance;
        if (direction == UPWARD)
            position += worldup * distance;
        if (direction == DOWNWARD)
            position -= worldup * distance;
    }

    void processMouseMove(double xoffset, double yoffset,
    bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(double yoffset) {
        zoom -= yoffset;
        if (zoom < 1.0) {
            zoom = 1.0;
        }
        if (zoom > 45.0) {
            zoom = 45.0;
        }
    }
private:
    void updateCameraVectors() {
        Vector3 f;
        f.x = cos(radians(pitch)) * cos(radians(yaw));
        f.y = sin(radians(pitch));
        f.z = cos(radians(pitch)) * sin(radians(yaw));
        front = normalize(f);
        right = normalize(cross(front, worldup));
        up = normalize(cross(right, front));
    }
};

#endif
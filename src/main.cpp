#include <vector>
#include <algorithm>
#include "tgaimage.h"
#include "model.h"
#include "vector.h"
#include "vec2.h"
#include "matrix.h"
#include "geometry.h"

using std::min;
using std::max;
using std::cout;
using std::endl;

const int width  = 800;
const int height = 800;
const int depth = 255;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor green = TGAColor(  0, 255,   0, 255);

Model* model = nullptr;
double* zbuffer = nullptr;
Vector3 camera(1.0, 1.0, 3.0);
Vector3 center(0.0, 0.0, 0.0);
Vector3 light = unit_vector(Vector3(1.0, -1.0, 1.0));

// Bresenham's line drawing algorithm
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = 2 * std::abs(dy);
    int error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error += derror;
        if (error > dx) {
            y += (y0 < y1) ? 1 : -1;
            error -= 2 * dx;
        }
    }
}

Vector3 getBarycentricCoord(Vector3 triangle[], vec2 p) {
    Vector3 coord = cross(Vector3(triangle[1].x() - triangle[0].x(),
                                  triangle[2].x() - triangle[0].x(),
                                  triangle[0].x() - p.x()),
                          Vector3(triangle[1].y() - triangle[0].y(),
                                  triangle[2].y() - triangle[0].y(),
                                  triangle[0].y() - p.y()));
    double u = coord.x() / coord.z();
    double v = coord.y() / coord.z();
    return Vector3(u, v, 1.0 - u - v);
}

// Screen space rasterization
void rasterizeTriangle(Vector3 triangle[], double zbuffer[], TGAImage& image, TGAColor color) {
    // Find out the bounding box of current triangle.
    double xmin = static_cast<double>(image.width() - 1);
    double ymin = static_cast<double>(image.height() - 1);
    double xmax = 0, ymax = 0;
    for (int i = 0; i < 3; i++) {
        xmin = min(xmin, triangle[i].x());
        ymin = min(ymin, triangle[i].y());
        xmax = max(xmax, triangle[i].x());
        ymax = max(ymax, triangle[i].y());
    }
    xmin = max(xmin, 0.0);
    ymin = max(ymin, 0.0);
    xmax = min(xmax, static_cast<double>(image.width() - 1));
    ymax = min(ymax, static_cast<double>(image.height() - 1));

    for (int x = xmin; x <= xmax; x++) {
        for (int y = ymin; y <= ymax; y++) {
            Vector3 coord = getBarycentricCoord(triangle, vec2(x, y));
            if (coord.x() < 0.0 || coord.y() < 0.0 || coord.z() < 0.0) {
                continue;
            }
            // cout << "check point 1" << endl;
            double z = coord.x() * triangle[0].z() +
                       coord.y() * triangle[1].z() +
                       coord.z() * triangle[2].z();
            int index = y * width + x;
            // Camera gaze at -z
            if (zbuffer[index] < z) {
                // cout << "check point 2" << endl;
                zbuffer[index] = z;
                image.set(x, y, color);
            }
        }
    }
}

Vector3 world2screen(Vector3 v) {
    return Vector3((v.x() + 1.0) * width / 2.0 + 0.5,
                   (v.y() + 1.0) * height / 2.0 + 0.5,
                   v.z());
}

int main(int argc, char** argv) {
    if (argc == 2) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../obj/african_head.obj");
    }

    zbuffer = new double[width * height];
    std::fill(zbuffer, zbuffer + width * height,
        -10000);

    Matrix4 view = getViewMatrix(center, camera, Vector3(0.0, 1.0, 0.0));
    Matrix4 projection = getPerspectiveProjectionMatrix(-1, 1, -1, 1, 1, -1);
    Matrix4 viewport = getViewportMatrix(0, 0, width, height);

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vector3 worldCoords[3];
        Vector3 screenCoords[3];
        // double intensity[3];
        for (int j = 0; j < 3; j++) {
            Vector3 v = model->vert(face[j]);
            worldCoords[j] = v;
            Vector4 v4 = viewport * homodiv(projection * view * Vector4(v));
            screenCoords[j] = Vector3(v4[0], v4[1], v4[2]);
            // screenCoords[j] = world2screen(v);
        }
        rasterizeTriangle(screenCoords, zbuffer, image,
            TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }

    // image.flip_vertically();
    image.write_tga_file("../images/5_moving_camera.tga");
    delete model;

    // Matrix4 a, b;
    // a << 1,2, 3, 4,
    //      5,   6, 7, 8,
    //      9, 10 , 11, 12, 13,
    //      14,15,   16;
    // // b << 1, 1, 1, 1,
    // //     2, 1, 2, 2,
    // //     3, 1, 3, 3,
    // //     4, 1, 4, 4;
    // Vector4 c(1.0, 1.0, 1.0, 1.0);
    // std::cout << a;
    // // std::cout << b;
    // // std::cout << a * b;
    // cout << c;
    // cout << a * c;

    return 0;
}
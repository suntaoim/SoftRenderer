#include <algorithm>
#include "tgaimage.h"
#include "model.h"
// #include "vec3.h"
#include "vec2.h"

using std::min;
using std::max;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor green = TGAColor(  0, 255,   0, 255);
Model* model = nullptr;
const int width  = 800;
const int height = 800;

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

vec3 getBarycentricCoord(vec2* triangle, vec2 p) {
    vec3 coord = cross(vec3(triangle[1].x() - triangle[0].x(),
                             triangle[2].x() - triangle[0].x(),
                             triangle[0].x() - p.x()),
                        vec3(triangle[1].y() - triangle[0].y(),
                             triangle[2].y() - triangle[0].y(),
                             triangle[0].y() - p.y()));
    double u = coord.x() / coord.z();
    double v = coord.y() / coord.z();
    return vec3(u, v, 1.0 - u - v);
}

void rasterizeTriangle(vec2* triangle, TGAImage& image, TGAColor color) {
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
            vec3 coord = getBarycentricCoord(triangle, vec2(x, y));
            if (coord.x() < 0.0 || coord.y() < 0.0 || coord.z() < 0.0) {
                continue;
            }
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    if (argc == 2) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        vec2 triangle[3];
        for (int j = 0; j < 3; j++) {
            // Vec3d v0 = model->vert(face[j]);
            // vec3 coord(v0.x, v0.y, v0.z);
            vec3 coord = model->vert(face[j]);
            triangle[j] = vec2((coord.x() + 1.0) * width / 2.0,
                (coord.y() + 1.0) * height / 2.0);
        }
        rasterizeTriangle(triangle, image,
            TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }

    // image.flip_vertically();
    image.write_tga_file("../images/output.tga");
    delete model;

    return 0;
}
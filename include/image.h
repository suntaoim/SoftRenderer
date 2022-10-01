#ifndef IMAGE_H
#define IMAGE_H

#include "vector.h"
#include "stb_image.h"

typedef enum {
    FORMAT_LDR,
    FORMAT_HDR
} format_t;

typedef struct {
    format_t format;
    int width, height, channels;
    unsigned char *ldr_buffer;
    float *hdr_buffer;
} image_t;

/* image creating/releasing */
image_t *image_create(int width, int height, int channels, format_t format);
void image_release(image_t *image);
image_t *image_load(const char *filename);
void image_save(image_t *image, const char *filename);

/* image processing */
void image_flip_h(image_t *image);
void image_flip_v(image_t *image);

class Image {
private:
    unsigned char* data;
    int width, height;
    int bytesPerLine;
    const double colorScale = 1.0 / 255.0;
public:
    const static int bytesPerPixel = 3;

    Image() : data(nullptr), width(0), height(0), bytesPerLine(0) {}
    Image(const char* filepath);
    ~Image() {delete data;}
    Vector3 sample(double u, double v) const;
};

#endif
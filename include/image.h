#ifndef IMAGE_H
#define IMAGE_H

#include "vector.h"
#include "stb_image.h"

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

typedef enum {
    FORMAT_LDR,
    FORMAT_HDR
} Format;

typedef struct {
    Format format;
    int width, height, channels;
    unsigned char *ldrBuffer;
    float *hdrBuffer;
} image_t;

/* image creating/releasing */
image_t *imageCreate(int width, int height, int channels, Format format);
void imageRelease(image_t *image);
image_t *imageLoad(const char *filename);
void imageSave(image_t *image, const char *filename);

/* image processing */
void imageFlipH(image_t *image);
void imageFlipV(image_t *image);

#endif
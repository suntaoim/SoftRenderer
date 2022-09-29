#ifndef PRIVATE_H
#define PRIVATE_H

#include "graphics.h"
#include "image.h"

/* framebuffer blitting */
void private_blit_bgr(Framebuffer *source, image_t *target);
void private_blit_rgb(Framebuffer *source, image_t *target);

/* misc functions */
const char *private_get_extension(const char *filename);

#endif

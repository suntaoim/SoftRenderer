#ifndef PRIVATE_H
#define PRIVATE_H

#include "graphics.h"
#include "image.h"

/* framebuffer blitting */
void privateBlitBgr(Framebuffer *source, image_t *target);
void privateBlitRgb(Framebuffer *source, image_t *target);

/* misc functions */
const char *privateGetExtension(const char *filename);

#endif

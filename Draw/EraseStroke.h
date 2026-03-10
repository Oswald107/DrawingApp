#ifndef ERASE_STROKE_H
#define ERASE_STROKE_H

#include "Stroke.h"

class EraseStroke : public Stroke {
public:
    EraseStroke();
    virtual ~EraseStroke();
    void draw(GLuint VBO) override;
};

#endif
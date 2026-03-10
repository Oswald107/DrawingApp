#ifndef LINE_STROKE_H
#define LINE_STROKE_H

#include "Stroke.h"

class LineStroke : public Stroke {
public:
    LineStroke();
    virtual ~LineStroke();
    void draw(GLuint VBO) override;
};

#endif
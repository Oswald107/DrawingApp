#include"Stroke.h"

class LineStroke : public Stroke {

public:
    void draw(GLuint VBO) override {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
    }
};
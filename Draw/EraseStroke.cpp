#include"EraseStroke.h"


EraseStroke::EraseStroke() {}

EraseStroke::~EraseStroke() {};

void EraseStroke::draw(GLuint VBO) {
    glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());


    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}
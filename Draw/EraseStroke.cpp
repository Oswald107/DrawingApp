//#include"Stroke.h"
//
//class EraseStroke : public Stroke {
//
//public:
//    void draw(GLuint VBO) override {
//        glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
//    }
//};
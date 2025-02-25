#include "physicsDebugRenderer.h"

PhysicsDebugRenderer::PhysicsDebugRenderer(RenderTexture* renderTexture, Shader* lineShader, Shader* triangleShader, CameraComponent* camera, rp3d::DebugRenderer* debugRenderer) : renderTexture(renderTexture), lineShader(lineShader), triangleShader(triangleShader), camera(camera), debugRenderer(debugRenderer) {

}

void PhysicsDebugRenderer::draw() { 
    renderTexture->bind();
    
    // I think these 2 functions actually do the exact same thing besides the input array and GL_LINES vs GL_TRIANGLES
    if (debugRenderer->getNbLines() > 0) {
        drawLines();
    }
    if (debugRenderer->getNbTriangles() > 0) {
        drawTriangles();
    }

    renderTexture->unbind();
}

void PhysicsDebugRenderer::drawLines() { 
    // setup
    GLuint VAO, VBO;
    // DebugLine is formatted as V3 pt1, U32 clr1, V3 pt2, U32 clr2
    const rp3d::Array<rp3d::DebugRenderer::DebugLine> lines = debugRenderer->getLines();
    rp3d::uint32 color1 = lines[0].color1;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rp3d::DebugRenderer::DebugLine) * lines.size(), &lines[0], GL_STATIC_DRAW);

    const size_t stride = sizeof(rp3d::DebugRenderer::DebugLine) / 2;
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)sizeof(rp3d::Vector3));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // draw
    Mat4 mvp = camera->getProjection() * camera->getView();
    lineShader->use();
    lineShader->setUniform("mvp", mvp.getData());
    glBindVertexArray(VAO);
    glLineWidth(5.0f); // Only width 1 is supported on mac
    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_LINES, 0, lines.size() * 2);
    glLineWidth(1.0f);
    // glEnable(GL_CULL_FACE);

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void PhysicsDebugRenderer::drawTriangles() { 
    // DebugLine is formatted as V3 pt1, U32 clr1, V3 pt2, U32 clr2, V3 pt3, U32 clr3
    const rp3d::Array<rp3d::DebugRenderer::DebugTriangle> tris = debugRenderer->getTriangles();
    rp3d::uint32 color1 = tris[0].color1;

    // setup
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rp3d::DebugRenderer::DebugTriangle) * tris.size(), &tris[0], GL_STATIC_DRAW);

    const size_t stride = sizeof(rp3d::DebugRenderer::DebugTriangle) / 3;
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)sizeof(rp3d::Vector3));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // draw
    Mat4 mvp = camera->getProjection() * camera->getView();
    triangleShader->use();
    triangleShader->setUniform("mvp", mvp.getData());
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5.0f);
    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0, tris.size() * 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);
    // glEnable(GL_CULL_FACE);

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

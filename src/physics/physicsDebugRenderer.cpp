#include "physicsDebugRenderer.h"

PhysicsDebugRenderer::PhysicsDebugRenderer(RenderTexture* renderTexture, Shader* lineShader, Shader* triangleShader, CameraComponent* camera) : renderTexture(renderTexture), lineShader(lineShader), triangleShader(triangleShader), camera(camera) {
    JPH::DebugRenderer::Initialize();
}

void PhysicsDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) { 
    renderTexture->bind();

    // setup
    GLuint VAO, VBO;
    float vertices[] = {
        inFrom.GetX(), inFrom.GetY(), inFrom.GetZ(),
        inTo.GetX(), inTo.GetY(), inTo.GetZ()
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // draw
    Mat4 mvp = camera->getProjection() * camera->getView();
    glUseProgram(lineShader->getID());
    lineShader->setUniform("mvp", mvp.getData());
    lineShader->setVec3("color", {(float)inColor.r, (float)inColor.g, (float)inColor.b});
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    renderTexture->unbind();
}

void PhysicsDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow) { 
    renderTexture->bind();

    // setup
    GLuint VAO, VBO;
    float vertices[] = {
        inV1.GetX(), inV1.GetY(), inV1.GetZ(),
        inV2.GetX(), inV2.GetY(), inV2.GetZ(),
        inV3.GetX(), inV3.GetY(), inV3.GetZ(),
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // draw
    Mat4 mvp = camera->getProjection() * camera->getView();
    lineShader->use();
    lineShader->setUniform("mvp", mvp.getData());
    lineShader->setVec3("color", { (float)inColor.r, (float)inColor.g, (float)inColor.b });
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    renderTexture->unbind();
}

void PhysicsDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view & inString, JPH::ColorArg inColor, float inHeight) {
    renderTexture->bind();

    renderTexture->unbind();
}

JPH::DebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount) {
    BatchImpl* batch = new BatchImpl;
    if (inTriangles == nullptr || inTriangleCount == 0)
        return batch;

    batch->mTriangles.assign(inTriangles, inTriangles + inTriangleCount);
    return batch;
}

JPH::DebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount) {
    BatchImpl* batch = new BatchImpl;
    if (inVertices == nullptr || inVertexCount == 0 || inIndices == nullptr || inIndexCount == 0)
        return batch;

    // Convert indexed triangle list to triangle list
    batch->mVertices.assign(inVertices, inVertices + inVertexCount);
    batch->mIndices.assign(inIndices, inIndices + inIndexCount);

    return batch;
}

void PhysicsDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode) { 
    renderTexture->bind();

    // Figure out which LOD to use
    const LOD* lod = inGeometry->mLODs.data();
    const auto cameraPos = camera->gameObject->transform->getPosition();
    lod = &inGeometry->GetLOD(JPH::Vec3(cameraPos.x(), cameraPos.y(), cameraPos.z()), inWorldSpaceBounds, inLODScaleSq);

    // Draw the batch
    const BatchImpl* batch = static_cast<const BatchImpl*>(lod->mTriangleBatch.GetPtr());
    if (!batch->mTriangles.empty()) {
        // for (const Triangle& triangle : batch->mTriangles) {
            // JPH::RVec3 v0 = inModelMatrix * JPH::Vec3(triangle.mV[0].mPosition);
            // JPH::RVec3 v1 = inModelMatrix * JPH::Vec3(triangle.mV[1].mPosition);
            // JPH::RVec3 v2 = inModelMatrix * JPH::Vec3(triangle.mV[2].mPosition);
            // JPH::Color color = inModelColor * triangle.mV[0].mColor;

            // switch (inDrawMode) {
            //     case EDrawMode::Wireframe:
            //         DrawLine(v0, v1, color);
            //         DrawLine(v1, v2, color);
            //         DrawLine(v2, v0, color);
            //         break;

            //     case EDrawMode::Solid:
            //         DrawTriangle(v0, v1, v2, color, inCastShadow);
            //         break;
            // }
        // }

        // setup
        GLuint VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * batch->mTriangles.size(), &batch->mTriangles[0], GL_STATIC_DRAW);

        // TODO: make sure triangle shader is set up to receive all of this data properly
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
        // uv
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mUV));
        // color ISSUE: COLOR IS STORED AS 4 UINT 8's SO THIS WON'T WORK
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mColor));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // draw
        Mat4 mvp = camera->getProjection() * camera->getView();
        triangleShader->use();
        triangleShader->setUniform("mvp", mvp.getData());
        triangleShader->setVec3("color", { (float)inModelColor.r, (float)inModelColor.g, (float)inModelColor.b });
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, inDrawMode == EDrawMode::Solid ? GL_FILL : GL_LINE);
        // could also disable backface culling based on inCullMode
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // cleanup
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

    } else if (!batch->mVertices.empty() && !batch->mIndices.empty()) {
        // setup
        GLuint VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * batch->mVertices.size(), &batch->mVertices[0], GL_STATIC_DRAW);
        glGenBuffers(1, &EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch->mIndices.size() * sizeof(JPH::uint32), &batch->mIndices[0], GL_STATIC_DRAW);

        // TODO: make sure triangle shader is set up to receive all of this data properly
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
        // uv
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mUV));
        // color ISSUE: COLOR IS STORED AS 4 UINT 8's SO THIS WON'T WORK
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mColor));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // draw
        Mat4 mvp = camera->getProjection() * camera->getView();
        triangleShader->use();
        triangleShader->setUniform("mvp", mvp.getData());
        triangleShader->setVec3("color", { (float)inModelColor.r, (float)inModelColor.g, (float)inModelColor.b });
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, inDrawMode == EDrawMode::Solid ? GL_FILL : GL_LINE);
        // could also disable backface culling based on inCullMode
        glDrawElements(GL_TRIANGLES, batch->mIndices.size(), GL_UNSIGNED_INT, 0);

        // cleanup
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    
    renderTexture->unbind();
}

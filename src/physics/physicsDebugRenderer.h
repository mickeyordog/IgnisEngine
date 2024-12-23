#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <renderTexture.h>
#include <shader.h>
#include <cameraComponent.h>

class PhysicsDebugRenderer : public JPH::DebugRenderer {
public:
    PhysicsDebugRenderer(RenderTexture* renderTexture, Shader* lineShader, Shader* triangleShader, CameraComponent* camera);
    virtual ~PhysicsDebugRenderer() override = default;

    virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;
    virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow = ECastShadow::Off) override;
    virtual void DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view& inString, JPH::ColorArg inColor = JPH::Color::sWhite, float inHeight = 0.5f) override;
    virtual Batch CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount) override;
    virtual Batch CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount) override;
    virtual void DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode = ECullMode::CullBackFace, ECastShadow inCastShadow = ECastShadow::On, EDrawMode inDrawMode = EDrawMode::Solid) override;

private:
    RenderTexture* renderTexture;
    Shader* lineShader;
    Shader* triangleShader;
    CameraComponent* camera;

    /// Implementation specific batch object
    class BatchImpl : public JPH::RefTargetVirtual
    {
    public:
        JPH_OVERRIDE_NEW_DELETE

        virtual void AddRef() override { ++mRefCount; }
        virtual void Release() override { if (--mRefCount == 0) delete this; }

        JPH::Array<Triangle> mTriangles; // Either mTriangles will be used, or mVertices and mIndices
        JPH::Array<Vertex> mVertices;
        JPH::Array<JPH::uint32> mIndices;

    private:
        JPH::atomic<JPH::uint32> mRefCount = 0;
    };
};
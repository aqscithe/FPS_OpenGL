#pragma once
#include "Component.h"
#include "Transform.h"
#include "Application.h"

using namespace Resources;
using namespace Physics;
using namespace Core::API;

namespace LowRenderer
{
    class Camera : public Component
    {
    public :
        Camera();

        float yaw;
        float pitch;
        float roll;
        float MOUSE_SENSITIVITY = 2.f;

        void start() override;
        void update() override;
        void fixedUpdate() override {}
        bool updateIMGUI() override;
        void save(std::ofstream& sceneFile) override;

        bool getOrthographic();
        mat4 getViewMatrix();
        mat4 getProjection();
        mat4 getViewProj();
        vec3 getPosition();

        float getYaw() const;

        void setOrthographic(bool value) { isOrthographic = value; }
        void setEnabled(bool value) { enabled = value; }
        void setPitch(float pitch_) { pitch = pitch_; }
        void setYaw(float yaw_) { yaw = yaw_; }
        void setRoll(float roll_) { roll = roll_; }
        void setAspect(float aspect_) { aspect = aspect_; }
        void setFovY(float fovY_) { fovY = fovY_; }
        void setNear(float near_) { near = near_; }
        void setFar(float far_) { far = far_; }

        void setGameCamera();

    private:
        Physics::Transform* transform;
        Application* api;

        bool isOrthographic;
        bool enabled;

        float editYaw;
        float editPitch;
        float aspect;
        float fovY;
        float near;
        float far;
        vec3 position;

        
    };

    class CameraManager //singleton
    {
    public :
        CameraManager() : activeCamera(nullptr), gameCamera(nullptr), editorCamera(new Camera) {}
        ~CameraManager();

        static CameraManager* getInstance();

        Camera* getActiveCamera();
        void setGameCamera(Camera* camera) { gameCamera = camera; }
        void setActiveCamera();
        void updateEditorCamera();

    private :
        Camera* activeCamera;
        Camera* gameCamera;
        Camera* editorCamera;
        bool cameraLocked = true;
        static CameraManager* instance;
    };
}

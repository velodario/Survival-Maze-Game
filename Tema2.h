#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/lab_camera.h"
//#include "lab_m1/Tema2/mazed.h"


using namespace std;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

       

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int object);
        bool isCollided_perete_inamic(int playerX, int playerZ, int pereteX, int pereteZ);
        void atackMode(float deltaSpeed, float x, float z);
        void setCamera(int sign);
        void generatemazed();
        void CreateJucator(float deltaTimeSeconds);
        void initMeshes();
        void init_mazed();
        void mazed_generator(int index, int upfront_x[1600], int upfront_y[1600], int x, int y, int n, int visited);
        int is_closed(int x, int y);
        void creare_inamic(int a, int b, int inamic, int mazed_dimens);
        void visualize_maze(int mazed_dimens);

    protected:
        implemented::Camera1* camera;
        glm::mat4 projectionMatrix, projectionMatrixOrtho;
        bool renderCameraTarget;
        glm::ivec2 resolution = window->GetResolution();
        float left = -resolution.x * 0.005f;
        float right = resolution.x * 0.005f;
        float bottom = -resolution.y * 0.005f;
        float top = resolution.y * 0.005f;
        float zNear = -100.0f, zFar = 200.0f;
        float fov = 60, changeX = 0, changeY = 0;
        float constx, consty, rotate = 0, rotate_inamic = 0, translateX = 0;
        float tx, ty, sphereY, tz, inamX, inamZ;
        float scale;
        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        int d, dimm, start = 1, remaining_lives = 3;
        std::vector<glm::vec3>walls; std::vector<glm::vec3> floors, inamici, collision_inamic;
        std::vector<int> count;
        int MAZED = 81;  // 40 * 2 + 1
        int CELLA = 1600;  // 40^2
        int WALL = 1;
        int EMPTY = 0;
        glm::vec3 startposition = glm::vec3(1) , endposition, ataced, transl, transl2, cam;
        int atac, mouseX, mouseY;
        int camX, camY, camZ, movement = 1;
        float a, b;
        int dimens, index = 0, collision = 0;
        int mazed[81][81];
        int upfront_x[1600], upfront_y[1600];
        int nr_exits;
        float rotateAngle = .00f;
    };
}   // namespace m1

#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include<stdlib.h>
#include <cmath>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;
   

    camera = new implemented::Camera1();
   
    generatemazed();
    
    initMeshes();
  
   
}

void Tema2::initMeshes() {
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        lightPosition = glm::vec3(0, 2, 0);
        materialShininess = 50;
        materialKd = 0.8;
        materialKs = 0.8;
    }

    constx = 2;
    consty = 0.5;
    tz = 0;
    ty = 0;
    tx = 0;
    sphereY = 0;
    scale = 0.5f;
    atac = 0;

    projectionMatrix = glm::perspective(RADIANS(90), window->props.aspectRatio, 0.01f, 100.0f);


}

void Tema2::setCamera(int sign) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::mat4(1); 
    transl2 = glm::vec3(startposition.x + tx - 3, startposition.y + ty + 0.5, startposition.z + tz + 4);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(4)); 
    modelMatrix = glm::translate(modelMatrix, transl2);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2, 0));
   //coordonatele corpului de jucator
   //modelmatrix[3] contine ultimele rezultate de scalare si translatie
    
        camera->Set(glm::vec3(modelMatrix[3].x - 5, modelMatrix[3].y + 4, modelMatrix[3].z + tz), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
   
   
}

void Tema2::generatemazed() {
    //int mazed[dimens][dimens];
   srand((unsigned)time(NULL));

    
    cout << "Creating maze\n\n";
    cout << " 0 < input < 40 : ";
    dimens = 8; //dimensiunea gridului
    cout << endl;
    nr_exits = (rand() % 2) + 1; //de la 1 la 2, 1 o iesire, 2 doi iesiri

    init_mazed();

    upfront_x[index] = 1; 
    upfront_y[index] = 1;

    mazed_generator(index, upfront_x, upfront_y, 1, 1, dimens, 1);
    visualize_maze(dimens);

   // getchar();
}


void Tema2::FrameStart()
{
    
   
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);


    int seconds;

    seconds = glfwGetTime();
    printf("\n%d\n", seconds);
    if (seconds == 60) { //Timp doar de 1 minut
        printf("TIME'S UP!");
        Exit();
    }
    
   
    

}



void Tema2::Update(float deltaTimeSeconds)
{
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.04, 0));
      
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0.5, 1, 0.5),0);
    } //podeaua

    // Render the point light in the scene
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
     //   RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
    }

    

    
    d = dimens / 2; //distanta ca sa incapa pe plan tot labirintul
   
    for (int i = 0; i < dimens * 2 + 1; i++)
    {
        for (int j = 0; j < dimens * 2 + 1; j++)
        {

            float pCx = -0.5 + j;
            float pCz = d - 0.5 - i;
            float pCy = 0.0f;
            if (mazed[i][j] == 1) { //daca e 1 e perete

                walls.push_back(glm::vec3(pCx, pCy, pCz));
            }
             if (mazed[i][j] == 5) //daca 5, pozitia de start
            {
                startposition = glm::vec3(pCx, pCy, pCz);
       
            }
          
             if (mazed[i][j] == 2){ //2 - inamic
                inamici.push_back(glm::vec3(pCx, pCy, pCz));
            }
        }
    }
   
    
    CreateJucator(deltaTimeSeconds);
  //  setCamera(0);
    
    printf("\nremaining lives %d\n", remaining_lives);
    
}

void Tema2::CreateJucator(float deltaTimeSeconds) {
    glm::mat4 modelMatrix = glm::mat4(1); //body
    transl = glm::vec3(startposition.x + tx - 3, startposition.y + ty, startposition.z + tz + 4); //coordonatele orientative pt fiecare parte a jucatorului
    modelMatrix = glm::scale(modelMatrix, glm::vec3(4)); //scalare cu 4 la fiecare parte a jucatorului ca sa fie in plane
    modelMatrix = glm::translate(modelMatrix, transl); 
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f)); //scalez cat vreau dupa ce am translatat pe pozitia corecta
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.7, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(rotate), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 0), 0);

    if (atac == 1) { //daca am apasat mouse left, lansez proiectile
        atackMode(1, startposition.x + tx - 3, startposition.z + tz + 4); //proiectile ies din corpul jucatorului
       

    }
    if (atac == 2) { //daca mai apas o data, dispar proiectile
        atac = 0;
        sphereY = 0;

    }
    
    

   

    for (glm::vec3 position : walls)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4)); //scalez cu 4 ca sa fie pe plane
        modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x-3, position.y, position.z+4)); //translatez asa cum sunt in matrice
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1),0);
        if (isCollided_perete_inamic(startposition.x, startposition.z, position.x, position.z)) { //daca colizune perete inamic
            tx += -1; //jucatorul se intoarce un pic behind
           
        }
        if (startposition.x + tx + sphereY < position.x + 1 &&
            startposition.x + tx + sphereY + 0.4> position.x && startposition.z + tz < position.z + 1 &&
            startposition.z + 0.4 + tz  > position.z) { //coliziune proiectil perete
            //in cazul acesta proiectilele au scalare 1.6 iar peretele 1 dar am redus scalarea proiectilelor ca sa fie mai buna coliziunea

            atac = 0; //dispare coliziunea
            sphereY = 0;
        }

    }
    
   

   
    if (translateX > 1.5) { 
        movement = -movement; //recusivitate 0-1.5 pentru miscarea inamicilor
    }

    if (translateX < 0) {
        movement = -movement;
    }
    translateX += movement * deltaTimeSeconds; 
    for (glm::vec3 position: inamici)
    {
      
        //collision inamic pastreaza coordonatele inamicilor ucisi, asa fel incat ei sa dispare din scena
        if (std::find(collision_inamic.begin(), collision_inamic.end(), position) != collision_inamic.end()) {
                continue;  
           
        }
            if (startposition.x + tx + sphereY < position.x + 1.5 &&
                startposition.x + tx + sphereY+  0.7> position.x  && startposition.z + tz < position.z + 1.5 &&
                startposition.z + 0.7 + tz  > position.z) { 
                //coliziune proiectil/jucator - inamic

                printf("shot");
                atac = 0;
                sphereY = 0;
                remaining_lives--;
                collision_inamic.push_back(position); //exista coliziune, pastram coord inamicului
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x - 3, position.y, position.z + 4));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.5, 0), 1); //activam deformarea == 1
                printf("crash, remaining lives %d\n", remaining_lives);
                

                    continue;    
                
               
            }
            //daca nu exista coliziune, render inamici
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x - 3 , position.y + translateX, position.z + 4 )); 
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));   
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.5, 0), 0);
            
       
    }

    if (remaining_lives == 0) { //0 vieti ramase = exit program
        Exit();
    }
    
    


    {
        modelMatrix = glm::mat4(1); //capul jucatorului
        transl2 = glm::vec3(startposition.x + tx - 3, startposition.y + ty, startposition.z + tz + 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4)); //scalez cu 4 pentru plane
        modelMatrix = glm::translate(modelMatrix, transl2); //translatez pe plane
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f)); //aranjez capul ca sa corespunde cu celelalte parti
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.7, 0));

         modelMatrix = glm::rotate(modelMatrix, RADIANS(rotate), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0),0);
    }

    {
        modelMatrix = glm::mat4(1); //mana dreapta
        transl2 = glm::vec3(startposition.x + tx - 3, startposition.y + ty, startposition.z + tz + 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));

        modelMatrix = glm::translate(modelMatrix, transl2);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5, 1.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rotate), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0), 0);
    }

    {
        modelMatrix = glm::mat4(1); //mana stanga
        transl2 = glm::vec3(startposition.x + tx - 3, startposition.y + ty, startposition.z + tz + 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
        modelMatrix = glm::translate(modelMatrix, transl2);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.2, -1.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rotate), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0), 0);
    }
    {
        modelMatrix = glm::mat4(1); //piciorul stang
        transl2 = glm::vec3(startposition.x + tx - 3, startposition.y + ty, startposition.z + tz + 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
        modelMatrix = glm::translate(modelMatrix, transl2);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -0.7));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rotate), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.7, 0.3), 0);
    }

    {
        modelMatrix = glm::mat4(1); //piciorul drept
        transl2 = glm::vec3(startposition.x + tx - 3, startposition.y + ty, startposition.z + tz + 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
        modelMatrix = glm::translate(modelMatrix, transl2);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0.7));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rotate), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.7, 0.3), 0);
    }
}
    void Tema2::atackMode(float deltaSpeed, float x, float z) {
        sphereY += 2 * deltaSpeed; //viteza proiectilului
        ataced = glm::vec3(startposition.x + tx - 3 + sphereY, startposition.y + rotate, startposition.z + tz + 4);
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
            modelMatrix = glm::translate(modelMatrix, ataced);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0.5,0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(rotate), glm::vec3(1, 0, 0));

            RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 1), 0);
            

        }
   
    }

    

bool Tema2::isCollided_perete_inamic(int playerX, int playerZ, int pereteX, int pereteZ) {
    if (playerX + tx < pereteX + 1 &&
        playerX + tx + 0.6> pereteX && playerZ + tz < pereteZ + 1 &&
        playerZ + tz + 0.6  > pereteZ) { 
        //coliziune AABB
            
            return true;
        }
    return false;
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int object)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);
    //daca object == 1 inseamna ca avem deformare
    if (object == 1) {
        GLint deformation = glGetUniformLocation(shader->GetProgramID(), "deformation");
        glUniform1i(deformation, 1);
    }
    else {
        GLint deformation = glGetUniformLocation(shader->GetProgramID(), "deformation");
        glUniform1i(deformation, 0);
    }


        // Set shader uniforms for light & material properties
        // TODO(student): Set light position uniform
        glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 1, glm::value_ptr(lightPosition));

        glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
        //std::cout << glm::to_string(eyePosition) << std::endl;
        // TODO(student): Set eye position (camera position) uniform
        glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

        // TODO(student): Set material property uniforms (shininess, kd, ks, object color)
        glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess); //e int
        glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
        glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
        glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

        // Bind model matrix
        GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
        glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // Bind view matrix
        glm::mat4 viewMatrix = camera->GetViewMatrix();
        int loc_view_matrix = glGetUniformLocation(shader->program, "View");
        glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // Bind projection matrix
       // glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
        int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
        glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
     

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::FrameEnd()
{
   // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}





/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    float cameraSpeed = 2.5f;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
       

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->TranslateForward(0.10 * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }
    if (window->KeyHold(GLFW_KEY_SPACE))
    {
        fov += deltaTime;
        changeX += deltaTime;
        changeY += deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_MINUS))
    {
        fov -= deltaTime;
        changeX -= deltaTime;
        changeY -= deltaTime;

    }

    if (window->KeyHold(GLFW_KEY_UP))
    {
        tx += deltaTime * cameraSpeed;
     

    }
    if (window->KeyHold(GLFW_KEY_DOWN))
    {
        tx -= deltaTime * cameraSpeed;
     

    }
    if (window->KeyHold(GLFW_KEY_LEFT))
    {
        tz -= deltaTime * cameraSpeed;
    

    }
    if (window->KeyHold(GLFW_KEY_RIGHT))
    {
        tz += deltaTime * cameraSpeed;
    

    }
    if (window->KeyHold(GLFW_KEY_RIGHT_SHIFT))
    {
        rotate += deltaTime * 50;
      

    }
    




}


void Tema2::OnKeyPress(int key, int mods)
{
    glm::ivec2 resolution = window->GetResolution();

    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    if (key == GLFW_KEY_O)
    {

        projectionMatrix = glm::ortho(left - changeX, right + changeX, bottom - changeY, top + changeY, zNear, zFar);
    }
    if (key == GLFW_KEY_P)
    {
        //setCamera();
        glm::mat4 modelMatrix = glm::mat4(1); //capul
        transl2 = glm::vec3(startposition.x + tx - 3, startposition.y + ty + 0.5, startposition.z + tz + 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4)); 
        modelMatrix = glm::translate(modelMatrix, transl2);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2, 0));

        //directionez camera pe pozitia capului jucatorului, un pic mai in spate
        camera->Set(glm::vec3(modelMatrix[3].x - 5, modelMatrix[3].y + 4, modelMatrix[3].z + tz), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
    }
    
    

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
           
            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }
    }
}




void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {

        atac++; //proiectilul e activat
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetz, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

void Tema2::init_mazed()
{
    for (int a = 0; a < MAZED; a++)
    {
        for (int b = 0; b < MAZED; b++)
        {
            if (a % 2 == 0 || b % 2 == 0) //daca linia sau coloana e para, punem perete
                mazed[a][b] = WALL;
            else
                mazed[a][b] = EMPTY; //la impare punem 0, drum liber
        }
    }
}

// determina daca celula inchisa
int Tema2::is_closed(int x, int y)
{
    if (mazed[x - 1][y] == WALL && mazed[x][y - 1] == WALL
        && mazed[x][y + 1] == WALL && mazed[x + 1][y] == WALL) return 1;

    return 0;
}
// generarea de drum liber aleatoriu
void Tema2::mazed_generator(int index, int upfront_x[1600], int upfront_y[1600], int x, int y, int n, int visited)
{
    if (visited < n * n) //daca nu trece dimensiunea
    {
        int valid_neighbor = -1;
        int neighbor_x[4], neighbor_y[4], step[4];

        int x_next;
        int y_next;

        if (x - 2 > 0 && is_closed(x - 2, y))  // daca e drum liber sus
        {
            valid_neighbor++;
            neighbor_x[valid_neighbor] = x - 2;
            neighbor_y[valid_neighbor] = y;
            step[valid_neighbor] = 1;
        }

        if (y - 2 > 0 && is_closed(x, y - 2))  //daca e drum liber stanga
        {
            valid_neighbor++;
            neighbor_x[valid_neighbor] = x;
            neighbor_y[valid_neighbor] = y - 2;
            step[valid_neighbor] = 2;
        }

        if (y + 2 < n * 2 + 1 && is_closed(x, y + 2))  //daca e drum liber dreapta
        {
            valid_neighbor++;
            neighbor_x[valid_neighbor] = x;
            neighbor_y[valid_neighbor] = y + 2;
            step[valid_neighbor] = 3;

        }

        if (x + 2 < n * 2 + 1 && is_closed(x + 2, y))  // daca e drum liber jos
        {
            valid_neighbor++;
            neighbor_x[valid_neighbor] = x + 2;
            neighbor_y[valid_neighbor] = y;
            step[valid_neighbor] = 4;
        }

        if (valid_neighbor == -1)
        {
            // drum ignorat
            x_next = upfront_x[index];
            y_next = upfront_y[index];
            index--;
        }

        if (valid_neighbor != -1)
        {
          //randomizam de la 0 la 4 la urmatorul drum
            int randomize = valid_neighbor + 1, random = rand() % randomize;
            x_next = neighbor_x[random];
            y_next = neighbor_y[random];
            index++;
            upfront_x[index] = x_next;
            upfront_y[index] = y_next;

            int rstep = step[random];

            //punem pe matrix
            if (rstep == 1)
                mazed[x_next + 1][y_next] = EMPTY;
            else if (rstep == 2)
                mazed[x_next][y_next + 1] = EMPTY;
            else if (rstep == 3)
                mazed[x_next][y_next - 1] = EMPTY;
            else if (rstep == 4)
                mazed[x_next - 1][y_next] = EMPTY;
            visited++;
        }

        // recursivitate
        //acum si indexul urmator va fi aleator
        mazed_generator(index, upfront_x, upfront_y, x_next, y_next, n, visited);
    }
}

void Tema2::creare_inamic(int a, int b, int inamic, int mazed_dimens) {
    if (inamic == 0) { //daca e 0, drum liber
        mazed[a][b] = EMPTY;
    }
    if(inamic == 2) { //daca e 2, inamic
        mazed[a][b] = 2;
    }
   


}


void Tema2::visualize_maze(int mazed_dimens)
{
    int inamic_random;
    
    for (int a = 0; a < mazed_dimens * 2 + 1; a++)
    {
        for (int b = 0; b < mazed_dimens * 2 + 1; b++)
        {
            if (mazed[a][b] == EMPTY) { //daca drum liber
               
                inamic_random = rand() % 5; //de la 0 la 5. daca 0 nu e inamic, daca 2 inamic
                
                creare_inamic(a, b, inamic_random, mazed_dimens);
            }
        }
      
    }
    for (int a = 0; a < mazed_dimens * 2 + 1; a++)
    {
        mazed[a][0] = 0; //am lasat goala linia unde este pozitional jucatorul
    }

    
    if (nr_exits == 1) { // o iesire
        mazed[mazed_dimens * 2 - 1][mazed_dimens * 2] = 0;
    }
    if (nr_exits == 2) { //2 iesiri
        mazed[mazed_dimens * 2 - 1][mazed_dimens * 2] = 0;
        mazed[1][mazed_dimens * 2] = 0;
    }
    //int jucator_initial = rand() % (mazed_dimens * 2) + 0;
   // if (mazed[jucator_initial][0] == 0) mazed[jucator_initial][0] = 5;
   // else 
    mazed[mazed_dimens * 2][0] = 5;

    for (int a = 0; a < mazed_dimens * 2 + 1; a++)
    {
        for (int b = 0; b < mazed_dimens * 2 + 1; b++)
        {
            printf("%d", mazed[a][b]);
        }
        printf("\n");
    }


}

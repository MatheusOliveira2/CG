// main.cpp - transform animation example

// V-ART example: Moving objects with an idle handler

// This application shows how to use ViewerGlutOGL's idle handler to animate objects.

// Changelog
// Jun 17, 2008 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>

using namespace std;

#define PI 3.14159265

float convert(float grau){
    float rad = PI/180;
    return rad*grau;
}

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : redRadians(0), terraTranslation(0), luaTranslation(0){}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            redRadians += 0.01;
            terraTranslation += 1;
            luaTranslation += 2;
            solRot->MakeZRotation(redRadians);
            terraRot->MakeZRotation(redRadians*2);
            float x = convert(terraTranslation);
            float y = convert(terraTranslation);
            terraTrans->MakeTranslation(cos(x)*10,sin(y)*10,0);
            luaRot->MakeZRotation(redRadians);
            luaTrans->MakeTranslation(cos(convert(luaTranslation))*4,sin(convert(luaTranslation))*4,0);
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* solRot;
        VART::Transform* terraTrans;
        VART::Transform* terraRot;
        VART::Transform* luaRot;
        VART::Transform* luaTrans;
    private:
        float redRadians;
        float terraTranslation;
        float luaTranslation;
};

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects
    VART::Texture texture;
    VART::Material material;
   
   //Sol
    VART::Transform solRotation;
    VART::Sphere sol;
    sol.SetRadius(5);
    solRotation.MakeIdentity();
    MyIHClass idleHandler;

    //Terra
    VART::Sphere terra;
    terra.SetRadius(2);
    VART::Transform terraTranslation;
    VART::Transform terraRotation;
    terraRotation.MakeIdentity();

    //Lua
    VART::Sphere lua;
    lua.SetRadius(1);
    VART::Transform luaTranslation;
    VART::Transform luaRotation;
    luaRotation.MakeIdentity();

    
    // Textura Sol
    if(texture.LoadFromFile("sun.gif")){
        material.SetTexture(texture);
        sol.SetMaterial(material);
        sol.SetDescription("Sol");
        solRotation.AddChild(sol);
    }
    else
    {
        cout << "Could not open texture file. Check if the image format is supported, "
             << "if it is in the correct path, and if V-Art was compiled with OpenGL "
             << "support and an image reader library." << std::endl;
    }

    terraTranslation.MakeTranslation(10,0,0);
    if(texture.LoadFromFile("earth.jpg")){
        material.SetTexture(texture);
        terra.SetMaterial(material);
        terra.SetDescription("Terra");
        terraTranslation.AddChild(terraRotation);
        terraRotation.AddChild(terra);
    }
    else
    {
        cout << "Could not open texture file. Check if the image format is supported, "
             << "if it is in the correct path, and if V-Art was compiled with OpenGL "
             << "support and an image reader library." << std::endl;
    }
    luaTranslation.MakeTranslation(3,0,0);
    if(texture.LoadFromFile("moon.png")){
        material.SetTexture(texture);
        lua.SetMaterial(material);
        lua.SetDescription("Lua");
        terraTranslation.AddChild(luaTranslation);
        luaTranslation.AddChild(luaRotation);
        luaRotation.AddChild(lua);
    }
    else
    {
        cout << "Could not open texture file. Check if the image format is supported, "
             << "if it is in the correct path, and if V-Art was compiled with OpenGL "
             << "support and an image reader library." << std::endl;
    }
    // Build up the scene
    scene.AddObject(&solRotation);
    scene.AddObject(&terraTranslation);
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    // Set up the Idle Handler
    idleHandler.solRot = &solRotation;
    idleHandler.terraRot = &terraRotation;
    idleHandler.terraTrans = &terraTranslation;
    idleHandler.luaTrans = &luaTranslation;
    idleHandler.luaRot = &luaRotation;
    //idleHandler.greenRotPtr = &greenSphereRotation;

    // Set up the viewer
    viewer.SetTitle("Sistema Solar");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler); // attach the idle handler to the viewer

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}

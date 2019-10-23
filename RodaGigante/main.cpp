#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/meshobject.h>
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
        MyIHClass() : wheelRad(0){}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            wheelRad+=1;
            wheelRotation->MakeXRotation(convert(wheelRad));
            int count = 0;
            for(int i = 0; i < 10; i++){
                chairTranslation[i]->MakeTranslation(0,cos(convert(18+wheelRad+count))*70,sin(convert(18+wheelRad+count))*70);
                count+=36;
            }
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* wheelRotation;
        VART::Transform* chairTranslation[10];

    private:
        float wheelRad;
};

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT

     if (argc < 2) {
        cerr << "Fatal error: No filename given.\nUsage: " << argv[0] << " filename\n"
             << "Where 'filename' is a Wavefront OBJ file.\n";
        return 1;
    }
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    MyIHClass idleHandler;
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects and transforms
    
    //wheel
    VART::MeshObject* wheel;
    VART::Transform wheelRotation;
    wheelRotation.MakeIdentity();

    //support
    VART::MeshObject* support;
    
    //chair
    VART::MeshObject* chair;
    VART::Transform chairRotation;
    VART::Transform chairTranslation[10];
    chairRotation.MakeIdentity();

    //Lista de objetos
    list<VART::MeshObject*> objects;
    VART::MeshObject::ReadFromOBJ(argv[1], &objects);
    list<VART::MeshObject*>::iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter){
        if((*iter)->GetDescription() == "wheel"){
            wheel = *iter;
            wheelRotation.AddChild(*dynamic_cast<VART::SceneNode*>(wheel->Copy()));
            scene.AddObject(&wheelRotation);
        }

        if((*iter)->GetDescription() == "support"){
            support = *iter;
            scene.AddObject(support);
        }

         if((*iter)->GetDescription() == "chair"){
            chair = *iter;
            for(int i = 0; i < 10; i++){
                chairTranslation[i].MakeTranslation(0,cos(convert(18*i))*70,sin(convert(18*i))*70);
                chairTranslation[i].AddChild((*dynamic_cast<VART::SceneNode*>(chair->Copy())));
                scene.AddObject(&chairTranslation[i]);
            }
        }

    }

    //setup idleHandler
    idleHandler.wheelRotation = &wheelRotation;
    for(int i = 0; i < 10; i++){
        idleHandler.chairTranslation[i] = &chairTranslation[i];
    }
    
    // Build up the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("Roda Gigante");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler); // attach the idle handler to the viewer

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}

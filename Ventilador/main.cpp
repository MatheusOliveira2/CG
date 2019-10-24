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
        MyIHClass() : wheelRad(0), rodaRad(0){}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
			if(wheelRad < 180){
				supportRotation->MakeYRotation(convert(wheelRad));
			}
			else if(wheelRad >180 && wheelRad < 360){
				supportRotation->MakeYRotation(convert(-wheelRad));
			}
			else if(wheelRad == 360)
				wheelRad = 0;
			wheelRad+=1;
			rodaRad+=4;
			wheelRotation->MakeXRotation(convert(rodaRad));
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* supportRotation;
        VART::Transform* wheelRotation;


    private:
        float wheelRad;
        float rodaRad;
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
    VART::Transform supportRotation;
    supportRotation.MakeIdentity();	
    
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
            wheelRotation.AddChild(*dynamic_cast<VART::SceneNode*>(wheel));
        }

        if((*iter)->GetDescription() == "support"){
            support = *iter;
            //supportRotation.AddChild((*dynamic_cast<VART::SceneNode*>(support)));
            
        }

    }
    supportRotation.AddChild(wheelRotation);
	scene.AddObject(&supportRotation);
    //setup idleHandler
    idleHandler.wheelRotation = &wheelRotation;
	idleHandler.supportRotation = &supportRotation;
	
    // Build up the scene
    scene.AddObject(support);
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

#include <vart/scene.h>
#include <vart/box.h>
#include <vart/cylinder.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>

using namespace std;
#define PI 3.14159265
#define RAD = PI/180;
class ClickHandlerClass : public VART::MouseControl::ClickHandler
{
    public:
        ClickHandlerClass() {
        }
        virtual ~ClickHandlerClass() {};
        virtual void OnClick() {
            if (mouseCtrlPtr->LastClickIsDown()) {
                VART::MouseControl::ButtonID lastButton = mouseCtrlPtr->GetLastClickButton();

                if (lastButton == VART::MouseControl::LEFT) {
                    scenePtr->Pick(mouseCtrlPtr->GetClickXPosition(),
                                   mouseCtrlPtr->GetClickYPosition(), &pickList);
                    if (!pickList.empty()) {
                        cout << pickList.size() << " selected objects: ";
                        while (!pickList.empty()) {
                            cout << pickList.front()->GetDescription() << ":\n";
#ifdef DEBUG
                            VART::MeshObject* ptObj = dynamic_cast<VART::MeshObject*>(pickList.front());
                            if (ptObj)
                                cout << *ptObj << "\n";
#endif
                            pickList.pop_front();
                        }
                        cout << endl;
                    }
                }
            }
        }
        list<VART::GraphicObj*> pickList;
        VART::Scene* scenePtr;
};

float convert(float grau){

    float rad = PI/180;
    return rad*grau;

}

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : i(0){}
        virtual ~MyIHClass() {}
        virtual void OnIdle(){
            i+=1;
            int count = 0;
            for(int j = 0; j < 10; j++){
                float y = count+i+18;
                float z = count+i+18; 
                chairTrans[j]->MakeTranslation(0,cos(convert(y))*70,sin(convert(z))*70);
                count+=36;
            }
            wheelTrans->MakeXRotation(convert(i));
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform *chairTrans[10];
        VART::Transform *wheelTrans;
        //VART::MeshObject* chair;
    private:
        int i;
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

    //KeyboardHandler kbh; // create a keyboard handler
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects
    list<VART::MeshObject*> objects;
    ClickHandlerClass clickHandler;
    MyIHClass idleHandler;
    clickHandler.scenePtr = &scene;

    // Initialize scene objects
    VART::MeshObject::ReadFromOBJ(argv[1], &objects);

    VART::MeshObject* chair = NULL;
    VART::MeshObject* wheel = NULL;
    VART::MeshObject* support = NULL;
    VART::Transform wheelTransform;
    // Build up the scene
    //VART::MeshObject* chairs[10];
    VART::Transform chairTransform[10];
    list<VART::MeshObject*>::iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
        cout << "descrição: " << (*iter)->GetDescription()<< endl;

        if((*iter)->GetDescription()=="chair"){
            chair = *iter;
            for(int i = 1; i<=10; i++){
                chairTransform[i-1].AddChild(*dynamic_cast<VART::SceneNode*>(chair->Copy()));
                scene.AddObject(&chairTransform[i-1]);
            }           
        }

        if((*iter)->GetDescription()=="wheel"){
            wheel =*iter;
            wheelTransform.AddChild(*dynamic_cast<VART::SceneNode*>(wheel->Copy()));
            scene.AddObject(&wheelTransform);
        }

        if((*iter)->GetDescription()=="support"){
            support =*iter;
            scene.AddObject(support);
        }

    }
    
    for(int i = 0; i<10; i++){
        idleHandler.chairTrans[i] = &chairTransform[i];
    }
    idleHandler.wheelTrans = &wheelTransform;


    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();


    // Set up the viewer
    viewer.SetTitle("V-ART OBJ Viewer");
    viewer.SetScene(scene); // attach the scene
    viewer.SetClickHandler(&clickHandler);
    viewer.SetIdleHandler(&idleHandler);
    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)

    return 0;
}

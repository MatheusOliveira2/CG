#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/meshobject.h>
#include <iostream>
#include <string.h>

using namespace std;

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : ampRot(0){}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            ampRot += 0.01;
            ampRotation->MakeYRotation(ampRot);
            ampRotation->MakeZRotation(ampRot);
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* ampRotation;

    private:
        float ampRot;
};
// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    MyIHClass idleHandler;
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects

    VART::MeshObject ampulheta;

    VART::Point4D v0(0,0,0);
    VART::Point4D v1(2,0,0);
    VART::Point4D v2(2,0,2);
    VART::Point4D v3(0,0,2);
    VART::Point4D v4(1,2,1);
    VART::Point4D v5(0,4,0);
    VART::Point4D v6(2,4,0);
    VART::Point4D v7(2,4,2);
    VART::Point4D v8(0,4,2);

    vector<VART::Point4D> vet;
    vet.push_back(v0);
    vet.push_back(v1);
    vet.push_back(v2);
    vet.push_back(v3);
    vet.push_back(v4);
    vet.push_back(v5);
    vet.push_back(v6);
    vet.push_back(v7);
    vet.push_back(v8);
   
    ampulheta.SetVertices(vet);
    string face1="1 4 0";
    string face2="2 4 1";
    string face3="2 4 3";
    string face4="0 4 3";
    string face5="2 1 0 3 ";
    string face6="5 4 7";
    string face7="7 4 6";
    string face8="5 4 7";
    string face9="7 4 6";
    string face10="8 7 6 5";

    
    ampulheta.AddFace(face1.c_str());
    ampulheta.AddFace(face2.c_str());
    ampulheta.AddFace(face3.c_str());
    ampulheta.AddFace(face4.c_str());
    ampulheta.AddFace(face5.c_str());
    ampulheta.AddFace(face6.c_str());
    ampulheta.AddFace(face7.c_str());
    ampulheta.AddFace(face8.c_str());
    ampulheta.AddFace(face9.c_str());
    ampulheta.AddFace(face10.c_str());
    ampulheta.SetMaterial(VART::Material::PLASTIC_RED());


    ampulheta.ComputeVertexNormals();
    // Initialize scene objects

    VART::Transform ampulhetaRotation;
    ampulhetaRotation.MakeIdentity();
    
    ampulhetaRotation.AddChild(ampulheta);
    idleHandler.ampRotation = &ampulhetaRotation;

    // Build up the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.AddObject(&ampulhetaRotation);
    // Set up the Idle Handler
    

    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler);
   
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}

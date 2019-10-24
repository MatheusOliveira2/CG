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
    VART::Camera camera(VART::Point4D(0,6,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects

    VART::MeshObject piramide;
    VART::MeshObject piramide2;
    VART::Point4D v0(0,0,0);
    VART::Point4D v1(2,0,0);
    VART::Point4D v2(2,0,2);
    VART::Point4D v3(0,0,2);
    VART::Point4D v4(1,2,1);
    vector<VART::Point4D> vet;
    vet.push_back(v0);
    vet.push_back(v1);
    vet.push_back(v2);
    vet.push_back(v3);
    vet.push_back(v4);
    piramide.SetVertices(vet);
    string face1="1 4 0";
    string face2="2 4 1";
    string face3="2 4 3";
    string face4="0 4 3";
    string face5="2 1 0 3";
    piramide.AddFace(face1.c_str());
    piramide.AddFace(face2.c_str());
    piramide.AddFace(face3.c_str());
    piramide.AddFace(face4.c_str());
    piramide.AddFace(face5.c_str());
    piramide.SetMaterial(VART::Material::PLASTIC_RED());
    piramide.ComputeVertexNormals();

    //piramide2
    VART::Point4D v5(0,4,0);
    VART::Point4D v6(2,4,0);
    VART::Point4D v7(2,4,2);
    VART::Point4D v8(0,4,2);
    VART::Point4D v9(1,2,1);
    vector<VART::Point4D> vet2;
    vet2.push_back(v5);
    vet2.push_back(v6);
    vet2.push_back(v7);
    vet2.push_back(v8);
    vet2.push_back(v9);
    piramide2.SetVertices(vet2);
    string face6="1 4 0";
    string face7="2 4 1";
    string face8="3 4 2";
    string face9="0 4 3";
    string face10="2 1 0 3";
    piramide2.AddFace(face6.c_str());
    piramide2.AddFace(face7.c_str());
    piramide2.AddFace(face8.c_str());
    piramide2.AddFace(face9.c_str());
    piramide2.AddFace(face10.c_str());
    piramide2.SetMaterial(VART::Material::PLASTIC_GREEN());

    piramide2.ComputeVertexNormals();
    // Initialize scene objects
    piramide.MergeWith(piramide2);

    // Build up the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    
  
    // Set up the Idle Handler

    VART::Transform ampulhetaRotation;
    ampulhetaRotation.MakeIdentity();
    
    ampulhetaRotation.AddChild(piramide);
    idleHandler.ampRotation = &ampulhetaRotation;
    
    scene.AddObject(&piramide);
    scene.AddObject(&piramide2);
    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler);
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}

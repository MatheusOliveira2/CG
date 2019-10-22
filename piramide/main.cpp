#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/meshobject.h>
#include <iostream>
#include <string.h>

using namespace std;

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects

    VART::MeshObject piramede;

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
   
    piramede.SetVertices(vet);
    string face1="1 4 0";
    string face2="2 4 1";
    string face3="2 4 3";
    string face4="0 4 3";
    string face5="2 1 0 3";
    
    piramede.AddFace(face1.c_str());
    piramede.AddFace(face2.c_str());
    piramede.AddFace(face3.c_str());
    piramede.AddFace(face4.c_str());
    piramede.AddFace(face5.c_str());
    piramede.SetMaterial(VART::Material::PLASTIC_RED());


    piramede.ComputeVertexNormals();
    // Initialize scene objects
    

    // Build up the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.AddObject(&piramede);
    // Set up the Idle Handler
    

    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
   
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}

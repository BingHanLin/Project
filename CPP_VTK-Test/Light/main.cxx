#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkCubeSource.h"


int main(int, char *[])
{
  // The following lines create a sphere represented by polygons.
  //
  vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetThetaResolution(100);
  sphere->SetPhiResolution(50);


  // The mapper is responsible for pushing the geometry into the graphics
  // library. It may also do color mapping, if scalars or other attributes
  // are defined.
  //
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphere->GetOutputPort());

  // The actor is a grouping mechanism: besides the geometry (mapper), it
  // also has a property, transformation matrix, and/or texture map.
  // In this example we create eight different spheres (two rows of four
  // spheres) and set the specular lighting coefficients. A little ambient
  // is turned on so the sphere is not completely black on the back side.
  //
  vtkSmartPointer<vtkActor> sphere1 = vtkSmartPointer<vtkActor>::New();
  sphere1->SetMapper(sphereMapper);
  sphere1->GetProperty()->SetColor(0,0,1);
//   sphere1->GetProperty()->SetAmbient(1.0);
//   sphere1->GetProperty()->SetDiffuse(0.5);
//   sphere1->GetProperty()->SetSpecular(0.5);
  sphere1->GetProperty()->SetSpecularPower(5.0);
  sphere1->AddPosition(0,0,1.1);

  // Create a cube.
  vtkSmartPointer<vtkCubeSource> cubeSource = 
    vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->SetYLength(10.0);
    cubeSource->SetXLength(10.0);
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(cubeSource->GetOutputPort());
  vtkSmartPointer<vtkActor> cubeActor = 
    vtkSmartPointer<vtkActor>::New();
  cubeActor->SetMapper(mapper);


  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  ren1->SetAmbient(1.0,1.0,1.0);
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);
  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  // Add the actors to the renderer, set the background and size.
  //
  ren1->AddActor(sphere1);
  ren1->AddActor(cubeActor);
  ren1->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(600, 400);

  // Set up the lighting.
  //
  vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
  light->SetFocalPoint(1.875,0.6125,0);
  light->SetPosition(0.875,1.6125,1);
  light->SetColor(1,0,0);
  light->SetIntensity(1.0);
  ren1->AddLight(light);

  // We want to eliminate perspective effects on the apparent lighting.
  // Parallel camera projection will be used. To zoom in parallel projection
  // mode, the ParallelScale is set.
  //
  ren1->GetActiveCamera()->SetFocalPoint(0,0,0);
  ren1->GetActiveCamera()->SetPosition(1,0,0);
  ren1->GetActiveCamera()->SetViewUp(0,0,1);
  ren1->GetActiveCamera()->ParallelProjectionOn();
  ren1->ResetCamera();
  ren1->GetActiveCamera()->SetParallelScale(1.5);

  // This starts the event loop and invokes an initial render.
  //
  iren->Initialize();
  iren->Start();

  return EXIT_SUCCESS;
}
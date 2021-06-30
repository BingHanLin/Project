#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"

int main()
{
    vtkSmartPointer<vtkSphereSource> sphere =
        vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetThetaResolution(100);
    sphere->SetPhiResolution(50);

    vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphere->GetOutputPort());

    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    sphereActor->GetProperty()->SetSpecular(1.0);
    sphereActor->GetProperty()->SetSpecularPower(64);
    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
    ren->AddActor(sphereActor);

    ren->SetBackground(0.1, 0.2, 0.4);

    auto light1 = vtkSmartPointer<vtkLight>::New();
    light1->SetFocalPoint(0, 0, 0);
    light1->SetPosition(1, 0, 0);
    light1->SetColor(0, 0, 1);
    light1->SetSpecularColor(0, 0, 1.0);
    ren->AddLight(light1);
    // Light one is on the right and is red.
    auto light2 = vtkSmartPointer<vtkLight>::New();
    light2->SetFocalPoint(0, 0, 0);
    light2->SetPosition(-1, 0, 0);
    light2->SetColor(1, 0, 0);
    light2->SetSpecularColor(0.5, 0, 0);
    ren->AddLight(light2);

    ren->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    ren->GetActiveCamera()->SetPosition(0, 0, 1);
    ren->GetActiveCamera()->SetViewUp(0, 1, 0);
    ren->GetActiveCamera()->ParallelProjectionOn();
    ren->ResetCamera();
    ren->GetActiveCamera()->SetParallelScale(1.5);

    vtkSmartPointer<vtkRenderWindow> renWin =
        vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(ren);
    renWin->SetSize(400, 200);
    vtkSmartPointer<vtkRenderWindowInteractor> iren =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);

    iren->Initialize();
    iren->Start();
    return EXIT_SUCCESS;
}

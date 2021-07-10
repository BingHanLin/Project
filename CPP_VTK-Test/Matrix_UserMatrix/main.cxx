#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkConeSource.h>
#include <vtkLineSource.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>

#include <iostream>

vtkSmartPointer<vtkActor> buildOriginActor()
{
    vtkSmartPointer<vtkSphereSource> originSource =
        vtkSmartPointer<vtkSphereSource>::New();
    originSource->SetRadius(0.1);
    originSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> originMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    originMapper->SetInputConnection(originSource->GetOutputPort());

    vtkSmartPointer<vtkActor> originActor = vtkSmartPointer<vtkActor>::New();
    originActor->SetPosition(0, 0, 0);
    originActor->SetMapper(originMapper);
    originActor->GetProperty()->SetColor(1.0, 1.0, 1.0);

    return originActor;
}

vtkSmartPointer<vtkActor> buildConeActor()
{
    vtkSmartPointer<vtkConeSource> coneSource =
        vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetRadius(.2);
    coneSource->SetHeight(.5);
    coneSource->SetCenter(0, 0, 0);

    vtkSmartPointer<vtkPolyDataMapper> coneMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(coneSource->GetOutputPort());

    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);

    return coneActor;
}

vtkSmartPointer<vtkAxesActor> buildAxesActor()
{
    vtkSmartPointer<vtkAxesActor> axesActor =
        vtkSmartPointer<vtkAxesActor>::New();
    axesActor->SetPosition(0, 0, 0);
    axesActor->SetTotalLength(1.0, 1.0, 1.0);
    axesActor->SetShaftType(0);
    axesActor->SetAxisLabels(0);
    axesActor->SetCylinderRadius(0.02);

    return axesActor;
}

int main(int, char *[])
{
    vtkSmartPointer<vtkActor> coneActor = buildConeActor();
    vtkSmartPointer<vtkAxesActor> axesActor = buildAxesActor();

#if 0
    // M = A*M where M is the current transformation matrix and A is the applied
    // matrix.
    vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
    trans->PostMultiply();
    trans->RotateZ(45);
    trans->Translate(1, 0, 0);
    coneActor->SetUserTransform(trans);
#endif

#if 1
    coneActor->RotateZ(45);
    coneActor->SetPosition(1.0, 0.0, 0.0);
#endif

    std::cout << "GetMatrix:" << std::endl;
    if (coneActor->GetMatrix() != NULL)
    {
        coneActor->GetMatrix()->Print(std::cout);
    }
    else
    {
        std::cout << "NULL" << std::endl;
    }

    // Get the modified time of the user matrix or user transform.
    // The UserMatrix can be used in place of UserTransform.
    std::cout << "GetUserMatrix:" << std::endl;
    if (coneActor->GetUserMatrix() != NULL)
    {
        coneActor->GetUserMatrix()->Print(std::cout);
    }
    else
    {
        std::cout << "NULL" << std::endl;
    }

    std::cout << "GetUserTransform:" << std::endl;
    if (coneActor->GetUserTransform() != NULL)
    {
        coneActor->GetUserTransform()->Print(std::cout);
    }
    else
    {
        std::cout << "NULL" << std::endl;
    }

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(800, 400);
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkSmartPointer<vtkActor> originActor = buildOriginActor();

    renderer->AddActor(axesActor);
    renderer->AddActor(originActor);
    renderer->AddActor(coneActor);
    renderer->SetBackground(.3, .3, .5);

    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
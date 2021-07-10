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
    vtkSmartPointer<vtkActor> oriConeActor = buildConeActor();
    vtkSmartPointer<vtkActor> coneActor = buildConeActor();

    vtkSmartPointer<vtkAxesActor> oriAxesActor = buildAxesActor();
    vtkSmartPointer<vtkAxesActor> axesActor = buildAxesActor();

    vtkSmartPointer<vtkTextActor> textActor =
        vtkSmartPointer<vtkTextActor>::New();
    textActor->SetPosition2(100, 40);
    textActor->GetTextProperty()->SetFontSize(24);
    textActor->GetTextProperty()->SetColor(1, 0, 0);

#if 0
    // M = A*M where M is the current transformation matrix and A is the applied matrix.(process in order)
    vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
    trans->PostMultiply();
    trans->RotateZ(45);
    trans->Translate(1, 0, 0);
    coneActor->SetUserTransform(trans);

    textActor->SetInput(
        "PostMultiply()\nRotateZ(45)\nTranslate(1.0, 0.0, 0.0)");
#endif

#if 0
    // M = M*A where M is the current transformation matrix and A is the applied
    // matrix.(process in inverse order)
    vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
    trans->RotateZ(45);
    trans->Translate(1, 0, 0);
    coneActor->SetUserTransform(trans);

    textActor->SetInput("PreMultiply()\nRotateZ(45)\nranslate(1.0, 0.0, 0.0)");

#endif

#if 1
    // M = A*M where M is the current transformation matrix and A is the applied
    // matrix.
    vtkSmartPointer<vtkMatrix4x4> transMatrix =
        vtkSmartPointer<vtkMatrix4x4>::New();

    transMatrix->SetElement(0, 0, 0.707107);
    transMatrix->SetElement(0, 1, -0.707107);
    transMatrix->SetElement(0, 2, 0);
    transMatrix->SetElement(0, 3, 0.707107);

    transMatrix->SetElement(1, 0, 0.707107);
    transMatrix->SetElement(1, 1, 0.707107);
    transMatrix->SetElement(1, 2, 0);
    transMatrix->SetElement(1, 3, 0.707107);

    transMatrix->SetElement(2, 0, 0.0);
    transMatrix->SetElement(2, 1, 0.0);
    transMatrix->SetElement(2, 2, 1.0);
    transMatrix->SetElement(2, 3, 0.0);

    transMatrix->SetElement(3, 0, 0.0);
    transMatrix->SetElement(3, 1, 0.0);
    transMatrix->SetElement(3, 2, 0.0);
    transMatrix->SetElement(3, 3, 1.0);

    coneActor->SetUserMatrix(transMatrix);

    textActor->SetInput("SetUserMatrix()");

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

    vtkSmartPointer<vtkRenderer> renderer1 =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderer> renderer2 =
        vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(800, 400);
    renderWindow->AddRenderer(renderer1);
    renderWindow->AddRenderer(renderer2);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    double leftViewport[] = {0.0, 0.0, 0.5, 1.0};
    double rightViewport[] = {0.5, 0.0, 1.0, 1.0};

    vtkSmartPointer<vtkActor> originActor = buildOriginActor();

    renderer1->AddActor(oriAxesActor);
    renderer1->AddActor(originActor);
    renderer1->AddActor(oriConeActor);
    renderer1->SetBackground(.3, .3, .5);
    renderer1->SetViewport(leftViewport);

    renderer2->AddActor(axesActor);
    renderer2->AddActor(originActor);
    renderer2->AddActor(coneActor);
    renderer2->AddActor2D(textActor);
    renderer2->SetBackground(.2, .4, .5);
    renderer2->SetViewport(rightViewport);

    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
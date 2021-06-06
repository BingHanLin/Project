#include <vtkActor.h>
#include <vtkAppendFilter.h>
#include <vtkConeSource.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

int main(int, char *[])
{
    // Set up the data pipeline
    auto cone = vtkSmartPointer<vtkConeSource>::New();
    cone->SetHeight(1.0);
    cone->Update();

    auto coneMapper = vtkSmartPointer<vtkDataSetMapper>::New();

    // this won't create connection back to cone source.(need Update manually)
    // coneMapper->SetInputData(cone->GetOutput());

    // this will create connection back to cone source.
    coneMapper->SetInputConnection(cone->GetOutputPort());

    auto coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(coneActor);
    renderer->SetBackground(0.1, 0.2, 0.4);

    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(200, 200);
    renderWindow->AddRenderer(renderer);

    auto renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Start();

    // Change cone property
    cone->SetHeight(10.0);

    auto renderer2 = vtkSmartPointer<vtkRenderer>::New();
    renderer2->AddActor(coneActor);
    renderer2->SetBackground(0.1, 0.2, 0.4);

    auto renderWindow2 = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow2->SetSize(200, 200);
    renderWindow2->AddRenderer(renderer2);

    auto renderWindowInteractor2 =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor2->SetRenderWindow(renderWindow2);

    renderWindowInteractor2->Start();

    return EXIT_SUCCESS;
}
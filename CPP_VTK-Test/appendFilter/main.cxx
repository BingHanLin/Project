#include <vtkActor.h>
#include <vtkAppendFilter.h>
#include <vtkCompositeDataIterator.h>
#include <vtkCompositeDataSet.h>
#include <vtkCubeSource.h>
#include <vtkCutter.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLMultiBlockDataReader.h>

#include <iostream>
int main(int, char*[])
{
    vtkSmartPointer<vtkXMLMultiBlockDataReader> reader =
        vtkSmartPointer<vtkXMLMultiBlockDataReader>::New();
    reader->SetFileName("block.vtm");
    reader->Update();

    vtkCompositeDataSet* dataset = reader->GetOutput();

    std::cout << "start iter" << std::endl;
    vtkNew<vtkAppendFilter> appendFilter;
    vtkCompositeDataIterator* iter = dataset->NewIterator();
    for (iter->InitTraversal(); !iter->IsDoneWithTraversal();
         iter->GoToNextItem())
    {
        vtkDataObject* dObj = iter->GetCurrentDataObject();

        vtkSmartPointer<vtkUnstructuredGrid> ug =
            vtkUnstructuredGrid::SafeDownCast(dObj);

        appendFilter->AddInputData(dObj);

        std::cout << dObj->GetClassName() << std::endl;
    }

    appendFilter->Update();

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0, 0, 0);
    plane->SetNormal(1, 0, 0);

    // Create cutter
    vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
    cutter->SetCutFunction(plane);
    cutter->SetInputConnection(appendFilter->GetOutputPort());
    cutter->Update();

    vtkSmartPointer<vtkPolyDataMapper> cutterMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    cutterMapper->SetInputConnection(cutter->GetOutputPort());

    // Create plane actor
    vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
    planeActor->GetProperty()->SetColor(1.0, 1.0, 0);
    planeActor->GetProperty()->SetLineWidth(2);
    planeActor->SetMapper(cutterMapper);

    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputConnection(appendFilter->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // Create a renderer, render window, and interactor
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Add the actor to the scene
    renderer->AddActor(actor);
    actor->GetProperty()->SetOpacity(0.3);
    renderer->SetBackground(colors->GetColor3d("RoyalBlue").GetData());
    renderer->AddActor(planeActor);

    // Render and interact
    renderWindow->SetWindowName("AppendFilter");
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
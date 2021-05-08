#include <iostream>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridReader.h>

int main(int, char*[])
{
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
        vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

    reader->SetFileName("bearing.vtu");

    if (true)
    {
        // At this point, the reader hasn't yet read the file, so the
        // following line with result in empty dataset (or something like that)
        std::cout << "Reader not updated." << std::endl;
    }
    else
    {
        // However, once you tell the reader "update right now, don't wait
        // for the pipeline to update you" with: you can now get access to the
        // data it has read:
        reader->Update();
        std::cout << "Reader updated." << std::endl;
    }

    // check current data from reader
    vtkSmartPointer<vtkUnstructuredGrid> ugrid = reader->GetOutput();
    std::cout << "Number of cells: " << ugrid->GetNumberOfCells() << std::endl;

    vtkSmartPointer<vtkDataSetMapper> mapper =
        vtkSmartPointer<vtkDataSetMapper>::New();

    // this won't create connection back to reader.
    // mapper->SetInputData(reader->GetOutput());

    // this will create connection back to reader.
    mapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // The renderer says "Ok, now I need the data that drives this whole chain",
    // the pipeline will go back and call Update() on the reader.
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->AddActor(actor);

    renderWindowInteractor->Initialize();
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
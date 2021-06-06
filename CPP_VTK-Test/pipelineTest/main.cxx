#include <vtkActor.h>
#include <vtkAppendFilter.h>
#include <vtkCommand.h>
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
#include <vtkTrivialProducer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLMultiBlockDataReader.h>
#include <vtkXMLUnstructuredGridReader.h>

#include <iostream>
#include <vtkAlgorithm.h>

vtkSmartPointer<vtkUnstructuredGrid> readUnstructuredGrid(
    const std::string fileName)
{
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
        vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();

    return reader->GetOutput();
}

class vtkTimerCallback : public vtkCommand
{
   public:
    vtkTimerCallback() = default;

    void setMapper(vtkSmartPointer<vtkTrivialProducer> producer)
    {
        trivialProducer_ = producer;
    };

    void setAcotr(vtkSmartPointer<vtkActor> actor)
    {
        actor_ = actor;
    };

    void setRenderer(vtkSmartPointer<vtkRenderer> renderer)
    {
        renderer_ = renderer;
    };

    void updateMapper(const int timerCount)
    {
        if (this->TimerCount % 2 == 0)
        {
            vtkSmartPointer<vtkUnstructuredGrid> grid =
                readUnstructuredGrid("1.vtu");
            trivialProducer_->SetOutput(grid);
        }
        else
        {
            vtkSmartPointer<vtkUnstructuredGrid> grid =
                readUnstructuredGrid("0.vtu");
            trivialProducer_->SetOutput(grid);
        }
        renderer_->GetRenderWindow()->Render();
    };

    static vtkTimerCallback* New()
    {
        vtkTimerCallback* cb = new vtkTimerCallback;
        cb->TimerCount = 0;
        return cb;
    }

    virtual void Execute(vtkObject* caller, unsigned long eventId,
                         void* vtkNotUsed(callData))
    {
        if (vtkCommand::TimerEvent == eventId)
        {
            ++this->TimerCount;
        }

        updateMapper(this->TimerCount);

        std::cout << this->TimerCount << std::endl;
        if (this->TimerCount >= this->maxCount)
        {
            auto iren = dynamic_cast<vtkRenderWindowInteractor*>(caller);
            if (this->timerId > -1)
            {
                iren->DestroyTimer(this->timerId);
            }
        }
    }

   private:
    int TimerCount = 0;
    vtkSmartPointer<vtkTrivialProducer> trivialProducer_;
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkRenderer> renderer_;

   public:
    int maxCount = 0;
    int timerId = -1;
};

int main(int, char*[])
{
    vtkSmartPointer<vtkUnstructuredGrid> grid = readUnstructuredGrid("1.vtu");

    vtkSmartPointer<vtkTrivialProducer> trivialProducer =
        vtkSmartPointer<vtkTrivialProducer>::New();
    trivialProducer->SetOutput(grid);

    vtkSmartPointer<vtkDataSetMapper> mapper =
        vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(trivialProducer->GetOutputPort());

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0, 0, 0.005);
    plane->SetNormal(0, 0, 1);

    // Create cutter
    vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
    cutter->SetCutFunction(plane);
    cutter->SetInputConnection(trivialProducer->GetOutputPort());
    cutter->Update();

    vtkSmartPointer<vtkDataSetMapper> cutterMapper =
        vtkSmartPointer<vtkDataSetMapper>::New();
    cutterMapper->SetInputConnection(cutter->GetOutputPort());

    cutterMapper->ScalarVisibilityOn();
    cutterMapper->SelectColorArray("pressure");
    cutterMapper->SetScalarModeToUseCellFieldData();

    // Create plane actor
    vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
    planeActor->GetProperty()->SetColor(1.0, 1.0, 0);
    planeActor->GetProperty()->SetLineWidth(2);
    planeActor->SetMapper(cutterMapper);

    vtkNew<vtkNamedColors> colors;

    // Create a renderer, render window, and interactor
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Add the actor to the scene
    renderer->SetBackground(colors->GetColor3d("RoyalBlue").GetData());
    renderer->AddActor(planeActor);

    renderWindowInteractor->Initialize();

    vtkNew<vtkTimerCallback> cb;
    cb->setMapper(trivialProducer);
    cb->setAcotr(planeActor);
    cb->setRenderer(renderer);

    renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
    int timerId = renderWindowInteractor->CreateRepeatingTimer(5000);
    std::cout << "timerId: " << timerId << std::endl;
    cb->maxCount = 20;
    cb->timerId = timerId;

    // Render and interact
    renderWindow->SetWindowName("AppendFilter");
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
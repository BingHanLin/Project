#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkInteractorObserver.h>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkWindowToImageFilter.h>
#include <vtkXMLUnstructuredGridReader.h>

// https://discourse.vtk.org/t/possible-to-use-different-interaction-styles-across-viewports/1926/2
// http://vtk.1045678.n5.nabble.com/VTK5-6-1-One-render-window-multiple-renders-and-auto-interactor-style-solution-td4358940.html

// class vtkTimerCallback : public vtkCommand
// {
//   public:
//     static vtkTimerCallback *New()
//     {
//       vtkTimerCallback *cb = new vtkTimerCallback;

//       return cb;
//     }

//     virtual void Execute(vtkObject *vtkNotUsed(caller),
//                          unsigned long vtkNotUsed(eventId),
//                          void *vtkNotUsed(callData))
//     {
//         TestFilter->Modified();
//         TestFilter->Update();
//     }

//     vtkMyTestFilter* TestFilter;

// };

static vtkSmartPointer<vtkRenderer> threeDRenderer;
void MouseMoveCallbackFunc(vtkObject *caller, long unsigned int eventId,
                           void *vtkNotUsed(clientData),
                           void *vtkNotUsed(callData)) {
  std::cout << "Mouse Move Callback" << std::endl;
  std::cout << "Event: " << vtkCommand::GetStringFromEventId(eventId)
            << std::endl;

  vtkRenderWindowInteractor *renderWindowInteractor =
      static_cast<vtkRenderWindowInteractor *>(caller);

  std::cout << threeDRenderer.GetPointer()->GetClassName() << std::endl;

  int pos1, pos2;
  renderWindowInteractor->GetEventPosition(pos1, pos2);
  if (threeDRenderer.GetPointer() ==
      renderWindowInteractor->FindPokedRenderer(pos1, pos2)) {
    std::cout << "threeDRenderer" << std::endl;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->SetInteractorStyle(style);
  } else {
    std::cout << "not the same" << std::endl;
    vtkSmartPointer<vtkInteractorStyleImage> style =
        vtkSmartPointer<vtkInteractorStyleImage>::New();
    renderWindowInteractor->SetInteractorStyle(style);
  }
}

int main(int, char *[]) {
  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  vtkSmartPointer<vtkConeSource> coneSource =
      vtkSmartPointer<vtkConeSource>::New();
  coneSource->Update();

  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(coneSource->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // Define viewport ranges
  double xmins[4] = {
      0,
      .75,
      .75,
      .75,
  };
  double xmaxs[4] = {.75, 1, 1, 1};
  double ymins[4] = {0, 0, .33, .67};
  double ymaxs[4] = {1, .33, .67, 1.0};

  vtkSmartPointer<vtkRenderer> mainRenderer =
      vtkSmartPointer<vtkRenderer>::New();
  threeDRenderer = mainRenderer.GetPointer();
  mainRenderer->SetViewport(xmins[0], ymins[0], xmaxs[0], ymaxs[0]);
  mainRenderer->AddActor(actor);
  mainRenderer->ResetCamera();
  renderWindow->AddRenderer(mainRenderer);
  renderWindow->Render();

  for (unsigned i = 1; i < 4; i++) {
    // vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    // camera->SetPosition(0, 0, 0);
    // camera->SetFocalPoint(1, 0, 0);
    // camera->SetViewUp(0, 0, 1);
    // // camera->ResetCamera();
    // mainRenderer->SetActiveCamera(camera);
    // renderWindow->Render();

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
        vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(renderWindow);
    windowToImageFilter->SetViewport(xmins[0], ymins[0], xmaxs[0], ymaxs[0]);
    windowToImageFilter->SetInputBufferTypeToRGBA();
    windowToImageFilter->ReadFrontBufferOff();
    windowToImageFilter->Update();

    vtkSmartPointer<vtkImageActor> imageAcotr =
        vtkSmartPointer<vtkImageActor>::New();
    imageAcotr->GetMapper()->SetInputConnection(
        windowToImageFilter->GetOutputPort());

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetViewport(xmins[i], ymins[i], xmaxs[i], ymaxs[i]);
    renderer->AddActor2D(imageAcotr);
    renderer->ResetCamera();

    renderWindow->AddRenderer(renderer);
    renderWindow->Render();
  }

  renderWindow->SetWindowName("Multiple ViewPorts");

  // add observer
  vtkSmartPointer<vtkCallbackCommand> clickCallbackCommand =
      vtkSmartPointer<vtkCallbackCommand>::New();
  clickCallbackCommand->SetCallback(MouseMoveCallbackFunc);
  renderWindowInteractor->AddObserver(vtkCommand::MouseMoveEvent,
                                      clickCallbackCommand);

  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

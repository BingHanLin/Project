#include <iostream>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkAppendPolyData.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkFloatArray.h>
#include <vtkLight.h>
#include <vtkLineSource.h>
#include <vtkPlaneSource.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQuadricDecimation.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTriangleFilter.h>

using namespace std;

int main() {
  vtkPolyData *originPolyData = vtkPolyData::New();
  vtkPolyData *shallowCopyPolyData = vtkPolyData::New();
  vtkPolyData *deepCopyPolyData = vtkPolyData::New();

  int i;
  static float x[8][3] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
                          {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}};

  static vtkIdType pts[6][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {0, 1, 5, 4},
                                {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}};

  vtkPoints *points = vtkPoints::New();
  vtkCellArray *polys = vtkCellArray::New();
  vtkFloatArray *scalars = vtkFloatArray::New();

  // load the point, cell, and data attributes.
  for (i = 0; i < 8; i++)
    points->InsertPoint(i, x[i]);

  for (i = 0; i < 6; i++)
    polys->InsertNextCell(4, pts[i]);

  for (i = 0; i < 8; i++)
    scalars->InsertTuple1(i, i);

  // setup origin poly data
  originPolyData->SetPoints(points);
  originPolyData->SetPolys(polys);
  originPolyData->GetPointData()->SetScalars(scalars);
  // setup shallow copy poly data
  shallowCopyPolyData->ShallowCopy(originPolyData);
  // setup deep copy poly data
  deepCopyPolyData->DeepCopy(originPolyData);

  // reset the data attributes in originPolyData
  scalars->Initialize();
  for (i = 0; i < 8; i++)
    scalars->InsertTuple1(i, 0);
  originPolyData->GetPointData()->SetScalars(scalars);
  originPolyData->Modified();

  // delete unecessary data
  points->Delete();
  polys->Delete();
  scalars->Delete();

  // create actors
  vtkSmartPointer<vtkPolyDataMapper> leftMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  leftMapper->SetInputData(originPolyData);
  vtkSmartPointer<vtkActor> leftActor = vtkSmartPointer<vtkActor>::New();
  leftActor->SetMapper(leftMapper);

  vtkSmartPointer<vtkPolyDataMapper> middleMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  middleMapper->SetInputData(shallowCopyPolyData);
  vtkSmartPointer<vtkActor> middleActor = vtkSmartPointer<vtkActor>::New();
  middleActor->SetMapper(middleMapper);

  vtkSmartPointer<vtkPolyDataMapper> rightMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  rightMapper->SetInputData(deepCopyPolyData);
  vtkSmartPointer<vtkActor> rightActor = vtkSmartPointer<vtkActor>::New();
  rightActor->SetMapper(rightMapper);

  // define viewport ranges
  // (xmin, ymin, xmax, ymax) left-top and right-bottom point
  double leftViewport[4] = {0.0, 0.0, 0.33333, 1.0};
  double middleViewport[4] = {0.33333, 0.0, 0.6666, 1.0};
  double rightViewport[4] = {0.6666, 0.0, 1.0, 1.0};

  // setup renderers
  vtkSmartPointer<vtkRenderer> leftRenderer =
      vtkSmartPointer<vtkRenderer>::New();
  leftRenderer->SetViewport(leftViewport);
  leftRenderer->SetBackground(0, 0, 0);

  vtkSmartPointer<vtkRenderer> middleRenderer =
      vtkSmartPointer<vtkRenderer>::New();
  middleRenderer->SetViewport(middleViewport);
  middleRenderer->SetBackground(0, 0, 0);

  vtkSmartPointer<vtkRenderer> rightRenderer =
      vtkSmartPointer<vtkRenderer>::New();
  rightRenderer->SetViewport(rightViewport);
  rightRenderer->SetBackground(0, 0, 0);

  // add actors
  leftRenderer->AddActor(leftActor);
  middleRenderer->AddActor(middleActor);
  rightRenderer->AddActor(rightActor);

  vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
  leftRenderer->SetActiveCamera(camera);
  middleRenderer->SetActiveCamera(camera);
  rightRenderer->SetActiveCamera(camera);

  leftRenderer->ResetCamera();
  middleRenderer->ResetCamera();
  rightRenderer->ResetCamera();

  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(600, 300);
  renderWindow->AddRenderer(leftRenderer);
  renderWindow->AddRenderer(middleRenderer);
  renderWindow->AddRenderer(rightRenderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindow->Render();
  renderWindowInteractor->Start();
  return 0;
}
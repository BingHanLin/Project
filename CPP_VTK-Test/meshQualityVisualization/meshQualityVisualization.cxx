#include <vtkSmartPointer.h>

#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>

#include <vtkMeshQuality.h>
#include <vtkFieldData.h>

#include <vtkLogLookupTable.h>
#include <vtkColorTransferFunction.h>

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
// #include <vtkNamedColors.h>

vtkSmartPointer<vtkUnstructuredGrid>
readUnstructuredGrid(const std::string fileName)
{

  vtkSmartPointer<vtkUnstructuredGridReader> reader =
      vtkSmartPointer<vtkUnstructuredGridReader>::New();
  reader->SetFileName(fileName.c_str());
  reader->Update();

  return reader->GetOutput();
}

vtkSmartPointer<vtkMeshQuality> applyQuality(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
  vtkSmartPointer<vtkMeshQuality> qualityFilter =
      vtkSmartPointer<vtkMeshQuality>::New();

  qualityFilter->SetInputData(grid);
  // qualityFilter->SetTriangleQualityMeasureToArea();
  qualityFilter->SetTetQualityMeasure(VTK_QUALITY_ASPECT_RATIO);
  qualityFilter->Update();

  return qualityFilter;
}

vtkSmartPointer<vtkLogLookupTable> lookUpTable()
{

  vtkSmartPointer<vtkLogLookupTable> lut =
      vtkSmartPointer<vtkLogLookupTable>::New();
  int numColors = 256;
  lut->SetNumberOfTableValues(numColors);

  vtkSmartPointer<vtkColorTransferFunction> transferFunc =
      vtkSmartPointer<vtkColorTransferFunction>::New();

  transferFunc->SetColorSpaceToDiverging();
  transferFunc->AddRGBPoint(0, 0.230, 0.299, 0.754);
  transferFunc->AddRGBPoint(1, 0.706, 0.016, 0.150);

  for (int i = 0; i < numColors; i++)
  {
    double ss = (double)i / (double)numColors;
    auto cc = transferFunc->GetColor(ss);
    lut->SetTableValue(i, cc[0], cc[1], cc[2], 1.0);
  }

  return lut;
}

int main(int argc, char *argv[])
{
  vtkSmartPointer<vtkUnstructuredGrid> grid = readUnstructuredGrid("testGrid.vtk");
  vtkSmartPointer<vtkMeshQuality> qualityFilter = applyQuality(grid);
  vtkSmartPointer<vtkLogLookupTable> lut = lookUpTable();

  double qualityMin = qualityFilter->GetOutput()->GetFieldData()->GetArray("Mesh Tetrahedron Quality")->GetComponent(0, 0);
  double qualityMax = qualityFilter->GetOutput()->GetFieldData()->GetArray("Mesh Tetrahedron Quality")->GetComponent(0, 2);

  vtkSmartPointer<vtkDataSetMapper> mapper =
      vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputData(qualityFilter->GetOutput());

  mapper->SetScalarRange(qualityMin, qualityMax);
  mapper->SetLookupTable(lut);

  // legend_actor = vtk.vtkScalarBarActor()
  // legend_actor.SetLookupTable(lookuptable)

  vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->EdgeVisibilityOn();

  vtkSmartPointer<vtkCamera> camera =
      vtkSmartPointer<vtkCamera>::New();
  camera->SetPosition(-27, 300, 170);
  camera->SetFocalPoint(0, 0, 50);
  camera->SetViewUp(0, -0.5, 1);

  vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  // renderer->AddActor(legend_actor);
  renderer->SetBackground(1, 1, 1);
  renderer->SetActiveCamera(camera);

  vtkSmartPointer<vtkRenderWindow> window =
      vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor>
      interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

  interactor->SetRenderWindow(window);

  vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

  interactor->SetInteractorStyle(style);

  interactor->Initialize();
  interactor->Start();

  return EXIT_SUCCESS;
}
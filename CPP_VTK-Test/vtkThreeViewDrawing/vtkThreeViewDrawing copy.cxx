#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkImageMapToColors.h>
#include <vtkImageReslice.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkResampleToImage.h>
#include <vtkSmartPointer.h>
#include <vtkWindowLevelLookupTable.h>

#include <vtkConeSource.h>
#include <vtkXMLUnstructuredGridReader.h>


int main(int, char *[]) {
  // vtu reader
  vtkSmartPointer<vtkXMLUnstructuredGridReader> vtuReader =
      vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

  //   vtkSmartPointer<vtkImageCast> pImageCast =
  //       vtkSmartPointer<vtkImageCast>::New();

  //   slice
  vtkSmartPointer<vtkImageReslice> pImageResliceX =
      vtkSmartPointer<vtkImageReslice>::New();
  vtkSmartPointer<vtkImageReslice> pImageResliceY =
      vtkSmartPointer<vtkImageReslice>::New();
  vtkSmartPointer<vtkImageReslice> pImageResliceZ =
      vtkSmartPointer<vtkImageReslice>::New();

  // resample
  vtkSmartPointer<vtkResampleToImage> resampleToImage =
      vtkSmartPointer<vtkResampleToImage>::New();

  //   image mapper
  vtkSmartPointer<vtkImageMapToColors> pImageMapToColorsX =
      vtkSmartPointer<vtkImageMapToColors>::New();
  vtkSmartPointer<vtkImageMapToColors> pImageMapToColorsY =
      vtkSmartPointer<vtkImageMapToColors>::New();
  vtkSmartPointer<vtkImageMapToColors> pImageMapToColorsZ =
      vtkSmartPointer<vtkImageMapToColors>::New();

  //   lookup table
  vtkSmartPointer<vtkWindowLevelLookupTable> pWindowLevelLookupTable =
      vtkSmartPointer<vtkWindowLevelLookupTable>::New();

  // actor
  vtkSmartPointer<vtkImageActor> pImageActorX =
      vtkSmartPointer<vtkImageActor>::New();
  vtkSmartPointer<vtkImageActor> pImageActorY =
      vtkSmartPointer<vtkImageActor>::New();
  vtkSmartPointer<vtkImageActor> pImageActorZ =
      vtkSmartPointer<vtkImageActor>::New();

  //   render
  vtkSmartPointer<vtkRenderer> pRenderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderer> pRendererX = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderer> pRendererY = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderer> pRendererZ = vtkSmartPointer<vtkRenderer>::New();

  //   render window
  vtkSmartPointer<vtkRenderWindow> pRenderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();

  //   interactor
  vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  // ===============================================================

  // read file
  std::string fileName = "test.vtu";
  vtuReader->SetFileName(fileName.c_str());
  vtuReader->Update();

  int extent[6];
  vtuReader->GetUpdateExtent(extent);

  double center[3];
  center[0] = 0.5 * (extent[0] + extent[1]);
  center[1] = 0.5 * (extent[2] + extent[3]);
  center[2] = 0.5 * (extent[4] + extent[5]);

  std::cout << center[0] << std::endl;
  std::cout << center[1] << std::endl;
  std::cout << center[2] << std::endl;

  //   pWindowLevelLookupTable->SetWindow(200);
  //   pWindowLevelLookupTable->SetLevel(100);

  resampleToImage->SetInputConnection(vtuReader->GetOutputPort());
  std::cout << "test" << std::endl;
  //   resampleToImage->SetSamplingDimensions(3, 3, 3);
  resampleToImage->Update();
  std::cout << "test" << std::endl;

  // x slice
  double xSliceX[3] = {1, 0, 0};
  double xSliceY[3] = {0, 1, 0};
  double xSliceZ[3] = {0, 0, 1};
  std::cout << "test" << std::endl;

  pImageResliceX->SetInputConnection(resampleToImage->GetOutputPort());
  std::cout << "test4" << std::endl;

  pImageResliceX->SetOutputDimensionality(2);
  pImageResliceX->SetResliceAxesDirectionCosines(xSliceX, xSliceY, xSliceZ);
  std::cout << "test5" << std::endl;

  pImageResliceX->SetResliceAxesOrigin(center);
  std::cout << "test6" << std::endl;

  pImageResliceX->SetInterpolationModeToLinear();
  std::cout << "test7" << std::endl;

  //   pImageResliceX->Update();
  std::cout << "test8" << std::endl;

  pImageActorX->SetInputData(pImageResliceX->GetOutput());
  pImageActorX->SetPosition(0, 0, 0);
  pImageActorX->Update();
  std::cout << "test6" << std::endl;

  vtkSmartPointer<vtkLookupTable> colorTable =
      vtkSmartPointer<vtkLookupTable>::New();
  colorTable->SetRange(0, 1000);
  colorTable->SetValueRange(0.0, 1.0);
  colorTable->SetSaturationRange(0.0, 0.0);
  colorTable->SetRampToLinear();
  colorTable->Build();
  std::cout << "test7" << std::endl;

  vtkSmartPointer<vtkImageMapToColors> colorMap =
      vtkSmartPointer<vtkImageMapToColors>::New();
  colorMap->SetLookupTable(colorTable);
  colorMap->SetInputConnection(pImageResliceX->GetOutputPort());
  colorMap->Update();

  pImageActorX->SetInputData(colorMap->GetOutput());

  pRendererX->AddActor(pImageActorX);

  pRenderWindow->AddRenderer(pRenderer);
  pRenderWindow->AddRenderer(pRendererX);
  pRenderWindow->AddRenderer(pRendererY);
  pRenderWindow->AddRenderer(pRendererZ);

  pRenderer->SetBackground(1, 1, 1);
  pRendererX->SetBackground(0, 0, 0);
  pRendererY->SetBackground(0, 0, 0);
  pRendererZ->SetBackground(0, 0, 0);

  pRenderer->SetViewport(0, 0, 0.6, 1);
  pRendererX->SetViewport(0.6, 0.66, 1, 1);
  pRendererY->SetViewport(0.6, 0.33, 1, 0.66);
  pRendererZ->SetViewport(0.6, 0, 1, 0.33);

  pRenderWindowInteractor->SetRenderWindow(pRenderWindow);
  pRenderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

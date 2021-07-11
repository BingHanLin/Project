#include <vtkActor.h>
#include <vtkImageReader.h>
#include <vtkImageReader2Factory.h>
#include <vtkJPEGReader.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTexture.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkTextureMapToSphere.h>
#include <vtkTexturedSphereSource.h>
#include <vtkTransformTextureCoords.h>
#include <vtkXMLPolyDataReader.h>

int main(int argc, char* argv[])
{
    vtkNew<vtkNamedColors> colors;

    vtkSmartPointer<vtkJPEGReader> texReader =
        vtkSmartPointer<vtkJPEGReader>::New();
    texReader->SetFileName("texture.jpg");

    vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
    texture->SetInputConnection(texReader->GetOutputPort());

    vtkSmartPointer<vtkXMLPolyDataReader> modelReader =
        vtkSmartPointer<vtkXMLPolyDataReader>::New();
    modelReader->SetFileName("cow.vtp");

    vtkSmartPointer<vtkTextureMapToCylinder> texturemap =
        vtkSmartPointer<vtkTextureMapToCylinder>::New();
    texturemap->SetInputConnection(modelReader->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(texturemap->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->SetTexture(texture);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("Black").GetData());

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("TexturedSphere");

    vtkNew<vtkRenderWindowInteractor> renWinInteractor;
    renWinInteractor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    renWinInteractor->Start();

    return EXIT_SUCCESS;
}
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCubeSource.h>
#include <vtkFloatArray.h>
#include <vtkImageFlip.h>
#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLinearSubdivisionFilter.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkParametricBoy.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricMobius.h>
#include <vtkParametricRandomHills.h>
#include <vtkParametricTorus.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataTangents.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSkybox.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkSliderWidget.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <vtkTexture.h>
#include <vtkTexturedSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkVersion.h>

#include <array>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#if VTK_VERSION_NUMBER >= 90000000000ULL
#define VTK_VER_GE_90 1
#endif

namespace
{
bool VTKVersionOk(unsigned long long const& major,
                  unsigned long long const& minor,
                  unsigned long long const& build);

vtkSmartPointer<vtkPolyData> GetCube();

vtkSmartPointer<vtkTexture> ReadCubeMap();
}  // namespace

int main(int argc, char* argv[])
{
    if (!VTKVersionOk(8, 90, 0))
    {
        std::cerr << "You need VTK version 8.90 or greater to run this program."
                  << std::endl;
        return EXIT_FAILURE;
    }

    vtkSmartPointer<vtkPolyData> polyData = GetCube();

    // Load the cube map
    auto cubemap = ReadCubeMap();

    // Build the pipeline
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(polyData);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    actor->GetProperty()->SetInterpolationToPBR();

    // configure the basic properties

    const double metallicCoefficient = 1.0;
    actor->GetProperty()->SetMetallic(metallicCoefficient);
    const double roughnessCoefficient = 0.05;
    actor->GetProperty()->SetRoughness(roughnessCoefficient);

    vtkNew<vtkOpenGLRenderer> renderer;
    renderer->UseImageBasedLightingOn();
    // renderer->UseSphericalHarmonicsOff();
#if VTK_VER_GE_90
    renderer->SetEnvironmentTexture(cubemap);
#else
    renderer->SetEnvironmentCubeMap(cubemap);
#endif
    renderer->AddActor(actor);

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->SetSize(640, 480);
    renderWindow->Render();
    renderWindow->SetWindowName("Skybox-PBR");
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();
    return EXIT_SUCCESS;
}

namespace
{
bool VTKVersionOk(unsigned long long const& major,
                  unsigned long long const& minor,
                  unsigned long long const& build)
{
    unsigned long long neededVersion =
        10000000000ULL * major + 100000000ULL * minor + build;
#ifndef VTK_VERSION_NUMBER
    vtkNew<vtkVersion>();
    ver;
    unsigned long long vtk_version_number =
        10000000000ULL * ver->GetVTKMajorVersion() +
        100000000ULL * ver->GetVTKMinorVersion() + ver->GetVTKBuildVersion();
    if (vtk_version_number >= neededVersion)
    {
        return true;
    }
    return false;
#else
    if (VTK_VERSION_NUMBER >= neededVersion)
    {
        return true;
    }
    return false;
#endif
}

vtkSmartPointer<vtkTexture> ReadCubeMap()
{
    const std::string root = "./";

    vtkNew<vtkTexture> texture;
    texture->CubeMapOn();

    // Build the file names.
    std::vector<std::string> paths;
    const std::vector<std::string> fileNames = {
        "posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"};

    for (auto f : fileNames)
    {
        paths.push_back(root + f);
    }
    auto i = 0;
    for (auto const& fn : paths)
    {
        // Read the images
        vtkNew<vtkImageReader2Factory> readerFactory;
        vtkSmartPointer<vtkImageReader2> imgReader;
        imgReader.TakeReference(readerFactory->CreateImageReader2(fn.c_str()));
        imgReader->SetFileName(fn.c_str());

        vtkNew<vtkImageFlip> flip;
        flip->SetInputConnection(imgReader->GetOutputPort());
        flip->SetFilteredAxis(1);  // flip y axis
        texture->SetInputConnection(i, flip->GetOutputPort(0));
        ++i;
    }
    texture->MipmapOn();
    texture->InterpolateOn();

    return texture;
}

vtkSmartPointer<vtkPolyData> GetCube()
{
    vtkNew<vtkCubeSource> surface;

    // Triangulate
    vtkNew<vtkTriangleFilter> triangulation;
    triangulation->SetInputConnection(surface->GetOutputPort());
    // Subdivide the triangles
    vtkNew<vtkLinearSubdivisionFilter> subdivide;
    subdivide->SetInputConnection(triangulation->GetOutputPort());
    subdivide->SetNumberOfSubdivisions(3);
    // Now the tangents
    vtkNew<vtkPolyDataTangents> tangents;
    tangents->SetInputConnection(subdivide->GetOutputPort());
    tangents->Update();
    return tangents->GetOutput();
}

}  // namespace
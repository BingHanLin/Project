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
/**
 * Show the command lime parameters.
 *
 * @param fn: The program name.
 *
 * @return A string describing the usage.
 */
std::string ShowUsage(std::string fn);

/**
 * Check the VTK version.
 *
 * @param major: Major version.
 * @param major: Minor version.
 * @param major: Build version.
 *
 * @return True if the requested VTK version is greater or equal to the actual
 * VTK version.
 */
bool VTKVersionOk(unsigned long long const& major,
                  unsigned long long const& minor,
                  unsigned long long const& build);

// Some sample surfaces to try.
vtkSmartPointer<vtkPolyData> GetBoy();
vtkSmartPointer<vtkPolyData> GetMobius();
vtkSmartPointer<vtkPolyData> GetRandomHills();
vtkSmartPointer<vtkPolyData> GetTorus();
vtkSmartPointer<vtkPolyData> GetSphere();
vtkSmartPointer<vtkPolyData> GetCube();

/**
 * Generate u, v texture coordinates on a parametric surface.
 *
 * @param uResolution: u resolution
 * @param vResolution: v resolution
 * @param pd: The polydata representing the surface.
 *
 * @return The polydata with the texture coordinates added.
 */
vtkSmartPointer<vtkPolyData> UVTcoords(const float& uResolution,
                                       const float& vResolution,
                                       vtkSmartPointer<vtkPolyData> pd);

/**
 * Read six images forming a cubemap.
 *
 * @param folderRoot: The folder where the cube maps are stored.
 * @param fileNames: The names of the cubemap files.
 *
 * @return The cubemap texture.
 */
vtkSmartPointer<vtkTexture> ReadCubeMap(std::string const& folderRoot);

class SliderCallbackMetallic : public vtkCommand
{
   public:
    static SliderCallbackMetallic* New()
    {
        return new SliderCallbackMetallic;
    }
    virtual void Execute(vtkObject* caller, unsigned long, void*)
    {
        vtkSliderWidget* sliderWidget =
            reinterpret_cast<vtkSliderWidget*>(caller);
        double value = static_cast<vtkSliderRepresentation2D*>(
                           sliderWidget->GetRepresentation())
                           ->GetValue();
        this->property->SetMetallic(value);
    }
    SliderCallbackMetallic() : property(nullptr)
    {
    }
    vtkProperty* property;
};

class SliderCallbackRoughness : public vtkCommand
{
   public:
    static SliderCallbackRoughness* New()
    {
        return new SliderCallbackRoughness;
    }
    virtual void Execute(vtkObject* caller, unsigned long, void*)
    {
        vtkSliderWidget* sliderWidget =
            reinterpret_cast<vtkSliderWidget*>(caller);
        double value = static_cast<vtkSliderRepresentation2D*>(
                           sliderWidget->GetRepresentation())
                           ->GetValue();
        this->property->SetRoughness(value);
    }
    SliderCallbackRoughness() : property(nullptr)
    {
    }
    vtkProperty* property;
};

struct SliderProperties
{
    // Set up the sliders
    double tubeWidth{0.008};
    double sliderLength{0.008};
    double titleHeight{0.025};
    double labelHeight{0.025};

    double minimumValue{0.0};
    double maximumValue{1.0};
    double initialValue{0.0};

    std::array<double, 2> p1{0.1, 0.1};
    std::array<double, 2> p2{0.9, 0.1};

    std::string title{""};

    std::string titleColor{"MistyRose"};
    std::string valueColor{"Cyan"};
    std::string sliderColor{"Coral"};
    std::string selectedColor{"Lime"};
    std::string barColor{"PeachPuff"};
    std::string barEndsColor{"Thistle"};
};

vtkSmartPointer<vtkSliderWidget> MakeSliderWidget(
    SliderProperties const& properties);

}  // namespace

int main(int argc, char* argv[])
{
    if (!VTKVersionOk(8, 90, 0))
    {
        std::cerr << "You need VTK version 8.90 or greater to run this program."
                  << std::endl;
        return EXIT_FAILURE;
    }
    if (argc < 2)
    {
        std::cout << ShowUsage(argv[0]) << std::endl;
        return EXIT_FAILURE;
    }

    std::string desiredSurface = "boy";
    if (argc > 2)
    {
        desiredSurface = argv[2];
    }
    std::transform(desiredSurface.begin(), desiredSurface.end(),
                   desiredSurface.begin(),
                   [](char c) { return std::tolower(c); });
    std::map<std::string, int> availableSurfaces = {
        {"boy", 0},   {"mobius", 1}, {"randomhills", 2},
        {"torus", 3}, {"sphere", 4}, {"cube", 5}};
    if (availableSurfaces.find(desiredSurface) == availableSurfaces.end())
    {
        desiredSurface = "boy";
    }
    vtkSmartPointer<vtkPolyData> source;
    switch (availableSurfaces[desiredSurface])
    {
        case 1:
            source = GetMobius();
            break;
        case 2:
            source = GetRandomHills();
            break;
        case 3:
            source = GetTorus();
            break;
        case 4:
            source = GetSphere();
            break;
        case 5:
            source = GetCube();
            break;
        case 0:
        default:
            source = GetBoy();
    };

    // Split  path into its components.
    auto splitPath = [](std::string path) {
        std::replace(path.begin(), path.end(), '\\', '/');

        std::regex regex("/");

        std::vector<std::string> elements(
            std::sregex_token_iterator(path.begin(), path.end(), regex, -1),
            std::sregex_token_iterator());
        return elements;
    };

    // Build a string from a vector of strings using a separator.
    auto join = [](std::vector<std::string> strings, std::string separator) {
        std::string res = std::accumulate(
            std::begin(strings), std::end(strings), std::string(),
            [&](std::string& ss, std::string& s) {
                return ss.empty() ? s : ss + separator + s;
            });
        return res;
    };

    // A map of the skybox folder name and the skybox files in it.
    std::map<std::string, std::vector<std::string>> skyboxFiles{
        {"skybox0",
         {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg",
          "back.jpg"}},
        {"skybox1",
         {"skybox-px.jpg", "skybox-nx.jpg", "skybox-py.jpg", "skybox-ny.jpg",
          "skybox-pz.jpg", "skybox-nz.jpg"}},
        {"skybox2",
         {"posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg",
          "negz.jpg"}}};

    std::vector<std::string> path = splitPath(std::string(argv[1]));
    // std::string root = join(path, "/");

    // Load the cube map
    auto cubemap = ReadCubeMap(argv[1]);

    // Load the skybox
    // Read it again as there is no deep copy for vtkTexture
    auto skybox = ReadCubeMap(argv[1]);
    skybox->InterpolateOn();
    skybox->MipmapOn();
    skybox->RepeatOff();
    skybox->EdgeClampOn();

    vtkNew<vtkNamedColors> colors;

    // Set the background color.
    std::array<unsigned char, 4> bkg{{26, 51, 102, 255}};
    colors->SetColor("BkgColor", bkg.data());

    vtkNew<vtkOpenGLRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    // Lets use a smooth metallic surface
    auto metallicCoefficient = 1.0;
    auto roughnessCoefficient = 0.05;

    auto slwP = SliderProperties();
    slwP.initialValue = metallicCoefficient;
    slwP.title = "Metallicity";

    auto sliderWidgetMetallic = MakeSliderWidget(slwP);
    sliderWidgetMetallic->SetInteractor(interactor);
    sliderWidgetMetallic->SetAnimationModeToAnimate();
    sliderWidgetMetallic->EnabledOn();

    slwP.initialValue = roughnessCoefficient;
    slwP.title = "Roughness";
    slwP.p1[0] = 0.1;
    slwP.p1[1] = 0.9;
    slwP.p2[0] = 0.9;
    slwP.p2[1] = 0.9;

    auto sliderWidgetRoughness = MakeSliderWidget(slwP);
    sliderWidgetRoughness->SetInteractor(interactor);
    sliderWidgetRoughness->SetAnimationModeToAnimate();
    sliderWidgetRoughness->EnabledOn();

    // Build the pipeline
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(source);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    renderer->UseImageBasedLightingOn();
#if VTK_VER_GE_90
    renderer->SetEnvironmentTexture(cubemap);
#else
    renderer->SetEnvironmentCubeMap(cubemap);
#endif
    actor->GetProperty()->SetInterpolationToPBR();

    // configure the basic properties
    actor->GetProperty()->SetColor(colors->GetColor4d("White").GetData());
    actor->GetProperty()->SetMetallic(metallicCoefficient);
    actor->GetProperty()->SetRoughness(roughnessCoefficient);

    // Create the slider callbacks to manipulate metallicity and roughness
    vtkNew<SliderCallbackMetallic> callbackMetallic;
    callbackMetallic->property = actor->GetProperty();
    vtkNew<SliderCallbackRoughness> callbackRoughness;
    callbackRoughness->property = actor->GetProperty();

    sliderWidgetMetallic->AddObserver(vtkCommand::InteractionEvent,
                                      callbackMetallic);
    sliderWidgetRoughness->AddObserver(vtkCommand::InteractionEvent,
                                       callbackRoughness);

    renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
    renderer->AddActor(actor);

    vtkNew<vtkSkybox> skyboxActor;
    skyboxActor->SetTexture(skybox);
    renderer->AddActor(skyboxActor);

    // renderer->UseSphericalHarmonicsOff();

    renderWindow->SetSize(640, 480);
    renderWindow->Render();
    renderWindow->SetWindowName("Skybox-PBR");

    vtkNew<vtkAxesActor> axes;

    vtkNew<vtkOrientationMarkerWidget> widget;
    double rgba[4]{0.0, 0.0, 0.0, 0.0};
    colors->GetColor("Carrot", rgba);
    widget->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(interactor);
    widget->SetViewport(0.0, 0.2, 0.2, 0.4);
    widget->SetEnabled(1);
    widget->InteractiveOn();

    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();
    return EXIT_SUCCESS;
}

namespace
{
std::string ShowUsage(std::string fn)
{
    // Remove the folder (if present) then remove the extension in this order
    // since the folder name may contain periods.
    auto last_slash_idx = fn.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
    {
        fn.erase(0, last_slash_idx + 1);
    }
    auto period_idx = fn.rfind('.');
    if (std::string::npos != period_idx)
    {
        fn.erase(period_idx);
    }
    std::ostringstream os;
    os << "\nusage: " << fn << " path [surface]\n\n"
       << "Demonstrates physically based rendering, image based lighting and a "
          "skybox.\n\n"
       << "positional arguments:\n"
       << "  path        The path to the cubemap files e.g. skyboxes/skybox2/\n"
       << "  surface     The surface to use. Boy's surface is the default.\n\n"
       << "Physically based rendering sets color, metallicity and roughness of "
          "the object.\n"
       << "Image based lighting uses a cubemap texture to specify the "
          "environment.\n"
       << "A Skybox is used to create the illusion of distant "
          "three-dimensional "
          "surroundings.\n"
       << "\n"
       << std::endl;
    return os.str();
}

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

vtkSmartPointer<vtkTexture> ReadCubeMap(std::string const& folderRoot)
{
    auto root = folderRoot;
    if (folderRoot.back() != '/')
    {
        root += '/';
    }

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

vtkSmartPointer<vtkPolyData> GetBoy()
{
    auto uResolution = 51;
    auto vResolution = 51;
    vtkNew<vtkParametricBoy> surface;

    vtkNew<vtkParametricFunctionSource> source;
    source->SetUResolution(uResolution);
    source->SetVResolution(vResolution);
    source->SetParametricFunction(surface);
    source->Update();
    // Build the tcoords
    auto pd = UVTcoords(uResolution, vResolution, source->GetOutput());
    // Now the tangents
    vtkNew<vtkPolyDataTangents> tangents;
    tangents->SetInputData(pd);
    tangents->Update();
    return tangents->GetOutput();
}

vtkSmartPointer<vtkPolyData> GetMobius()
{
    auto uResolution = 51;
    auto vResolution = 51;
    vtkNew<vtkParametricMobius> surface;
    surface->SetMinimumV(-0.25);
    surface->SetMaximumV(0.25);

    vtkNew<vtkParametricFunctionSource> source;
    source->SetUResolution(uResolution);
    source->SetVResolution(vResolution);
    source->SetParametricFunction(surface);
    source->Update();
    // Build the tcoords
    auto pd = UVTcoords(uResolution, vResolution, source->GetOutput());
    // Now the tangents
    vtkNew<vtkPolyDataTangents> tangents;
    tangents->SetInputData(pd);
    tangents->Update();

    vtkNew<vtkTransform> transform;
    transform->RotateX(90.0);
    vtkNew<vtkTransformPolyDataFilter> transformFilter;
    transformFilter->SetInputConnection(tangents->GetOutputPort());
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    return transformFilter->GetOutput();
}

vtkSmartPointer<vtkPolyData> GetRandomHills()
{
    auto uResolution = 51;
    auto vResolution = 51;
    vtkNew<vtkParametricRandomHills> surface;
    surface->SetRandomSeed(1);
    surface->SetNumberOfHills(30);
    // If you want a plane
    // surface->SetHillAmplitude(0);

    vtkNew<vtkParametricFunctionSource> source;
    source->SetUResolution(uResolution);
    source->SetVResolution(vResolution);
    source->SetParametricFunction(surface);

    source->Update();
    // Build the tcoords
    auto pd = UVTcoords(uResolution, vResolution, source->GetOutput());
    // Now the tangents
    vtkNew<vtkPolyDataTangents> tangents;
    tangents->SetInputData(pd);
    tangents->Update();

    vtkNew<vtkTransform> transform;
    transform->RotateZ(180.0);
    transform->RotateX(90.0);
    vtkNew<vtkTransformPolyDataFilter> transformFilter;
    transformFilter->SetInputConnection(tangents->GetOutputPort());
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    return transformFilter->GetOutput();
}

vtkSmartPointer<vtkPolyData> GetTorus()
{
    auto uResolution = 51;
    auto vResolution = 51;
    vtkNew<vtkParametricTorus> surface;

    vtkNew<vtkParametricFunctionSource> source;
    source->SetUResolution(uResolution);
    source->SetVResolution(vResolution);
    source->SetParametricFunction(surface);

    source->Update();
    // Build the tcoords
    auto pd = UVTcoords(uResolution, vResolution, source->GetOutput());
    // Now the tangents
    vtkNew<vtkPolyDataTangents> tangents;
    tangents->SetInputData(pd);
    tangents->Update();

    vtkNew<vtkTransform> transform;
    transform->RotateX(90.0);
    vtkNew<vtkTransformPolyDataFilter> transformFilter;
    transformFilter->SetInputConnection(tangents->GetOutputPort());
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    return transformFilter->GetOutput();
}

vtkSmartPointer<vtkPolyData> GetSphere()
{
    auto thetaResolution = 32;
    auto phiResolution = 32;
    vtkNew<vtkTexturedSphereSource> surface;
    surface->SetThetaResolution(thetaResolution);
    surface->SetPhiResolution(phiResolution);

    // Now the tangents
    vtkNew<vtkPolyDataTangents> tangents;
    tangents->SetInputConnection(surface->GetOutputPort());
    tangents->Update();
    return tangents->GetOutput();
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

vtkSmartPointer<vtkPolyData> UVTcoords(const float& uResolution,
                                       const float& vResolution,
                                       vtkSmartPointer<vtkPolyData> pd)
{
    float u0 = 1.0;
    float v0 = 0.0;
    float du = 1.0 / (uResolution - 1.0);
    float dv = 1.0 / (vResolution - 1.0);
    vtkIdType numPts = pd->GetNumberOfPoints();
    vtkNew<vtkFloatArray> tCoords;
    tCoords->SetNumberOfComponents(2);
    tCoords->SetNumberOfTuples(numPts);
    tCoords->SetName("Texture Coordinates");
    vtkIdType ptId = 0;
    float u = u0;
    for (auto i = 0; i < uResolution; ++i)
    {
        float v = v0;
        for (auto j = 0; j < vResolution; ++j)
        {
            float tc[2]{u, v};
            tCoords->SetTuple(ptId, tc);
            v += dv;
            ptId++;
        }
        u -= du;
    }
    pd->GetPointData()->SetTCoords(tCoords);
    return pd;
}

vtkSmartPointer<vtkSliderWidget> MakeSliderWidget(
    SliderProperties const& properties)
{
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkSliderRepresentation2D> slider;

    slider->SetMinimumValue(properties.minimumValue);
    slider->SetMaximumValue(properties.maximumValue);
    slider->SetValue(properties.initialValue);
    slider->SetTitleText(properties.title.c_str());

    slider->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
    slider->GetPoint1Coordinate()->SetValue(properties.p1[0], properties.p1[1]);
    slider->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
    slider->GetPoint2Coordinate()->SetValue(properties.p2[0], properties.p2[1]);

    slider->SetTubeWidth(properties.tubeWidth);
    slider->SetSliderLength(properties.sliderLength);
    slider->SetTitleHeight(properties.titleHeight);
    slider->SetLabelHeight(properties.labelHeight);

    // Set the color properties
    // Change the color of the bar.
    slider->GetTubeProperty()->SetColor(
        colors->GetColor3d(properties.barColor).GetData());
    // Change the color of the ends of the bar.
    slider->GetCapProperty()->SetColor(
        colors->GetColor3d(properties.barEndsColor).GetData());
    // Change the color of the knob that slides.
    slider->GetSliderProperty()->SetColor(
        colors->GetColor3d(properties.sliderColor).GetData());
    // Change the color of the knob when the mouse is held on it.
    slider->GetSelectedProperty()->SetColor(
        colors->GetColor3d(properties.selectedColor).GetData());
    // Change the color of the text displaying the value.
    slider->GetLabelProperty()->SetColor(
        colors->GetColor3d(properties.valueColor).GetData());

    vtkNew<vtkSliderWidget> sliderWidget;
    sliderWidget->SetRepresentation(slider);

    return sliderWidget;
}

}  // namespace
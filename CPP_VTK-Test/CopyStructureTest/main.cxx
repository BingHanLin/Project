
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridReader.h>

#include <iostream>
#include <stdlib.h>
vtkSmartPointer<vtkUnstructuredGrid> readUnstructuredGrid(
    const std::string fileName)
{
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
        vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();

    return reader->GetOutput();
}

int main(int, char*[])
{
    vtkSmartPointer<vtkUnstructuredGrid> grid = readUnstructuredGrid("0.vtu");

    std::cout << "grid GetCellData number: "
              << grid->GetCellData()->GetNumberOfArrays() << std::endl;
    std::cout << "grid GetPointData number: "
              << grid->GetPointData()->GetNumberOfArrays() << std::endl;

    // The memory usage won't increase with CopyStructure
    vtkSmartPointer<vtkUnstructuredGrid> grid2 =
        vtkSmartPointer<vtkUnstructuredGrid>::New();
    grid2->CopyStructure(grid);

    for (int i = 0; i < grid->GetCellData()->GetNumberOfArrays(); i++)
    {
        auto oneArray = grid->GetCellData()->GetArray(i);
        grid2->GetCellData()->AddArray(oneArray);
    }

    std::cout << "grid2 GetCellData number: "
              << grid2->GetCellData()->GetNumberOfArrays() << std::endl;
    std::cout << "grid2 GetPointData number: "
              << grid2->GetPointData()->GetNumberOfArrays() << std::endl;

    system("pause");

    return EXIT_SUCCESS;
}
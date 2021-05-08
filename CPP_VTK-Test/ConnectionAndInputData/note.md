https://stackoverflow.com/questions/47773443/vtk-pipeline-update

VTK pipelines are demand-driven pipelines. They do not update automatically even if one of the elements of the pipeline is modified. We need to explicitly call the Update() function on the last vtkAlgorithm( or its derived class object) of the pipeline to update the entire pipeline. The correct way to set up a pipeline is when we are connecting two objects which are derived from vtkAlgorithm type is to use

    ```currAlgoObj->SetInputConnection(prevAlgoObj->GetOutputPort())```

instead of

    ```currAlgoObj->SetInputData(prevAlgo->GetOutput())```
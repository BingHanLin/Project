https://stackoverflow.com/questions/47773443/vtk-pipeline-update

VTK pipelines are demand-driven pipelines. They do not update automatically even if one of the elements of the pipeline is modified. We need to explicitly call the Update() function on the last vtkAlgorithm( or its derived class object) of the pipeline to update the entire pipeline. The correct way to set up a pipeline is when we are connecting two objects which are derived from vtkAlgorithm type is to use

    ```currAlgoObj->SetInputConnection(prevAlgoObj->GetOutputPort())```

instead of

    ```currAlgoObj->SetInputData(prevAlgo->GetOutput())```



VTK architecture is based on a **demand-driven** pipeline architecture (a visualization network)
* Applications must first create a network, and then EXECUTE it
* Typically requires a **Render()** (sent to the rendering window) or an **Update()** (sent
to a filter in the pipeline).
* Just instantiating the objects and hooking them together won't do anything – **YOU HAVE TO REQUEST DATA** to get data

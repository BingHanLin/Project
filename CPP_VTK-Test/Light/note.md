如果没有指定，vtkRenderer会自动地创建默认的光照和相机。VTK里用类vtkLight来表示渲染场景中的光照。与现实中的灯光类似，VTK中的vtkLight实例也可以打开、关闭，设置光照的颜色，照射位置(即焦点)，光照所在的位置，强度等等。

vtkLight可以分为位置光照(Positional Light，即聚光灯)和方向光照(Direction Light)。
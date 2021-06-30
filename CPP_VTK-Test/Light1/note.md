如果沒有指定，vtkRenderer會自動地創建默認的光照和攝像機。VTK裡用類vtkLight來表示渲染場景中的光照(white ambient light, two-sided lighting turned on)。與現實中的渲染方式，VTK 中 的vtkLight實例也可以打開、關閉，設置光源的顏色，位置（即焦點），光源的位置，強度等等。

通過vtkVolumeProperty可以設置物體渲染效果（Shading）。效果主要受光環境、反射光特性、反射光係數和高光強度四個參數屬性影響。

* vtkVolumeProperty::SetAmbient() 設置環境光係數
* vtkVolumeProperty::SetDiffuse() 設置散射光係數
* vtkVolumeProperty::SetSpecular() 設置反射光係數

一般情況下，三個係數的和應該為1。但是，為了提高速度，三值之和會大於1。當**環境光係數**占主導時，陰影效果不明顯。當**散射光係數**占主導時，顯示效果會比較粗燥；但**反射光係數**占主導時，顯示效果會比較光滑。

另外，還有一個參數是高光強度（Specular Power）,用於控制體繪製的外觀平滑程度。
* SetSpecularPower() 設置外觀平滑程度。 

vtkVolumeProperty中默認是是關閉陰影效果的，因此需要顯式調用
* ShadeOn() 設置開陰影效果

> 需要注意的是，開啟陰影效果對vtkUnstructuredGrid數據類型是無效的


* vtkLight: 可以分為位置光源（Positional Light，即聚光燈）和方向光源（Direction Light）。

參考: https://shenchunxu.blog.csdn.net/article/details/54917295
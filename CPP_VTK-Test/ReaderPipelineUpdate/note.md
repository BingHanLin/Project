https://blog.csdn.net/shenziheng1/article/details/53343103

通常，我们不用显性地去调用Update()函数，因为在渲染引擎的最后，当我们调用Render()函数的时候，Actor就会收到渲染请求，接着Actor会请求Mapper给它发送数据，而Mapper又会请求上一层的Filter的数据，Filter最后去请求Source给它数据，于是，整条管线就被执行。除非像上面的代码段里列出的，读入数据以后，中间想要输出某些信息，在得到这些信息之前，你就应该显性地调用Update()函数。管线的执行过程大致如下图所示。

https://stackoverflow.com/questions/24066010/in-visualization-toolkit-which-types-of-objects-need-update-and-modified-to



Update() is required when you want to use an object before the pipeline updates it for you. If, on the other hand, you are going to take the reader, attach it to a mapper, attach the mapper to an actor, and display the actor in the renderwindow, then at the time when the renderer says "Ok, now I need the data that drives this whole chain", the pipeline will go back and call Update() on the reader. This is the whole reason for/benefit of the pipeline execution model.

Modified() is required when you want to inform the pipeline "on the next pass, you need to re-process this object". This is done internally by most of the Set* functions, so I guess you just have to look at the implementation to see if Modified() is getting called or not by whatever function you've called that you expect to take effect the next pass through the pipeline.
https://blog.csdn.net/shenziheng1/article/details/53343103

通常，我们不用显性地去调用Update()函数，因为在渲染引擎的最后，当我们调用Render()函数的时候，Actor就会收到渲染请求，接着Actor会请求Mapper给它发送数据，而Mapper又会请求上一层的Filter的数据，Filter最后去请求Source给它数据，于是，整条管线就被执行。除非像上面的代码段里列出的，读入数据以后，中间想要输出某些信息，在得到这些信息之前，你就应该显性地调用Update()函数。管线的执行过程大致如下图所示。
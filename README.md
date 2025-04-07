# OpenGL学习

#### OpenGL是什么

OpenGL的是一个图形API，API代表应用程序编程接口，它本质上是我们可以调用来执行特定任务的函数。在这种情况下，由于OpenGL是一个图形API，它允许我们进行图形处理，具体来说OpenGL允许我们来实际访问和控制我们的GPU。OpenGL是一个规范，而不是一种框架或代码，没有实际的源代码，真正的源代码由显卡驱动提供。OpenGL有一定跨平台能力。

#### 传统OpenGL和现代OpenGL

OpenGL于90年代发布，那时的GPU是不可编程的，不能随心所欲，尤其在使用这些底层的API时。但现在人们可以很大程度去控制它，制造商给了程序员和开发者更多的控制权，这显然很好，因为我们可以为它做更多的优化。现代的OpenGL更像一个循环，一个低级的法则，它能给你比之前更多的控制权。传统的OpenGL更像是一套程序。
所以比如要画一个三角形，并且想要添加光源，基本上就是让光源等于true，就可以激活OpenGL的光源，然后就再告诉OpenGL光源加在哪里就可以了。所以传统的OpenGL真的很像一套预设，很容易使用代码比较少。但这样就会造成不能给你太多的控制权。但其实我们是想要更多的控制权的。

**传统OpenGL和现代OpenGL之间最大的区别是可编程的着色器。** 着色器是程序，它是运行在GPU上的代码。当我们使用cpp或Java等语言写代码时，这些代码是运行在CPU上的，但当我们开始处理图形的大部分时间里，我们想要更为精确的控制显卡运行，我们可能要将大部分代码从CPU转到GPU上，因为它在GPU上运行更快，这就是着色器存在的意义，允许我们在GPU上运行代码。

#### OpenGL中的顶点缓冲区和绘制三角形 

要使用现代OpenGL绘制绘制一个三角形，需要完成几件事情。首先，我们需要创建一个顶点缓冲区（vertor buffer）。同时还需要创建一个着色器（shader）。

buffer就是一块用来存字节的内存，一个内存字节数组。顶点缓冲区跟C++中的字符数组的内存缓冲区不太一样，区别在于它是 OpenGL 中的内存缓冲区，这意味着它实际上在显卡上，在我们的VRAM（显存）中，也就是Video RAM。所以使用现代的OpenGL表示三角形的话，需要定义一些数据来表示三角形，然后把它们放到显卡（GPU）的 VRAM 中，还需要发出 DrawCall 指令（这是一个绘制指令），（意思就是告诉电脑，你的显存中有一堆数据，读取它，并把它绘制在屏幕上），实际上，我们还需要告诉显卡，如何读取和解释这些数据，以及如何把它实际显示到我们屏幕上。当我们在CPU这边做了所有事情（我们用C++写的东西都是在CPU上运行的）。当我们写完这些东西，还要用某种方式告诉显卡，一旦从CPU发出了DrawCall指令，且一旦从显卡读到了这些数据，我希望你在屏幕上给我绘制出三角形。因为我们需要告诉显卡它要做什么，所以需要对显卡编程，这就是着色器的作用。
**着色器是一个运行在显卡上的程序，是一堆我们可以编写的且可以在显卡上运行的代码。** 它可以在显卡上以一种非常特殊又非常强大的方式运行。

**注意： OpenGL具体的操作就是一个状态机，不需要把它看成对象或类似的东西。** 我们所做的就是设置一系列的状态，然后当我们说一些事情，比如说给我绘制个三角形，那是与上下文相关的（先告诉它绘制三角形所需要的数据，然后它才去绘制，这种是分状态的）。换句话说，选择一个缓冲区和一个着色器，让电脑帮我绘制一个三角形。电脑会根据你选择的缓冲区和着色器，决定绘制什么样的三角形，绘制在哪里等等。这就是OpenGL的原理，它是一个状态机。

**顶点缓冲区的好处：** 顶点缓冲区包含这些顶点的数据，传到OpenGL的VRAM，然后发出一个DrawCall指令，电脑就可以根据缓冲区画出图形了。我们可以在GL渲染循环外面，先定义好缓冲区，在GL渲染循环中，就可以绘制已经存在的数据了。如果出于某些原因，我们需要改变数据帧或者其他东西，我们也可以通过更新缓冲区来做到这一点。

查OpenGL文档的链接： https://docs.gl/。

```c++
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /*向OpenGL提供数据所需的所有代码*/
    
    float positions[6] = {      //三个顶点画一个三角形
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };
    

    //在OpenGL中生成的所有东西都会被分配一个唯一的标识符，它只是一个整数。比如0、1、2。0通常是一个无效状态，但不都是。
    //但基本上会得到一个数字，比如1、2、3等，这是实际对象的id，不管它是顶点缓冲区、顶点数组、着色器还是其他东西，都会得到一个整数来代表它。
    //当想要使用这个对象时，就用这个数字
    unsigned int buffer;    //定义一个缓冲区，无符号int类型，用来存放缓冲区的地址
    glGenBuffers(1, &buffer);   //定义顶点缓冲区，然后指定要多少个缓冲区，因为我们只需要一个，所以输入 1，第二个参数需要一个无符号整型指针。因为这个函数的返回类型是 void，所以函数不返回生成的缓冲区id，我们要给它提供一个整数（指针）。函数会把id写入这个整数的内存，这就是为什么需要指针。
    glBindBuffer(GL_ARRAY_BUFFER, buffer);   //绑定缓冲区，第一个参数是目标，GL_ARRAY_BUFFER表示这是一个数组；第二个参数是我们要绑定的缓冲区
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),positions,GL_STATIC_DRAW);      //第一个参数是target；第二个参数是指我们希望缓冲区多大或者数据有多大；
    
    /*向OpenGL提供数据所需的所有代码*/   
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))   //glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后游戏循环便结束了，之后为我们就可以关闭应用程序了。
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //传统的OpenGL绘制一个三角形，只需要下列函数,通过指定三个顶点来画三角形
        /*
            glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);     //按住alt+shift
            glVertex2f( 0.0f,  0.5f);
            glVertex2f( 0.5f, -0.5f);
            glEnd();
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);    //交换颜色缓冲

        /* Poll for and process events */
        glfwPollEvents();     //glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），然后调用对应的回调函数（可以通过回调方法手动设置）。我们一般在游戏循环的开始调用事件处理函数。
    }

    glfwTerminate();    //释放glfw分配的内存
    return 0;
}

```



#### OpenGL中的顶点属性与布局

为了让我们的三角形能被渲染出来，我们还遗漏了两个主要部分：顶点属性和着色器。

**顶点属性**即顶点数据，它指定了每个顶点的各种属性数据。 在OpenGL ES1.1中，顶点属性有四个预定义的名字：position（位置）, normal（法线）, color（颜色）, 和 texture coordinates（纹理坐标）。 在OpenGL ES2.0中，用户必须定义“顶点属性的名字”。 常量顶点属性对所有顶点都是一样的。 因此只需要指定一个值就可以应用于所有顶点。
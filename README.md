Anitools
========
Anitools hopes to be a suite of crossplatform, open source, simple tools writen in C++/Qt whose sole purpose is automating a series of tedious, time consuming, tasks present in the traditional animation pipeline.

Who?
----
The main developer of Anitools is Deif Lou, an independent illustrator ([tumblr blog](http://deiflou.tumblr.com)).

What?
----
The goal is to make a series of simple applications that will help in the development of traditional, hand-drawn animation. Each application will solve one of the following problems:

- To apply a list of effects to a list of images (useful if you want to transform/filter/modify a folder with hundreds of frames). The goal here is to provide the application with a set of filters that will help to achieve animation specific effects (non-uniform illumination correction, keying and so on).
This tool is under development. Here are some snapshots:

  [![](/doc/images/readme01-thumb.jpg)](/doc/images/readme01.jpg)
 
  The previous image shows the use of a generic color correction filter list.

  [![](/doc/images/readme02-thumb.jpg)](/doc/images/readme02.jpg) [![](/doc/images/readme03-thumb.jpg)](/doc/images/readme03.jpg)
 
  The previous images show a generic illumination correction filter list. The first image shows levels correction with no intensity inhomogeneity (IIH) correction. The second image applies a IIH correction filter before adjusting the levels.

- To make a capturing application, with options for digital camera, webcam, and others. The application must have a simple yet versatile timeline interface (multilayer, sound layers, xsheet functionality, etc.). It must also have some other helper tools like arc planning and exporting capabilities.

  This is not being developed yet.

- To make some utility applications such as a powerful palette/color chart editor; a chronometer with x-sheet functionality; an arc planning app with motion tracking capabilities.
  
  This small (yet powerful) applications can also be implemented as Android/iPhone apps.
- To make other more complex applications like a vectorization tool that converts the frames to a geometric representation and finds correspondences between frames; an autocoloring tool that automatically paints the frames besed on one reference frame; an inbetweening tool that creates the mising frames between two given frames.

  This tools require a lot of research and knowledge, so they will be the last ones.

Why?
----
There are two reasons why I am making these applications. The first one is to help me in some specific tasks and minimize the amount of time required to do some repetitive work as I animate. The second one is to help other people that want to make an animation on their own and to minimize the time they spend in tedious tasks, like coloring and capturing.

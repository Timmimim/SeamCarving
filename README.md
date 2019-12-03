# SeamCarving
C++ implementation of Seam Carving for content-aware image resizing and manipulation, as first presented in [1].

Basic Qt UI setup courtesy of JProf. Dr. Benjamin Risse and Andreas Nienkötter as part of their Computer Vision lecture at the Faculty of Computer Science at WWU Münster.

### How to use

This implementation comes with a simple-to-use graphic UI, that allows the user to select and load an image they would like to resize, and a number of rows and columns in the image to _carve_ or _duplicate_. 

(Note that Image Extension by Seam Duplication is only supported for one dimension at a time!)

The interface provides an option to display the computed seams. Horizontal seams are marked in blue, while vertical seams are displayed in red. Overlapping pixels are marked in pink, as the result of  the red and blue RGB channels being set to their respective max value.

For convenience, our tool also displays the Original Image and it's corresponding Energy Image, as well as the resulting Carved or Extended Image.

### Compile and run

\# 	qmake
\# 	make
\# 	./SeamCarving


### Dependencies

-	OpenCV 4.1^ (should work with previous Versions after minor adjustments)
-	Qt4 or higher (also qmake)
-	g++ Compiler (in a version able to compile the C++17 ISO standard)



Note: Only run and tested on Linux machines. Should work on any UNIX machine when library paths are adjusted. Could, maybe, hypothetically, on a good day, compile and run on a Windows machine, but, you know... :-) Anyway, didn't test that, is what we are saying. 

### Future work / Planned extensions

**Save** option for the resized image.

### Sources

[1]	Avidan, Shai & Shamir, Ariel. (2007). Seam carving for content-aware image resizing. ACM Trans. Graph.. 26. 10. 10.1145/1276377.1276390. 
# FSF3565 Project 4 - Grid Functions

C++ implementation of a framework to compute arithmetic and differential
operators of over a given domain. The domain will be discretized to allow for
numerical approximation of the differential operators - support for irregular
grids is provided.

## Running the code

To reproduce the full report and results start by going to ```bin``` and run

```sh
g++ *.cpp */*.cpp */*.h -o main.o -std=c++17 -Wall
```
after compilation do
```sh
./main.o<<EOF
50
20
EOF
```
this will generate the following files:
```sh
.res
├── grid.png
├── laplacian
│   ├── values.bin
│   ├── x_vec.bin
│   └── y_vec.bin
├── partial_x
│   ├── values.bin
│   ├── x_vec.bin
│   └── y_vec.bin
├── partial_y
│   ├── values.bin
│   ├── x_vec.bin
│   └── y_vec.bin
├── ufun
│   ├── values.bin
│   ├── x_vec.bin
│   └── y_vec.bin

```
To visualize the results go up one folder (to directory where ```visualize.py```
is located) and run the following command.

```sh
./visualuze.py specs/analysis-design.yaml
```
This will generate a set of images in your ```res``` folder (all figures used in
the report). In order to compile the report go to the ```report``` folder and
run your favorite latex compilator, e.g:
```sh
pdflatex main.tex
```

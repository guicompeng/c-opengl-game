# c-opengl-game
Game made entirely in C with OpenGL. It has only been tested on debian.
## Demo
### Video: https://www.youtube.com/watch?v=Lt8l_hjmREM

### Prints
<img src="img/demo1.png" alt="drawing" width="300"/> <img src="img/demo.png" alt="drawing" width="300"/>
<img src="img/demo3.png" alt="drawing" width="300"/> <img src="img/demo4.png" alt="drawing" width="300"/>

## Run
Clone this project, then enter that folder and run:
```sh
./game
```

# Edit and compile the game:
## Dependencies
```sh
sudo apt-get install freeglut3-dev 
sudo apt-get install libglew-dev 
sudo apt-get install libsoil-dev 
```
gcc is also needed
## Run
```sh
gcc main.c -lGL -lglut -lGLEW -lSOIL
./a.out
```

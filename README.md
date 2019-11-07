# Yet yet yet another Robot Ricochet solver :-)

## How it works ?

This really poor design uses PNG images as map input. You can open `map.xcf` with GIMP and move
the layers, paint the walls etc. And export it to `map.png`.

Then, use the following command to generate `map.bin`:

    php parse.php > map.bin

After, just compile:

    make

And run:

    ./solver

No argument so far, to change the robot you'll need to edit `main.cpp`
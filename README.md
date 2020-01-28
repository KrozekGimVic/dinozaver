# Dinozaver
Clone of the Chrome's dinosaur jumping game

# Installation and building
Make sure you have SFML installed, as well as necessary build tools.
```
sudo apt-get install libsfml-dev cmake
```

Clone the repository.
```
git clone git@github.com:KrozekGimVic/dinozaver.git
```
Go into the cloned folder and build the application.
```
cd dinozaver
mkdir -p build
cd build
cmake ..
make
```

After building, run
```
./dinozaver
```
to play.

To recompile, simply go back to the `build/` folder and run `make`.

Jure Slak

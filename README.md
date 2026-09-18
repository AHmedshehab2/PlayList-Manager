<<<<<<< HEAD
# PlayList-Manager
Built a MusicTrack class managing a manually resized dynamic array of songs, implementing a copy constructor, destructor, and doubling-capacity resize strateg
=======
# Music Playlist Manager

A simple C++ music playlist manager with two interfaces:

- The original console application in `main.cpp`.

The project keeps playlist behavior in `MusicTrack` and uses the global `Player` collection as the shared playlist store.

## Features

- Create, copy, and delete playlists.
- Add songs with a title and duration.
- Remove the last song from a playlist.
- Display songs in a table.
- Play a song by its index.
- Compare two playlists by song count.
- Display common songs between two playlists.
- Display songs unique to one playlist.
- Find the longest song across all playlists.
- Show the current total playlist count.


- C++17 compiler
- CMake 3.16 or newer
- Qt 6 with the Widgets component
- A compiler kit compatible with the installed Qt version

On Windows, Qt 6 with MinGW and CMake can be installed using the Qt Online Installer and the CMake installer or `winget`.

## Build the Qt GUI

From the project root, configure the project with the path to the Qt installation:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.8.0/mingw_64"
cmake --build build
```

Run the application:

```powershell
.\build\MusicPlaylistManager.exe
```

Change `C:/Qt/6.8.0/mingw_64` to the actual Qt installation path on your computer.

## Build the Console Application

The console application is independent of the Qt GUI:

```powershell
g++ main.cpp musictrack.cpp -std=c++17 -o console_app.exe
.\console_app.exe
```



## Learning Topics

This project demonstrates:

- C++ classes, constructors, destructors, copying, and operator overloading.
- Raw-pointer ownership for dynamically stored playlists.
- Qt widgets, layouts, dialogs, signals, and slots.
- CMake automoc support for Qt classes using `Q_OBJECT`.
- Separation between business logic and a graphical presentation layer.
>>>>>>> 53354f6 (Initial commit)

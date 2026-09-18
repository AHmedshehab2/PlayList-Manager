# Music Playlist Manager — Qt GUI Plan

> Goal: Add a SIMPLE Qt Widgets GUI as a presentation layer on top of existing `MusicTrack` / `Song` logic. No rewrite of business logic, no new features (DB, login, networking, playback, animations).

## 1. Existing code analysis (from `musictrack.h`, `musictrack.cpp`, `main.cpp`)

### 1.1 `struct Song`
```cpp
struct Song { string title; double duration; };
```

### 1.2 `class MusicTrack`
- Storage: `Song* playlist; int playlist_size; int capacity;`
- Static: `static int total_playlists; static Song longestSongInAllPlaylists;`
- Ctors (all `total_playlists++`):
  - `MusicTrack()` → `playlist=nullptr, capacity=0`
  - `MusicTrack(int size)` → `new Song[size]`
  - `MusicTrack(const MusicTrack& other)` → deep copy
- Dtor: `delete[] playlist` only, does NOT decrement count.
- `void resize()` → x2 capacity (0→1), copy over.
- `void addNewSong(Song, int index)` → `if(index>=capacity) resize()`, update longest static, `playlist[index]=s`.
- `Song get_song(int)` / `int get_Playlist_size()`
- Operators to preserve:
  - `Song& operator[](int)` — play by index
  - `MusicTrack operator+(other)` — common titles (deduped)
  - `MusicTrack operator-(other)` — unique to `this` (deduped)
  - `void operator--(int)` — remove last song (shrinks size, reallocs)
  - `friend ostream& operator<<(ostream&, MusicTrack&)` — prints all songs
  - `friend bool operator>=(m1,m2)` — compares `playlist_size`
- `static longest_song_in_all_playlists(vector<MusicTrack*> Player)` — loops all, `cout` title+duration, returns void.
- `static get_totalPlayList_created()`, `static decrementTotal()`

### 1.3 Global store
```cpp
extern vector<MusicTrack*> Player;
```
`main.cpp` console pattern:
- Add: `new MusicTrack(5); Player.push_back()`
- Remove: `delete Player[n]; Player.erase(begin+n); decrementTotal()`
- Copy: `new MusicTrack(*Player[n]); Player.push_back()`
- Compare/Common/Unique/Longest/Play-by-index/Print-all map 1:1 to operators above.

### 1.4 GUI pitfalls (do not fix by rewriting class)
1. `operator+` / `-` create `MusicTrack res` via default ctor (+1 to counter) + return-by-value copy (+1). They are never added to `Player` and never decremented → counter drifts. GUI fix: call `decrementTotal()` after displaying temp result, with comment.
2. `longest_song...` prints to `cout`, returns void → GUI will replicate same loop with `get_song()`/`get_Playlist_size()` to get a value for `QMessageBox`, leaving original function untouched.
3. `operator<<` → GUI iterates instead of using `cout`.
4. No playlist names → GUI shows `Playlist 1 (3 songs)` generated from index.
5. Include case: `#include "MusicTrack.h"` vs file `musictrack.h` — OK on Windows, fails on Linux. Only fix include case if needed, no logic change.
6. No bounds checking in `get_song`/`operator[]` → GUI checks `0<=i<size` before calling.

## 2. How GUI connects (no rewrite principle)

- Reuse global `Player` as single source of truth.
- `QListWidget` left = index into `Player`. `QTableWidget` right = loop `get_song(i)`.
- Each button slot = 2–5 lines calling existing API directly.
- No new manager/database class. `MainWindow` + 2 small dialogs only.
- UI code lives in `gui/`, business logic stays in `musictrack.*`.

## 3. Proposed file structure

```text
project/
  musictrack.h          # UNCHANGED
  musictrack.cpp        # UNCHANGED
  main.cpp              # UNCHANGED (console app, excluded from GUI build)
  CMakeLists.txt        # NEW
  gui/
    main_qt.cpp         # NEW (~15 lines: QApplication + MainWindow)
    mainwindow.h        # NEW
    mainwindow.cpp      # NEW
    addsongdialog.h     # NEW (QDialog: title QLineEdit + duration QDoubleSpinBox)
    addsongdialog.cpp   # NEW
    selecttwodialog.h   # NEW (QDialog: 2x QComboBox)
    selecttwodialog.cpp # NEW
```

### 3.1 `CMakeLists.txt` sketch
```cmake
cmake_minimum_required(VERSION 3.16)
project(MusicPlaylistManager LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable(MusicPlaylistManager
  musictrack.cpp
  gui/main_qt.cpp
  gui/mainwindow.cpp
  gui/addsongdialog.cpp
  gui/selecttwodialog.cpp
)
target_include_directories(MusicPlaylistManager PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(MusicPlaylistManager PRIVATE Qt6::Widgets)
```

### 3.2 Main window layout (code-only, no `.ui`, layouts only)
```text
QMainWindow "Music Playlist Manager"
└── central QWidget (QHBoxLayout)
    ├── left QVBoxLayout (stretch 1)
    │   ├── QLabel "Playlists"
    │   ├── QListWidget playlistList
    │   ├── QPushButton Add Playlist / Copy Playlist / Delete Playlist
    │   └── QLabel totalCount ("Total playlists: N")
    └── right QVBoxLayout (stretch 3)
        ├── QTableWidget (2 cols: Title | Duration, edit-disabled, row-select)
        ├── QLabel emptyState ("This playlist has no songs." hidden by default)
        └── QGridLayout buttons:
            [Add Song] [Remove Last Song] [Play by Index]
            [Compare] [Common Songs] [Unique Songs] [Longest Song]
```
- Empty playlists: `QListWidget` shows `"No playlists yet. Create a playlist to get started."` as disabled item.
- Theme: minimal dark/neutral QSS, reasonable spacing via `setSpacing/setContentsMargins`.

## 4. Implementation steps

1. Add `CMakeLists.txt` above.
2. Add `gui/main_qt.cpp`: `QApplication a; MainWindow w; w.show(); return a.exec();`
3. Add `MainWindow` (header declares `QListWidget*`, `QTableWidget*`, slots; cpp builds layouts, connects signals):
   - `refreshPlaylists()` — rebuild list from `Player`, keep selection, update count label.
   - `refreshSongs()` — clear table, loop `Player[cur]->get_song(i)`, show/hide empty label.
4. Add `AddSongDialog`: `QLineEdit title`, `QDoubleSpinBox duration (0.1–600, suffix " min")`, `QDialogButtonBox Ok|Cancel`, `getTitle()/getDuration()` getters.
5. Add `SelectTwoDialog`: two `QComboBox` filled with `Playlist i (N songs)`, `getFirst()/getSecond()` getters, reject if `<2` playlists.
6. Wire slots (see §6 table). Use `connect(btn, &QPushButton::clicked, this, &MainWindow::onX)`, `connect(list, &QListWidget::currentRowChanged, this, &MainWindow::refreshSongs)`.
7. Memory: `delete Player[i]` on remove + `qDeleteAll(Player)` in `~MainWindow` or `closeEvent`. Never `delete` temp `+`/`-` results (they are values, not pointers).
8. Verify: build, manually test all 11 GUI actions, check counter stays correct after Common/Unique.

## 5. Qt concepts to learn (to be explained with code after build)

1. `QApplication` + event loop (`exec()`)
2. `QMainWindow` vs `QWidget` vs `QDialog`
3. Qt Widgets used: `QListWidget`, `QTableWidget`, `QPushButton`, `QLabel`, `QLineEdit`, `QDoubleSpinBox`, `QComboBox`, `QMessageBox`, `QDialogButtonBox`
4. Layouts: `QHBoxLayout`, `QVBoxLayout`, `QGridLayout`, stretch factors, no absolute positions
5. Signals & slots: `connect()`, `clicked`, `currentRowChanged`, `accept()/reject()`, `exec()` modal
6. `QTableWidget` rows: `setRowCount`, `setItem`, `QTableWidgetItem`, disable editing
7. Beginner memory: parent takes ownership (`new QPushButton(parent)`), raw `MusicTrack*` still manual `delete`
8. `AUTOMOC` + `CMake Qt6::Widgets`
9. `QSS` for simple dark theme
10. Separation: UI calls business logic, never duplicates it

## 6. GUI action → existing code map

| # | GUI | Existing call | Refresh / output |
|---|---|---|---|
| 1 | Add Playlist | `new MusicTrack(5); Player.push_back()` | `refreshPlaylists()` |
| 2 | Delete Playlist | confirm `QMessageBox::question` → `delete Player[r]; Player.erase(...); decrementTotal()` | `refreshPlaylists()+refreshSongs()` |
| 3 | Add Song | dialog → `Song s{title,dur}; Player[cur]->addNewSong(s, Player[cur]->get_Playlist_size())` | `refreshSongs()+refreshPlaylists()` (count change) |
| 4 | Remove Last Song | `(*Player[cur])--` | `refreshSongs()+refreshPlaylists()` |
| 5 | Copy Playlist | `new MusicTrack(*Player[cur]); Player.push_back()` | `refreshPlaylists()` |
| 6 | Compare | dialog(a,b) → `*Player[a] >= *Player[b]` → `QMessageBox::information` | no list change |
| 7 | Common | dialog → `MusicTrack r = *Player[a] + *Player[b];` loop `r.get_song(i)` → message/table; `decrementTotal(); decrementTotal();`* | no list change |
| 8 | Unique | dialog → `MusicTrack r = *Player[a] - *Player[b];` same display + count fix | no list change |
| 9 | Longest | GUI loop (same algo as static fn) with getters → `QMessageBox` | no list change |
| 10 | Select playlist | `currentRowChanged` → display songs | `refreshSongs()` |
| 11 | Play by index | selected row `r` → `Song s = (*Player[cur])[r];` → `QMessageBox` | — |
| — | Total label | `MusicTrack::get_totalPlayList_created()` | status label |
| — | Empty states | `Player.empty()` → placeholder; `get_Playlist_size()==0` → label | — |

\* Temp fix keeps counter honest without editing your class. Commented clearly for learning.

## 7. Build & run (Qt NOT installed — per answers)

1. Install Qt 6 + MinGW via Qt Online Installer (check Widgets, Qt Creator). Install CMake: `winget install Kitware.CMake` + Ninja.
2. Configure + build (PowerShell, project root):
```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.8.0/mingw_64"
cmake --build build
.\build\MusicPlaylistManager.exe
```
3. Console version still works: `g++ main.cpp musictrack.cpp -o console_app`.
4. Qt Creator alternative: Open `CMakeLists.txt` → Configure → Run.
5. Troubleshoot: `CMAKE_PREFIX_PATH` wrong → `find_package Qt6` fails; MinGW mismatch → use kit compiler; `MusicTrack.h` case → rename include to `musictrack.h`.

## 8. Non-goals / constraints

- No DB, login, networking, playback, animations.
- No `.ui` Designer file (code-only per choice).
- No `MusicTrack`/`Song` logic rewrite.
- No over-abstraction: no extra store/service classes.

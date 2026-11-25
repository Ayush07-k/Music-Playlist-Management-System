# Music Playlist Management System (C)

## 1. Project Overview

The **Music Playlist Management System** is a menu-driven C project that manages songs using a **Doubly Linked List**. Each node stores a song's title, artist, duration, and two links:

- `prev` points to the previous song.
- `next` points to the next song.

Because both links are maintained, the playlist can be traversed in forward and backward directions. Songs can also be inserted, deleted, and updated dynamically.

> Note: The playback options simulate selecting/navigating songs in a playlist. This console project does not play actual audio files.

## 2. DSA Concepts Used

| Concept | Where it is used |
|---|---|
| Doubly Linked List | Main data structure for storing songs |
| Structure (`struct`) | `Song` and `Playlist` structures |
| Pointers | `head`, `tail`, `current`, `prev`, and `next` |
| Dynamic Memory Allocation | `malloc()` creates nodes |
| Dynamic Memory Deallocation | `free()` deletes nodes and clears the list |
| Insertion | Add at beginning, end, or a selected position |
| Deletion | Delete a song at a selected position |
| Updating | Change title, artist, and duration |
| Traversal | Display from beginning to end and end to beginning |
| Searching | Search song title or artist |
| Navigation | Move to next/previous song; start/end selection |
| Aggregation | Calculate total playlist duration |
| File Handling | Save to and load from `playlist.dat` |
| Functions | Program is divided into reusable functions |

## 3. Features / Modules

### Module 1: Playlist Creation
- Add a song at the beginning.
- Add a song at the end.
- Insert a song at any valid position.
- Store title, artist, and duration in seconds.

### Module 2: Song Deletion and Update
- Delete a song using its position.
- Update title, artist, and duration.
- Clear the complete playlist.

### Module 3: Playback Navigation
- Select the first song or last song.
- Move to the next song using the `next` pointer.
- Move to the previous song using the `prev` pointer.
- Display the currently selected song.

### Module 4: Search and Display
- Search by song title or artist.
- Display songs in forward order.
- Display songs in backward order.
- Calculate total playlist duration.

### Additional Feature: File Handling
- The playlist is loaded from `playlist.dat` when the program starts.
- Changes are saved automatically after add, delete, update, and clear operations.
- Use **Save playlist now** to save manually.
- The file is created in the program's current working directory.

## 4. Project Files

```text
Music-Playlist-Management-System/
│
├── music_playlist.c    # Complete C source code
├── README.md           # Project documentation
└── playlist.dat        # Created automatically when the playlist is saved
```

`playlist.dat` is generated at runtime; it does not need to be created manually.

## 5. Requirements

- A C compiler such as GCC (MinGW GCC on Windows).
- Visual Studio Code or another C IDE/editor.
- A terminal/command prompt.

## 6. How to Compile and Run

### Windows (GCC / MinGW)

Open the terminal in the folder containing `music_playlist.c`, then run:

```bash
gcc music_playlist.c -o music_playlist.exe
```

Run the program:

```bash
.\music_playlist.exe
```

### Linux / macOS

```bash
gcc music_playlist.c -o music_playlist
./music_playlist
```

If your compiler supports it, you can also enable warnings:

```bash
gcc -Wall -Wextra -std=c11 music_playlist.c -o music_playlist
```

## 7. How to Use

1. Run the program.
2. Choose an option from the menu by entering its number.
3. Add songs by entering the title, artist, and duration in seconds.
4. Use the display options to view the playlist.
5. Use next/previous to navigate through songs.
6. Use search to find a song by title or artist.
7. Choose `0` to save and exit.

### Example Song Details

```text
Enter song title: Perfect
Enter artist name: Ed Sheeran
Enter duration in seconds: 263
```

The duration is entered in seconds. For example, `263` seconds is displayed as `4 min 23 sec`.

## 8. Important Functions

| Function | Purpose |
|---|---|
| `initializePlaylist()` | Initializes the list pointers and count |
| `createSong()` | Allocates and initializes a new node |
| `insertSongAtPosition()` | Inserts a node at a requested position |
| `getSongAtPosition()` | Finds a node by its 1-based position |
| `deleteSong()` | Removes a node and reconnects its neighbours |
| `updateSong()` | Updates the details of a selected song |
| `clearPlaylist()` | Frees every node and resets the playlist |
| `displayForward()` | Traverses from `head` using `next` |
| `displayBackward()` | Traverses from `tail` using `prev` |
| `searchSongs()` | Searches title and artist text |
| `playNext()` / `playPrevious()` | Moves the current pointer |
| `savePlaylist()` / `loadPlaylist()` | Writes and reads song data |

## 9. Doubly Linked List Representation
```text
HEAD                                                  TAIL
  |                                                     |
  v                                                     v
NULL <- [Song 1] <-> [Song 2] <-> [Song 3] -> NULL
          prev/next     prev/next     prev/next
```

Each node contains song information and two pointers. The `head` points to the first node, while `tail` points to the last node. The `current` pointer identifies the currently selected song.

## 10. Conclusion
This project demonstrates how a Doubly Linked List can be used in a practical playlist-management application. It covers insertion, deletion, updating, searching, traversal in both directions, pointer-based navigation, dynamic memory management, and file handling. It is a console-based educational project designed to demonstrate DSA concepts in C.
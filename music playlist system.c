/*
 * Music Playlist Management System
 * Main DSA: Doubly Linked List
 
 * Features:
 * - Insert songs at beginning, end, or any valid position
 * - Delete a song by position
 * - Update song title, artist, and duration
 * - Clear the complete playlist
 * - Move to next/previous song using next/prev pointers
 * - Play from the beginning or end
 * - Search by title or artist
 * - Display playlist forward and backward
 * - Calculate total playlist duration
 * - Save/load playlist using file handling
 *
 * Compile: gcc music_playlist.c -o music_playlist
 * Run:     ./music_playlist      (Linux/macOS)
 *          music_playlist.exe    (Windows)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE_SIZE 100
#define ARTIST_SIZE 100
#define DATA_FILE "playlist.dat"

typedef struct Song {
    char title[TITLE_SIZE];
    char artist[ARTIST_SIZE];
    int duration;                 /* Duration in seconds */
    struct Song *prev;
    struct Song *next;
} Song;

typedef struct {
    Song *head;
    Song *tail;
    Song *current;
    int count;
} Playlist;

/* ---------- Input helpers ---------- */

void readLine(const char *prompt, char *buffer, int size) {
    int ch;
    int hasNewline;
    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    hasNewline = (strchr(buffer, '\n') != NULL);
    buffer[strcspn(buffer, "\n")] = '\0';

    /* Discard remaining characters if the input was longer than the buffer. */
    if (!hasNewline) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard */
        }
    }
}

int readInt(const char *prompt) {
    char input[64];
    char *end;
    long value;

    while (1) {
        readLine(prompt, input, sizeof(input));
        value = strtol(input, &end, 10);

        if (input[0] != '\0' && *end == '\0' &&
            value >= -2147483647L - 1 && value <= 2147483647L) {
            return (int)value;
        }

        printf("Invalid input. Please enter a whole number.\n");
    }
}

void formatTime(int seconds, char *output, size_t size) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int remainingSeconds = seconds % 60;

    if (hours > 0) {
        snprintf(output, size, "%d hr %d min %d sec",
                 hours, minutes, remainingSeconds);
    } else {
        snprintf(output, size, "%d min %d sec", minutes, remainingSeconds);
    }
}

/* ---------- Doubly linked list helpers ---------- */

void initializePlaylist(Playlist *playlist) {
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->current = NULL;
    playlist->count = 0;
}

Song *createSong(const char *title, const char *artist, int duration) {
    Song *newSong = (Song *)malloc(sizeof(Song));

    if (newSong == NULL) {
        printf("Memory allocation failed. Song was not added.\n");
        return NULL;
    }

    snprintf(newSong->title, TITLE_SIZE, "%s", title);
    snprintf(newSong->artist, ARTIST_SIZE, "%s", artist);
    newSong->duration = duration;
    newSong->prev = NULL;
    newSong->next = NULL;

    return newSong;
}

void getSongDetails(char *title, char *artist, int *duration) {
    do {
        readLine("Enter song title: ", title, TITLE_SIZE);
        if (title[0] == '\0') {
            printf("Title cannot be empty.\n");
        }
    } while (title[0] == '\0');

    do {
        readLine("Enter artist name: ", artist, ARTIST_SIZE);
        if (artist[0] == '\0') {
            printf("Artist name cannot be empty.\n");
        }
    } while (artist[0] == '\0');

    do {
        *duration = readInt("Enter duration in seconds: ");
        if (*duration <= 0) {
            printf("Duration must be greater than zero.\n");
        }
    } while (*duration <= 0);
}

Song *getSongAtPosition(const Playlist *playlist, int position) {
    Song *temp;
    int i;

    if (position < 1 || position > playlist->count) {
        return NULL;
    }

    /* Traverse from the nearer end to make use of both links. */
    if (position <= playlist->count / 2 + 1) {
        temp = playlist->head;
        for (i = 1; i < position; i++) {
            temp = temp->next;
        }
    } else {
        temp = playlist->tail;
        for (i = playlist->count; i > position; i--) {
            temp = temp->prev;
        }
    }

    return temp;
}

int insertSongAtPosition(Playlist *playlist, const char *title,
                         const char *artist, int duration, int position) {
    Song *newSong;
    Song *nextSong;

    if (position < 1 || position > playlist->count + 1) {
        printf("Invalid position. Valid positions are 1 to %d.\n",
               playlist->count + 1);
        return 0;
    }

    newSong = createSong(title, artist, duration);
    if (newSong == NULL) {
        return 0;
    }

    if (playlist->head == NULL) {
        playlist->head = playlist->tail = newSong;
    } else if (position == 1) {
        newSong->next = playlist->head;
        playlist->head->prev = newSong;
        playlist->head = newSong;
    } else if (position == playlist->count + 1) {
        newSong->prev = playlist->tail;
        playlist->tail->next = newSong;
        playlist->tail = newSong;
    } else {
        nextSong = getSongAtPosition(playlist, position);
        newSong->next = nextSong;
        newSong->prev = nextSong->prev;
        nextSong->prev->next = newSong;
        nextSong->prev = newSong;
    }

    playlist->count++;

    if (playlist->current == NULL) {
        playlist->current = playlist->head;
    }

    printf("Song added successfully at position %d.\n", position);
    return 1;
}

void addSong(Playlist *playlist, int position) {
    char title[TITLE_SIZE];
    char artist[ARTIST_SIZE];
    int duration;

    getSongDetails(title, artist, &duration);

    if (insertSongAtPosition(playlist, title, artist, duration, position)) {
        /* Save after a successful change. */
    }
}

void deleteSong(Playlist *playlist, int position) {
    Song *target;
    Song *replacement;

    target = getSongAtPosition(playlist, position);
    if (target == NULL) {
        printf("Invalid position. Playlist has %d song(s).\n", playlist->count);
        return;
    }

    replacement = target->next ? target->next : target->prev;

    if (target->prev != NULL) {
        target->prev->next = target->next;
    } else {
        playlist->head = target->next;
    }

    if (target->next != NULL) {
        target->next->prev = target->prev;
    } else {
        playlist->tail = target->prev;
    }

    if (playlist->current == target) {
        playlist->current = replacement;
    }

    printf("Deleted: %s - %s\n", target->title, target->artist);
    free(target);
    playlist->count--;

    if (playlist->count == 0) {
        playlist->head = playlist->tail = playlist->current = NULL;
    }
}

void updateSong(Playlist *playlist, int position) {
    Song *song = getSongAtPosition(playlist, position);
    char title[TITLE_SIZE];
    char artist[ARTIST_SIZE];
    int duration;

    if (song == NULL) {
        printf("Invalid position. Playlist has %d song(s).\n", playlist->count);
        return;
    }

    printf("Updating song at position %d.\n", position);
    getSongDetails(title, artist, &duration);

    snprintf(song->title, TITLE_SIZE, "%s", title);
    snprintf(song->artist, ARTIST_SIZE, "%s", artist);
    song->duration = duration;

    printf("Song details updated successfully.\n");
}

void clearPlaylist(Playlist *playlist) {
    Song *temp = playlist->head;
    Song *nextSong;

    while (temp != NULL) {
        nextSong = temp->next;
        free(temp);
        temp = nextSong;
    }

    initializePlaylist(playlist);
    printf("Playlist cleared successfully.\n");
}

/* ---------- Display, search and playback ---------- */

void printSong(const Song *song, int position) {
    char timeText[64];
    formatTime(song->duration, timeText, sizeof(timeText));
    printf("%-4d %-32s %-28s %s\n",
           position, song->title, song->artist, timeText);
}

void displayForward(const Playlist *playlist) {
    Song *temp = playlist->head;
    int position = 1;

    if (temp == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    printf("\nPLAYLIST (FORWARD)\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("%-4s %-32s %-28s %s\n", "No.", "Song Title", "Artist", "Duration");
    printf("--------------------------------------------------------------------------------\n");

    while (temp != NULL) {
        printSong(temp, position++);
        temp = temp->next;
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("Total songs: %d\n", playlist->count);
}

void displayBackward(const Playlist *playlist) {
    Song *temp = playlist->tail;
    int position = playlist->count;

    if (temp == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    printf("\nPLAYLIST (BACKWARD)\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("%-4s %-32s %-28s %s\n", "No.", "Song Title", "Artist", "Duration");
    printf("--------------------------------------------------------------------------------\n");

    while (temp != NULL) {
        printSong(temp, position--);
        temp = temp->prev;
    }
    printf("--------------------------------------------------------------------------------\n");
}

void searchSongs(const Playlist *playlist) {
    char query[TITLE_SIZE];
    Song *temp = playlist->head;
    int position = 1;
    int found = 0;

    if (playlist->head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    readLine("Enter title or artist to search: ", query, sizeof(query));
    if (query[0] == '\0') {
        printf("Search text cannot be empty.\n");
        return;
    }

    printf("\nSEARCH RESULTS\n");
    while (temp != NULL) {
        if (strstr(temp->title, query) != NULL ||
            strstr(temp->artist, query) != NULL) {
            printSong(temp, position);
            found = 1;
        }
        temp = temp->next;
        position++;
    }

    if (!found) {
        printf("No matching song found.\n");
    }
}

void showCurrentSong(const Playlist *playlist) {
    if (playlist->current == NULL) {
        printf("No song is currently selected. Playlist is empty.\n");
        return;
    }

    printf("Currently selected: %s - %s\n",
           playlist->current->title, playlist->current->artist);
}

void playNext(Playlist *playlist) {
    if (playlist->current == NULL) {
        printf("Playlist is empty. Add a song first.\n");
    } else if (playlist->current->next == NULL) {
        printf("You are already at the last song.\n");
        showCurrentSong(playlist);
    } else {
        playlist->current = playlist->current->next;
        showCurrentSong(playlist);
    }
}

void playPrevious(Playlist *playlist) {
    if (playlist->current == NULL) {
        printf("Playlist is empty. Add a song first.\n");
    } else if (playlist->current->prev == NULL) {
        printf("You are already at the first song.\n");
        showCurrentSong(playlist);
    } else {
        playlist->current = playlist->current->prev;
        showCurrentSong(playlist);
    }
}

void playFromStart(Playlist *playlist) {
    if (playlist->head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    playlist->current = playlist->head;
    printf("Playback selected from the beginning.\n");
    showCurrentSong(playlist);
}

void playFromEnd(Playlist *playlist) {
    if (playlist->tail == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    playlist->current = playlist->tail;
    printf("Playback selected from the end.\n");
    showCurrentSong(playlist);
}

void showTotalDuration(const Playlist *playlist) {
    const Song *temp = playlist->head;
    long totalSeconds = 0;
    char timeText[64];

    while (temp != NULL) {
        totalSeconds += temp->duration;
        temp = temp->next;
    }

    formatTime((int)totalSeconds, timeText, sizeof(timeText));
    printf("Total playlist duration: %s (%ld seconds)\n",
           timeText, totalSeconds);
}

/* ---------- File handling ---------- */

void savePlaylist(const Playlist *playlist) {
    FILE *file = fopen(DATA_FILE, "wb");
    Song *temp = playlist->head;
    int savedCount = 0;

    if (file == NULL) {
        printf("Warning: Could not open %s for saving.\n", DATA_FILE);
        return;
    }

    while (temp != NULL) {
        if (fwrite(temp->title, sizeof(temp->title), 1, file) != 1 ||
            fwrite(temp->artist, sizeof(temp->artist), 1, file) != 1 ||
            fwrite(&temp->duration, sizeof(temp->duration), 1, file) != 1) {
            printf("Warning: Error while saving playlist.\n");
            fclose(file);
            return;
        }
        savedCount++;
        temp = temp->next;
    }

    fclose(file);
    printf("Playlist saved (%d song(s)) to %s.\n", savedCount, DATA_FILE);
}

void loadPlaylist(Playlist *playlist) {
    FILE *file = fopen(DATA_FILE, "rb");
    char title[TITLE_SIZE];
    char artist[ARTIST_SIZE];
    int duration;

    if (file == NULL) {
        printf("No saved playlist found. Starting with an empty playlist.\n");
        return;
    }

    while (fread(title, sizeof(title), 1, file) == 1) {
        if (fread(artist, sizeof(artist), 1, file) != 1 ||
            fread(&duration, sizeof(duration), 1, file) != 1) {
            printf("Warning: Saved file appears incomplete. Loaded records are kept.\n");
            break;
        }

        title[TITLE_SIZE - 1] = '\0';
        artist[ARTIST_SIZE - 1] = '\0';

        if (duration > 0) {
            if (!insertSongAtPosition(playlist, title, artist, duration,
                                      playlist->count + 1)) {
                break;
            }
        }
    }

    fclose(file);
    if (playlist->count > 0) {
        printf("Loaded %d song(s) from %s.\n", playlist->count, DATA_FILE);
    }
}

/* ---------- Menu ---------- */

void displayMenu(void) {
    printf("\n========== MUSIC PLAYLIST MANAGEMENT SYSTEM ==========\n");
    printf(" 1. Add song at beginning\n");
    printf(" 2. Add song at end\n");
    printf(" 3. Add song at a specific position\n");
    printf(" 4. Delete song by position\n");
    printf(" 5. Update song details\n");
    printf(" 6. Clear complete playlist\n");
    printf(" 7. Play next song\n");
    printf(" 8. Play previous song\n");
    printf(" 9. Play from beginning\n");
    printf("10. Play from end\n");
    printf("11. Search by title or artist\n");
    printf("12. Display playlist forward\n");
    printf("13. Display playlist backward\n");
    printf("14. Show currently selected song\n");
    printf("15. Show total playlist duration\n");
    printf("16. Save playlist now\n");
    printf(" 0. Save and exit\n");
    printf("======================================================\n");
}

int main(void) {
    Playlist playlist;
    int choice;
    int position;

    initializePlaylist(&playlist);
    printf("Welcome to Music Playlist Management System!\n");
    loadPlaylist(&playlist);

    do {
        displayMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                addSong(&playlist, 1);
                savePlaylist(&playlist);
                break;

            case 2:
                addSong(&playlist, playlist.count + 1);
                savePlaylist(&playlist);
                break;

            case 3:
                position = readInt("Enter insertion position: ");
                addSong(&playlist, position);
                savePlaylist(&playlist);
                break;

            case 4:
                position = readInt("Enter song position to delete: ");
                deleteSong(&playlist, position);
                savePlaylist(&playlist);
                break;

            case 5:
                position = readInt("Enter song position to update: ");
                updateSong(&playlist, position);
                savePlaylist(&playlist);
                break;

            case 6:
                clearPlaylist(&playlist);
                savePlaylist(&playlist);
                break;

            case 7:
                playNext(&playlist);
                break;

            case 8:
                playPrevious(&playlist);
                break;

            case 9:
                playFromStart(&playlist);
                break;

            case 10:
                playFromEnd(&playlist);
                break;

            case 11:
                searchSongs(&playlist);
                break;

            case 12:
                displayForward(&playlist);
                break;

            case 13:
                displayBackward(&playlist);
                break;

            case 14:
                showCurrentSong(&playlist);
                break;

            case 15:
                showTotalDuration(&playlist);
                break;

            case 16:
                savePlaylist(&playlist);
                break;

            case 0:
                savePlaylist(&playlist);
                printf("Thank you for using the Music Playlist Management System!\n");
                break;

            default:
                printf("Invalid choice. Please select an option from the menu.\n");
        }
    } while (choice != 0);

    /* Free all dynamically allocated nodes before program termination. */
    clearPlaylist(&playlist);
    return 0;
}
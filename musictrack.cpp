#include "musictrack.h"

vector<MusicTrack*> Player;
int MusicTrack::total_playlists = 0;
Song MusicTrack::longestSongInAllPlaylists = { "", 0 };

MusicTrack::MusicTrack() :playlist_size(0), capacity(0), playlist(nullptr) {
    total_playlists++;
}

MusicTrack::MusicTrack(int size) :playlist_size(0), capacity(size) {
    playlist = new Song[capacity];
    total_playlists++;
}

MusicTrack::MusicTrack(const MusicTrack& other) {
    playlist_size = other.playlist_size;
    capacity = other.capacity;
    if (other.playlist != nullptr && capacity > 0) {
        playlist = new Song[capacity];
        for (int i = 0; i < playlist_size; ++i) {
            playlist[i] = other.playlist[i];
        }
    }
    else {
        playlist = nullptr;
    }
    total_playlists++;
}

void MusicTrack::resize() {
    int old_capacity = capacity;
    if (capacity == 0)
        capacity = 1;
    else
        capacity *= 2;

    Song* new_playlist = new Song[capacity];
    for (int i = 0; i < playlist_size; ++i) {
        new_playlist[i] = playlist[i];
    }
    delete[] playlist;
    playlist = new_playlist;
}

void MusicTrack::addNewSong(Song newSong, int index) {
    if (index >= capacity) {
        resize();
    }
    if (newSong.duration > longestSongInAllPlaylists.duration)
        longestSongInAllPlaylists = newSong;
    playlist[index] = newSong;
    if (index >= playlist_size) {
        playlist_size = index + 1;
    }
}

Song MusicTrack::get_song(int index) {
    return playlist[index];
}

Song& MusicTrack::operator[](int index) {
    return this->playlist[index];
}

MusicTrack MusicTrack::operator+(const MusicTrack& other) {
    MusicTrack res;
    for (int i = 0; i < this->playlist_size; ++i) {
        for (int j = 0; j < other.playlist_size; ++j) {
            if (this->playlist[i].title == other.playlist[j].title) {
                bool exist = false;
                for (int z = 0; z < res.playlist_size; z++) {
                    if (res.playlist[z].title == this->playlist[i].title)
                        exist = true;
                }
                if (!exist) {
                    res.addNewSong(this->playlist[i], res.playlist_size);
                }
                break;
            }
        }
    }
    return res;
}

MusicTrack MusicTrack::operator-(const MusicTrack& other) {
    MusicTrack res;
    for (int i = 0; i < this->playlist_size; ++i) {
        bool found_in_other = false;
        for (int j = 0; j < other.playlist_size; ++j) {
            if (this->playlist[i].title == other.playlist[j].title) {
                found_in_other = true;
                break;
            }
        }
        if (!found_in_other) {
            bool exist = false;
            for (int z = 0; z < res.playlist_size; z++) {
                if (res.playlist[z].title == this->playlist[i].title)
                    exist = true;
            }
            if (!exist) {
                res.addNewSong(this->playlist[i], res.playlist_size);
            }
        }
    }
    return res;
}

void MusicTrack::operator--(int) {
    if (playlist_size == 0)
        return;
    Song* new_playlist;
    playlist_size--;
    new_playlist = new Song[capacity];
    for (int i = 0; i < playlist_size; ++i) {
        new_playlist[i] = playlist[i];
    }
    delete[] playlist;
    playlist = new_playlist;
}

ostream& operator<<(ostream& os, MusicTrack& M1) {
    for (int i = 0; i < M1.playlist_size; i++) {
        os << M1.playlist[i].title << " " << M1.playlist[i].duration << endl;
    }
    return os;
}

bool operator>=(const MusicTrack& m1, const MusicTrack& m2) {
    return m1.playlist_size >= m2.playlist_size;
}

int MusicTrack::get_totalPlayList_created() {
    return total_playlists;
}

int MusicTrack::get_Playlist_size() {
    return playlist_size;
}

void MusicTrack::longest_song_in_all_playlists(vector<MusicTrack*> Player) {
    if (MusicTrack::get_totalPlayList_created() > 0) {
        double duration = 0;
        string title = "";
        for (int y = 0; y < Player.size(); y++) {
            for (int x = 0; x < Player[y]->get_Playlist_size(); x++) {
                if (Player[y]->playlist[x].duration > duration) {
                    duration = Player[y]->playlist[x].duration;
                    title = Player[y]->playlist[x].title;
                }
            }
        }
        cout << title << " is the Longest song " << endl;
        cout << "Duration of " << title << " is " << duration << endl;
    }
    else
        cout << "You have 0 playlist" << endl;
}

void MusicTrack::decrementTotal() {
    total_playlists--;
}

MusicTrack::~MusicTrack() {
    delete[] playlist;
    playlist = nullptr;
}
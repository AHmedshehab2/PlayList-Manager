#pragma once
#include <iostream>
#include <vector>
using namespace std;

struct Song {
    string title;
    double duration;
};

class MusicTrack {
private:
    Song* playlist;
    int playlist_size;
    int capacity;
    static int total_playlists;
    static Song longestSongInAllPlaylists;

public:
    MusicTrack();
    MusicTrack(int size);
    MusicTrack(const MusicTrack& other);
    void resize();
    void addNewSong(Song newSong, int index);
    Song get_song(int index);
    Song& operator[](int index);
    MusicTrack operator+(const MusicTrack& other);
    MusicTrack operator-(const MusicTrack& other);
    void operator--(int);
    friend ostream& operator<<(ostream& os, MusicTrack& M1);
    friend bool operator>=(const MusicTrack& m1, const MusicTrack& m2);
    static int get_totalPlayList_created();
    int get_Playlist_size();
    static void longest_song_in_all_playlists(vector<MusicTrack*> Player);
    static void decrementTotal();
    ~MusicTrack();
};

extern vector<MusicTrack*> Player;
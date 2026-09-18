#include "musictrack.h"
using namespace std;

void display_all_playlist() {
    for (int x = 0; x < MusicTrack::get_totalPlayList_created(); x++) {
        cout << x + 1 << ". Playlist" << endl;
        for (int y = 0; y < Player[x]->get_Playlist_size(); y++) {
            cout << "\t\tTitle song " << y + 1 << ": " << Player[x]->get_song(y).title << endl;
            cout << "\t\tDuration song " << y + 1 << ": " << Player[x]->get_song(y).duration << endl;
        }
        cout << endl;
    }
}

void remove_playlist(int num) {
    if (MusicTrack::get_totalPlayList_created() > num && num >= 0) {
        delete Player[num];
        Player.erase(Player.begin() + num);
        MusicTrack::decrementTotal();
        cout << "\nNow the number of playlists is: " << MusicTrack::get_totalPlayList_created() << endl;
    }
    else {
        cout << "Playlist does not exist" << endl;
    }
}

int Menu_screen() {
    int choose = 20;
    while (choose > 13 || choose < 1) {
        system("cls");
        cout << endl;
        cout << "\t1. Insert a new playlist" << endl;
        cout << "\t2. Push new songs to a playlist" << endl;
        cout << "\t3. Remove a playlist" << endl;
        cout << "\t4. Copy a playlist" << endl;
        cout << "\t5. Display total playlists created" << endl;
        cout << "\t6. Show the longest song among all playlists" << endl;
        cout << "\t7. Compare two playlists" << endl;
        cout << "\t8. Play a song by index" << endl;
        cout << "\t9. Display common songs" << endl;
        cout << "\t10. Display unique songs" << endl;
        cout << "\t11. Remove last song" << endl;
        cout << "\t12. Print all songs" << endl;
        cout << "\t13. Exit" << endl << endl;
        cout << "\t\t\t Enter your choice: ";
        cin >> choose;
        if (choose > 13 || choose < 1) {
            cout << "\t\t Enter a number from 1 to 13" << endl;
            system("cls");
        }
    }
    return choose;
}

int main() {
    int choice;

    do {
        choice = Menu_screen();
        switch (choice) {
        case 1: {
            MusicTrack* newPlaylist = new MusicTrack(5);
            Player.push_back(newPlaylist);
            system("cls");
            break;
        }
        case 2: {
            if (Player.empty()) {
                system("cls");
                break;
            }
            int playlistNum;
            cin >> playlistNum;
            playlistNum--;
            if (playlistNum >= 0 && playlistNum < Player.size()) {
                int numSongs;
                cin >> numSongs;
                cin.ignore();
                for (int i = 0; i < numSongs; i++) {
                    Song s;
                    getline(cin, s.title);
                    cin >> s.duration;
                    cin.ignore();
                    Player[playlistNum]->addNewSong(s, Player[playlistNum]->get_Playlist_size());
                }
            }
            system("cls");
            break;
        }
        case 3: {
            int num;
            cin >> num;
            remove_playlist(num);
            system("cls");
            break;
        }
        case 4: {
            int num;
            cin >> num;
            if (num >= 0 && num < Player.size()) {
                MusicTrack* copy = new MusicTrack(*Player[num]);
                Player.push_back(copy);
            }
            system("cls");
            break;
        }
        case 5: {
            cout << MusicTrack::get_totalPlayList_created() << endl;
            system("cls");
            break;
        }
        case 6: {
            MusicTrack::longest_song_in_all_playlists(Player);
            system("cls");
            break;
        }
        case 7: {
            int p1, p2;
            cin >> p1 >> p2;
            if (p1 >= 0 && p1 < Player.size() && p2 >= 0 && p2 < Player.size()) {
                if (*Player[p1] >= *Player[p2]) {
                    cout << "Playlist " << p1 << " has more or equal songs" << endl;
                }
                else {
                    cout << "Playlist " << p2 << " has more songs" << endl;
                }
            }
            system("cls");
            break;
        }
        case 8: {
            int pNum, sNum;
            cin >> pNum >> sNum;
            if (pNum >= 0 && pNum < Player.size()) {
                if (sNum >= 0 && sNum < Player[pNum]->get_Playlist_size()) {
                    Song s = (*Player[pNum])[sNum];
                    cout << s.title << " " << s.duration << endl;
                }
            }
            system("cls");
            break;
        }
        case 9: {
            int p1, p2;
            cin >> p1 >> p2;
            if (p1 >= 0 && p1 < Player.size() && p2 >= 0 && p2 < Player.size()) {
                MusicTrack common = *Player[p1] + *Player[p2];
                cout << common;
            }
            system("cls");
            break;
        }
        case 10: {
            int p1, p2;
            cin >> p1 >> p2;
            if (p1 >= 0 && p1 < Player.size() && p2 >= 0 && p2 < Player.size()) {
                MusicTrack unique = *Player[p1] - *Player[p2];
                cout << unique;
            }
            system("cls");
            break;
        }
        case 11: {
            int pNum;
            cin >> pNum;
            if (pNum >= 0 && pNum < Player.size()) {
                (*Player[pNum])--;
            }
            system("pause");
            break;
        }
        case 12: {
            display_all_playlist();
            system("cls");
            break;
        }
        case 13:
            break;
        }
    } while (choice != 13);

    for (int i = 0; i < Player.size(); i++) {
        delete Player[i];
    }

    return 0;
}
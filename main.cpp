#include <chrono>
#include <iostream>
#include <thread>
#include "clui.h"

//colors!
#define RED "\x1B[31m"
#define YELLOW "\x1B[33m"
#define GREEN "\x1B[32m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define BLUE "\x1B[34m"
#define BLACK "\x1B[30m"
#define RESET "\x1B[0m"

//strings
#define GET_USERNAME "Enter your username: "
#define USER_ERROR "Error: This user already exists."
#define GAME_ERROR "Error: This game name already exists."
#define INVALID_CHOICE "The entered number is invalid. Try again."
#define NO_UNFINISHED "Error: No unfinished games exist for this user."
#define RESUME_GAME "Enter a number to resume that game: "
#define STATUS_SAVED "Game status saved."
#define ERROR_NOUSERS "Error: No users found."
#define ONE "1. "
#define TWO "2. "
#define THREE "3. "
#define FOUR "4. "
#define WELCOME "Welcome to Sudoku!"
#define PLAY_NEW "Play New Game"
#define PLAY_SAVED "Play From Saved Games"
#define SCOREBOARD "Scoreboard"
#define ARROW "    ->"
#define EXIT "Exit"
#define EASY "Easy"
#define MEDIUM "Medium"
#define HARD "Hard"
#define LEADERBOARD "Rank, Name, Corrects, Wrongs, Won, Lost, Total Play Time"
#define ERROR_1 "Error: Wrong number! Life decreased by one."
#define ERROR_2 "Error: Default values can't be changed."
#define YOU_WON "Congratulations! You won."
#define YOU_LOST "Oops! You lost."
#define GAME_NAME "Enter a name for your game: "
#define TIMES_UP "Time's up! Be faster next time."
#define RETURN_ANYKEY "Press any key to return"
#define PAUSED "Game is paused. Press ESC to resume."
#define ASCII_ART "\n\n"\
                  "███████╗██╗   ██╗██████╗  ██████╗ ██╗  ██╗██╗   ██╗\n"\
                  "██╔════╝██║   ██║██╔══██╗██╔═══██╗██║ ██╔╝██║   ██║\n"\
                  "███████╗██║   ██║██║  ██║██║   ██║█████╔╝ ██║   ██║\n"\
                  "╚════██║██║   ██║██║  ██║██║   ██║██╔═██╗ ██║   ██║\n"\
                  "███████║╚██████╔╝██████╔╝╚██████╔╝██║  ██╗╚██████╔╝\n"\
                  "╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ \n"                                                   


using namespace std;

enum OS_Type{
    Windows, Linux
} os_type;
struct Game{
    char name[100];
    int total_time;
    int time;
    int lives = 5;
    int table[9][9];
    bool is_finished = false;
    int corrects = 0;
    int wrongs = 0;
};
struct User{
    char name[100];
    Game *games = 0;
    int number_of_games = 0;
    int corrects = 0;
    int wrongs = 0;
    int total_play_time = 0;
    int won = 0;
    int lost = 0;
};

int sudoku_table[9][9] = {
    {8, 3, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 2, 3, 0, 0},
    {1, 0, 0, 0, 5, 0, 0, 0, 4},
    {9, 8, 0, 1, 0, 5, 0, 7, 2},
    {2, 5, 7, 9, 0, 0, 0, 3, 1},
    {6, 1, 3, 7, 2, 8, 0, 4, 0},
    {4, 2, 0, 5, 0, 1, 0, 0, 3},
    {0, 7, 8, 0, 0, 9, 0, 0, 5},
    {0, 6, 0, 4, 0, 0, 0, 0, 0}
};
User *users = 0;
int users_count = 0;

//prototypes
void sleep(int);
void type(const char*,int);
void clear();
void countdown(int);
void display_main_menu();
void ask_username();
OS_Type identify_OS();
void choose_difficulty(int);
void update_game_board(int,int,int,int);
void play_game(int);
bool check_table(int,int,int,int);
void end_game(int,int);
void create_new_game();
int choose_unfinished_game();
void play_unfinished_game();
void display_leader_board();

int main(){
    os_type = identify_OS();
    users = new User[100];
    while(true){
        ask_username();
        display_main_menu();
    }

    for(int i = 0; i < users_count; i++)
        delete[] users[i].games;
    delete[] users;
    return 0;
}

void sleep(int ms){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void type(const char *str, int time){
    for(int i = 0; str[i]; i++){
        cout << str[i]; 
        cout.flush();
        if(time == 2)
            sleep(30);
        else if(time == 1)
            sleep(5);
        else
            sleep(2);
	}
}

OS_Type identify_OS(){
    #if defined(_WIN32) || defined(_WIN64)
        return Windows;
    #endif
        return Linux;
}

void clear(){
    if(os_type == Windows)
        system("cls");
    else
        system("clear");
}

void display_main_menu(){
    while(true){
        clear();
        cout << GREEN ASCII_ART RESET << endl;
        type(CYAN ONE RESET PLAY_NEW, 2); cout << endl;
        type(CYAN TWO RESET PLAY_SAVED, 2); cout << endl;
        type(CYAN THREE RESET SCOREBOARD, 2); cout << endl;
        type(CYAN FOUR RESET EXIT, 2); cout << endl;
        type(MAGENTA ARROW RESET, 2);

        int c; cin >> c;
        switch(c){
            case 1:
                create_new_game();
                choose_difficulty(users[users_count-1].number_of_games - 1);
                play_game(users[users_count-1].number_of_games - 1);
                break;
            case 2:
                play_unfinished_game();
                break;
            case 3:
                display_leader_board();
                break;
            case 4:
                return;
                break;
            default:
                cout << "invalid choice" << endl;
                //todo
        }
    }
}

void create_new_game(){
    Game *temp = new Game[users[users_count-1].number_of_games + 1];
    for(int i = 0; i < users[users_count-1].number_of_games; i++){
        strcpy(temp[i].name, users[users_count-1].games[i].name);
        temp[i].total_time = users[users_count-1].games[i].total_time;
        temp[i].time = users[users_count-1].games[i].time;
        temp[i].lives = users[users_count-1].games[i].lives;
        for(int j = 0; j < 9; j++)
            for(int k = 0; k < 9; k++)
                temp[i].table[j][k] = users[users_count-1].games[i].table[j][k];
        temp[i].is_finished = users[users_count-1].games[i].is_finished;
        temp[i].corrects = users[users_count-1].games[i].corrects;
        temp[i].wrongs = users[users_count-1].games[i].wrongs;
    }
    for(int j = 0; j < 9; j++)
        for(int k = 0; k < 9; k++)
            temp[users[users_count-1].number_of_games].table[j][k] = sudoku_table[j][k];
    temp[users[users_count-1].number_of_games].name[0] = '\0';
    users[users_count-1].number_of_games++;
    delete[] users[users_count-1].games;
    users[users_count-1].games = temp;
}

void ask_username(){
    clear();
    char user[100];
    
    int is_valid = true;
    do{
        if(is_valid == false){
            clear();
            type(RED USER_ERROR RESET "\n", 2);
        }
        is_valid = true;
        type(GREEN GET_USERNAME RESET, 2);
        cin >> user;
        for(int i = 0; i < users_count; i++)
            if(strcmp(users[i].name, user) == 0){
                is_valid = false;
                break;
            }
    } while(is_valid == false);
    User *temp = new User[users_count + 1];
    for(int i = 0; i < users_count; i++){
        temp[i] = users[i];
    }
    strcpy(temp[users_count].name, user);
    users_count++;
    delete[] users;
    users = temp;
    /*for(int i = 0; user[i]; i++)
        usernames[users_count][i] = user[i];
    users_count++;*/
}

bool is_thread_running;
void countdown(int game_index){
    while (users[users_count-1].games[game_index].time > 0 && is_thread_running) {
        this_thread::sleep_for(chrono::seconds(1));
        users[users_count-1].games[game_index].time--;
    }
}

void choose_difficulty(int game_index){
    clear();
    int is_valid = true;

    do{
        if(is_valid == false){
            clear();
            type(RED INVALID_CHOICE RESET "\n", 2);
        }
        is_valid = true;
        type(CYAN ONE RESET EASY, 2); cout << endl;
        type(CYAN TWO RESET MEDIUM, 2); cout << endl;
        type(CYAN THREE RESET HARD, 2); cout << endl;
        type(MAGENTA ARROW RESET, 2);
        int c; cin >> c;
        switch(c){
            case 1:
                users[users_count-1].games[game_index].time = 300;
                users[users_count-1].games[game_index].total_time = 300;
                break;
            case 2:
                users[users_count-1].games[game_index].time = 180;
                users[users_count-1].games[game_index].total_time = 180;
                break;
            case 3:
                users[users_count-1].games[game_index].time = 120;
                users[users_count-1].games[game_index].total_time = 120;
                break;
            default:
                is_valid = false;
        }
    } while(is_valid == false);

}

void update_game_board(int row, int col, int game_index, int status_code){
    clear();
    cout << "Time: " << users[users_count-1].games[game_index].time << endl;
    cout << CYAN << "┏━━━━━━━┳━━━━━━━┳━━━━━━━┓" << RESET << endl;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 9; j++){
            if(j % 3 == 0) cout << CYAN << "┃ " << RESET;
            if(i == row && j == col){
                change_background_color(6);
                cout << BLACK << users[users_count-1].games[game_index].table[i][j] << RESET << " ";
            }
            else{
                if(sudoku_table[i][j] != 0) cout << GREEN;
                cout << users[users_count-1].games[game_index].table[i][j] << " " << RESET;
            }
        }
        cout << CYAN << "┃" << RESET << endl;
    }
    cout << CYAN << "┣━━━━━━━╋━━━━━━━╋━━━━━━━┫" << RESET << endl;
    for(int i = 3; i < 6; i++){
        for(int j = 0; j < 9; j++){
            if(j % 3 == 0) cout << CYAN << "┃ " << RESET;
            if(i == row && j == col){
                change_background_color(6);
                cout << BLACK << users[users_count-1].games[game_index].table[i][j] << RESET << " ";
            }
            else{
                if(sudoku_table[i][j] != 0) cout << GREEN;
                cout << users[users_count-1].games[game_index].table[i][j] << " " << RESET;
            }
        }
        cout << CYAN << "┃" << RESET << endl;
    }
    cout << CYAN << "┣━━━━━━━╋━━━━━━━╋━━━━━━━┫" << RESET << endl;
    for(int i = 6; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(j % 3 == 0) cout << CYAN << "┃ " << RESET;
            if(i == row && j == col){
                change_background_color(6);
                cout << BLACK << users[users_count-1].games[game_index].table[i][j] << RESET << " ";
            }
            else{
                if(sudoku_table[i][j] != 0) cout << GREEN;
                cout << users[users_count-1].games[game_index].table[i][j] << " " << RESET;
            }
        }
        cout << CYAN << "┃" << RESET << endl;
    }
    cout << CYAN << "┗━━━━━━━┻━━━━━━━┻━━━━━━━┛" << RESET << endl;
    if(status_code == 1)
        cout << RED ERROR_1 RESET << endl;
    else if(status_code == 2)
        cout << RED ERROR_2 RESET << endl;

    int cursor_row = 0, cursor_col = 0;
    if(0 <= row && row <3)
        cursor_row = 3+row;
    else if(3 <= row && row < 6)
        cursor_row = 4+row;
    else
        cursor_row = 5+row;
    
    if(0 <= col && col <3)
        cursor_col = 2 * col + 3;
    else if(3 <= col && col < 6)
        cursor_col = 2 * col + 5;
    else
        cursor_col = 2 * col + 7;
    cursor_to_pos(cursor_row, cursor_col);
    
}

void play_game(int game_index){
    int status_code = 0, correct = 0;
    init_clui();
    cout << "\033[?25l";
    int row = 0, col = 0;
    update_game_board(row,col,game_index,0);
    is_thread_running = true;
    thread timer(countdown, game_index);

    while(true){
        if(users[users_count-1].games[game_index].time == 0){
            users[users_count-1].lost++;
            users[users_count-1].games[game_index].corrects += correct;
            users[users_count-1].games[game_index].wrongs += (5 - users[users_count-1].games[game_index].lives);
            is_thread_running = false; timer.join();
            end_game(4,game_index);
            return;
        }
        if(is_keyboard_hit()){
            char c = getch();
            if(c == '\033'){
                getch();
                switch(getch()){
                    case 'A':
                        if(row > 0) row--;
                        break;
                    case 'B':
                        if(row < 8) row++;
                        break;
                    case 'C':
                        if(col < 8) col++;
                        break;
                    case 'D':
                        if(col > 0) col--;
                        break;
                }
            } else{
                if(c >= '0' && c <= '9'){
                    if(sudoku_table[row][col] == 0)
                        if(check_table(game_index, row, col, c - '0')){
                            users[users_count-1].games[game_index].table[row][col] = c - '0';
                            correct++;
                            status_code = 3; //won
                            for(int i = 0; i < 9; i++)
                                for(int j = 0; j < 9; j++)
                                    if(users[users_count-1].games[game_index].table[i][j] == 0){
                                        status_code = 0; //playing
                                        break;
                                    }
                            if(status_code == 3){
                                users[users_count-1].won++;
                                users[users_count-1].games[game_index].corrects += correct;
                                users[users_count-1].games[game_index].wrongs += (5 - users[users_count-1].games[game_index].lives);
                                is_thread_running = false; timer.join();
                                end_game(1, game_index);
                                return;
                            }
                        }
                        else{
                            status_code = 1;
                            users[users_count-1].games[game_index].lives--;
                            if(users[users_count-1].games[game_index].lives == 0){
                                users[users_count-1].lost++;
                                users[users_count-1].games[game_index].corrects += correct;
                                users[users_count-1].games[game_index].wrongs += (5 - users[users_count-1].games[game_index].lives);
                                is_thread_running = false; timer.join();
                                end_game(2, game_index);
                                return;
                            }
                        }
                    else
                        status_code = 2;
                }
                else if(c == 's'){
                    users[users_count-1].games[game_index].corrects += correct;
                    users[users_count-1].games[game_index].wrongs += (5 - users[users_count-1].games[game_index].lives);
                    is_thread_running = false; timer.join();
                    end_game(3, game_index);
                    return;
                } 
                else if(c == '`'){
                    clear();
                    is_thread_running = false; timer.join();
                    type(PAUSED, 2);
                    while(true){
                        char c = getch();
                        if(c == '\033'){
                            clear();
                            is_thread_running = true;
                            timer = thread(countdown, game_index);
                            break;
                        }
                    }
                }
                else if(c == 'q'){
                    users[users_count-1].lost++;
                    users[users_count-1].games[game_index].corrects += correct;
                    users[users_count-1].games[game_index].wrongs += (5 - users[users_count-1].games[game_index].lives);
                    is_thread_running = false; timer.join();
                    end_game(2, game_index);
                    return;
                }
            }
        }
        else
            sleep(40);
        update_game_board(row,col,game_index, status_code);
    }

}

bool check_table(int game_index, int row, int col, int num){
    for(int i = 0; i < 9; i++)
        if(users[users_count-1].games[game_index].table[i][col] == num)
            return false;
    for(int i = 0; i < 9; i++)
        if(users[users_count-1].games[game_index].table[row][i] == num)
            return false;
    int start_row = (row/3)*3, start_col = (col/3)*3;
    for (int i = start_row; i < start_row+3; i++)
        for (int j = start_col; j < start_col+3; j++)
            if (users[users_count - 1].games[game_index].table[i][j] == num)
                return false;

    return true;
        
}

void end_game(int code, int game_index){
    // 1-> won , 2-> lost, 3->paused, 4->time up
    clear_screen();
    fflush(stdin);
    cout << "\033[?25h";
    if(code == 1){
        type(GREEN YOU_WON RESET "\n", 2);
        users[users_count-1].games[game_index].is_finished = true;
    }
    else if(code == 2){
        type(RED YOU_LOST RESET "\n", 2);
        users[users_count-1].games[game_index].is_finished = true;
    }
    else if(code == 3)
        type(GREEN STATUS_SAVED RESET "\n", 2);
    else{
        type(RED TIMES_UP RESET "\n", 2);
        users[users_count-1].games[game_index].is_finished = true;
    }
    char name[100]; 
    bool is_valid = true;
    if(users[users_count-1].games[game_index].name[0] == '\0'){
        do{
            if(is_valid == false){
                clear();
                type(RED GAME_ERROR RESET "\n", 2);
            }
            type(GAME_NAME, 2);
            cin >> name;
            is_valid = true;
            for(int i = 0; i < users_count; i++)
                for(int j = 0; j < users[i].number_of_games; j++)
                    if(strcmp(users[i].games[j].name, name) == 0){
                        is_valid = false;
                        break;
                    }
        } while(is_valid == false);
        strcpy(users[users_count-1].games[game_index].name, name);
    }

    clear();
    cout << "Status: ";
    if(code == 1) cout << GREEN << "Won" << RESET << endl;
    else if(code == 2) cout << RED << "Lost" << RESET << endl;
    else if(code == 3) cout << GREEN << "Paused" << RESET << endl;
    else cout << RED << "Time's up" << RESET << endl;
    cout << "Score:" << users[users_count-1].games[game_index].corrects - users[users_count-1].games[game_index].wrongs << endl;
    cout << "Corrects: " << GREEN << users[users_count-1].games[game_index].corrects << RESET << endl;
    cout << "Wrongs: " << RED << users[users_count-1].games[game_index].wrongs << RESET << endl;
    cout << "Remaining Time: " << users[users_count-1].games[game_index].time << endl;
    
    cout << RETURN_ANYKEY << endl;
    fflush(stdin);
    getch();
}

int choose_unfinished_game(){
    int unfinished_games[users[users_count].number_of_games], index = 0; 
    for(int i = 0; i < users[users_count-1].number_of_games; i++)
        if(users[users_count-1].games[i].is_finished == false)
            unfinished_games[index++] = i;
    if(index == 0){
        type(RED NO_UNFINISHED RESET, 2);
        return -1;
    }

    bool first_time = true;
    char n = 0;
    do{
        clear();
        for(int i = 0; i < index; i++)
                cout << CYAN << i+1 << ". " << RESET << users[users_count-1].games[unfinished_games[i]].name << endl;

        if(first_time == false)
            type(RED INVALID_CHOICE RESET "\n", 2);
        
        type(RESUME_GAME, 2);
        cin >> n;
        if(n == 'm') return -1;
        first_time = false;
    } while(n < '1' || n >index+'0');
    return unfinished_games[index-1];
}

void play_unfinished_game(){
    int n = choose_unfinished_game();
    if(n == -1) return;
    else play_game(n);
}

void display_leader_board(){
    if(users_count == 0){
        type(RED ERROR_NOUSERS RESET, 2);
        return;
    }
    clear_screen();
    fflush(stdin);
    for(int i = 0; i < users_count; i++){
        users[i].corrects = 0;
        users[i].wrongs = 0;
        users[i].total_play_time = 0;
        for(int j = 0; j < users[i].number_of_games; j++){
            users[i].corrects += users[i].games[j].corrects;
            users[i].wrongs += users[i].games[j].wrongs;
            users[i].total_play_time += users[i].games[j].total_time - users[i].games[j].time;
            //cout << "name: " << users[i].games[j].name << " total time: " << users[i].games[j].total_time << " time: " << users[i].games[j].time << endl;
        }
    }
    int sorted[users_count];
    for(int i = 0; i < users_count; i++)
        sorted[i] = i;
    for(int i = 0; i < users_count-1; i++) {
        int max = i;
        for(int j = i + 1; j < users_count; j++)
            if((users[sorted[j]].corrects - users[sorted[j]].wrongs) > (users[sorted[max]].corrects - users[sorted[max]].wrongs)) 
                max = j;
        int temp = sorted[i];
        sorted[i] = sorted[max];
        sorted[max] = temp;
    }
    for(int i = 0; i < users_count-1; i++)
        for(int j = i + 1; j < users_count; j++)
            if((users[sorted[j]].corrects - users[sorted[j]].wrongs) == (users[sorted[i]].corrects - users[sorted[i]].wrongs))
                if(users[sorted[j]].total_play_time < users[sorted[i]].total_play_time){
                    int temp = sorted[i];
                    sorted[i] = sorted[j];
                    sorted[j] = temp;
                }

        
    
    cout << LEADERBOARD << endl;
    for(int i = 0; i < users_count; i++){
        cout << CYAN << i+1 << ". " << RESET 
            << users[sorted[i]].name << ", " 
            << GREEN << users[sorted[i]].corrects << RESET << ", "
            << RED << users[sorted[i]].wrongs << RESET << ", "
            << GREEN << users[sorted[i]].won << RESET << ", "
            << RED << users[sorted[i]].lost << RESET << ", "
            << users[sorted[i]].total_play_time
            << endl;
    }
    cout << endl << RETURN_ANYKEY << endl;
    getch();
}
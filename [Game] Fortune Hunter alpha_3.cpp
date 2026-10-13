#include <algorithm>
#include <cmath>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <direct.h>
#include <fstream>
#include <io.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>


#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
#define YELLOW SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
#define BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY);
#define GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
#define RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY);
#define AQUA SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
#define PINK SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);

/*
Upd Log:

pre 0
Repeat Original Function (Done)
Detect Keyboard, Adapt ASDW & UpDownLeftRight (Done)
Color selected row to GREEN, column to BLUE, chiasma to AQUA, with coords (Done)
Display dynamic coord (Done)
Timing Mode (Done) & MOD Mode
	timing: a start flag, an end flag, a container

(those versions below are informal, and is under-testing.
the next formal version is only for playing, so will not be submitted                                )

alpha 1
Start Screen

alpha 2
Menu Page

alpha 3
Rules Page

alpha 4
GUIDE mode + auto move

beta 1
settings:
	Customize map (size, sensor, ring)

beta 2
difficulty parameter
	Calculate Difficulty Ratio
*/

using namespace std;

// MAX width = 74  MAX height = 23
struct pos {
	int x, y;
} table[40005], crosshair;
struct map_node {
	int value;
	bool state;
	// state = 0: not viewed, state = 1: viewed
};
struct two_dimension {
	vector<map_node> sub;
};
struct time_container {
	int hrs, min, sec, msec;
};
vector<pos> treasure;
int MAP_WIDTH, MAP_HEIGHT, sensor_amount, treasure_left;
vector<two_dimension> treasure_map;

void reset() {
	for (int i = 0; i < 40005; i ++)
		table[i] = pos();
	treasure.clear();
	treasure_map.clear();
}

void locate(int x,int y) {
     COORD c;
     c.X = x - 1;
     c.Y = y - 1;
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

void typer(int x, int y, string a, int time) {
     for (int i = x, j = y, count = 0; count < a.length(); i ++, count ++) {
         Sleep(time);
         locate(i, j);
         cout << a[count];
         if (i > 80) {
            i = 1;
            j ++;
         }
     }
}

char inkey() {
     char x = '\0';
     if (kbhit()) {
        x = getch();
     }
     return x;
}

void cincode(int xline, int yline, char a[], int n, char p) {
     int top = -1;
     char x = '\0';
     for (;;) {
         for (;x == '\0';) x = inkey();
         if (x == 13) break;
         if (x == 8 && top < 0);
         else if (x == 8 && top >= 0) {
              top --;
              a[top] = '\0';
              locate(xline + top + 1, yline);
              cout << char(32);
         } else if (top < n && x != 8) {
               top ++;
               a[top] = x;
               locate(xline + top, yline);
               cout << p;
         } else if (top >= n) { 
               locate(1, yline + 1);
               cout << "The password must less than " << n << " letters/numbers.";
         }
         x = '\0';
     }
     a[top + 1] = '\0';
}

time_container read_time(int x) {
	time_container result;
	result.msec = x % 1000;
	result.sec = (x / 1000) % 60;
	result.min = (x / 60000) % 60;
	result.hrs = (x / 3600000) % 60;
	return result;
}

time_container sub(time_container a, time_container b) {
	time_container result;
	result.msec = a.msec - b.msec;
	result.sec = a.sec - b.sec;
	result.min = a.min - b.min;
	if (result.msec < 0) {
		result.sec --;
		result.msec += 1000;
	}
	if (result.sec < 0) {
		result.min --;
		result.sec += 60;
	}
	if (result.min < 0) {
		result.hrs --;
		result.min += 60;
	}
	return result;
}

//--------------------------------------------------------------------------------------------------------------

void customize() {
	// COMING SOON
	MAP_WIDTH = 60;
	MAP_HEIGHT = 15;
	sensor_amount = 18;
	treasure_left = 3;
	for (int i = 0, current; i < MAP_WIDTH; i ++) {
		for (int j = 0; j < MAP_HEIGHT; j ++) {
			current = i * MAP_HEIGHT + j;
			table[current].x = i;
			table[current].y = j;
		}
	}
	random_shuffle(table, table + MAP_WIDTH * MAP_HEIGHT);
}

void generate_rand_pos() {
	for (int i = 0; i < sensor_amount; i ++) {
		treasure.push_back(table[i]);
	}
}

int dist(pos a, pos b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

void initialize_map(vector<two_dimension> &temp, int row, int column) {
	crosshair.x = 0;
	crosshair.y = 0;
	for (int i = 0; i < row; i ++) {
		two_dimension subtemp;
		map_node nodetemp;
		pos tempcoord;
		for (int j = 0; j < column; j ++) {
			tempcoord.x = j;
			tempcoord.y = i;
			nodetemp.value = dist(tempcoord, treasure[0]);
			for (int k = 1; k < treasure_left; k ++) {
				nodetemp.value = min(nodetemp.value, dist(tempcoord, treasure[k]));
			}
			nodetemp.state = 0;
			subtemp.sub.push_back(nodetemp);
		}
		temp.push_back(subtemp);
	}
}


void texture_print(map_node temp) {
	if (!temp.state) {
		cout << char(219) /*temp[i].sub[j].value % 10*/;
	} else if (temp.value > 9) {
		cout << "-";
	} else if (temp.value == 0) {
		cout << "@";
	} else {
		cout << char(temp.value + 48);
	}
}

void print_map(vector<two_dimension> &temp, int row, int column) {
	system("cls");
	WHITE;
	for (int i = 10; i < column; i += 10) {
		locate(5 + i, 1);
		cout << i / 10;
	}
	locate(5, 2);
	for (int i = 0; i < column; i ++) {
		cout << i % 10;
	}
	locate(1, 4);
	for (int i = 0; i < row; i ++) {
		cout << " " << setfill(' ') << setw(2) << i << " ";
		for (int j = 0; j < column; j ++) {
			if (i == 0 && j == 0) {
				AQUA;
			}
			if (i == 0 && j > 0) {
				BLUE;
			}
			if (i > 0 && j == 0) {
				GREEN;
			}
			texture_print(temp[i].sub[j]);
			if (i == 0 || j == 0) {
				WHITE;
			}
		}
		cout << " " << i << "\n";
	}
	locate(5, 20);
	for (int i = 0; i < column; i ++) {
		cout << i % 10;
	}
	for (int i = 10; i < column; i += 10) {
		locate(5 + i, 21);
		cout << i / 10;
	}
	locate(4, 3);
	cout << char(218);
	locate(4, 19);
	cout << char(192);
	locate(65, 3);
	cout << char(191);
	locate(65, 19);
	cout << char(217);
	locate(1, 22);
	for (int i = 0; i < 68; i ++) {
		cout << char(205);
	}
	locate(69, 22);
	cout << char(185);
	for (int i = 1; i <= 25; i ++) {
		if (i == 22) i ++;
		locate(69, i);
		cout << char(186);
	}
	locate(70, 1);
	RED;
	cout << "Timer";
	locate(70, 4);
	GREEN;
	cout << "Treasure";
	locate(70, 7);
	YELLOW;
	cout << "Sensor";
	locate(70, 10);
	PINK;
	cout << "Coord";
	locate(70, 13);
	AQUA;
	cout << "Action";
	locate(70, 16);
	BLUE;
	cout << "MODE";
	WHITE;
	locate(70, 2);
	cout << "00:00.000";
	locate(70, 5);
	cout << "3 left";
	locate(70, 8);
	cout << "18 remain";
	locate(70, 11);
	cout << "0 : 0";
	locate(70, 14);
	cout << "None";
	locate(70, 17);
	cout << "Rank Timing";
}

void endgame_review(vector<two_dimension> &temp, int row, int column) {
	system("cls");
	locate(35, 2);
	WHITE;
	cout << "GAME  REVIEW";
	for (int i = 0; i < row; i ++) {
		locate(11, i + 4);
		for (int j = 0; j < column; j ++) {
			if (temp[i].sub[j].state) {
				GREEN;
			} else if (temp[i].sub[j].value == 0) {
				RED;
			}
			if (temp[i].sub[j].value < 10){
				temp[i].sub[j].state = 1;
			}
			texture_print(temp[i].sub[j]);
			if (temp[i].sub[j].state || temp[i].sub[j].value == 0) {
				WHITE;
			}
		}
	}
}

void motion(string cmd) {
	if (cmd == "place") {
		treasure_map[crosshair.y].sub[crosshair.x].state = 1;
		AQUA;
		locate(crosshair.x + 5, crosshair.y + 4);
		texture_print(treasure_map[crosshair.y].sub[crosshair.x]);
		WHITE;
		if (treasure_map[crosshair.y].sub[crosshair.x].value == 0) {
			treasure_left --;
			locate(70, 5);
			cout << treasure_left;
		}
	} else if (cmd == "left" || cmd == "right") {
		WHITE;
		for (int i = 0; i < MAP_HEIGHT; i ++) {
			if (i == crosshair.y) {
				BLUE;
			}
			locate(crosshair.x + 5, i + 4);
			texture_print(treasure_map[i].sub[crosshair.x]);
			if (i == crosshair.y) {
				WHITE;
			}
		}
		if (cmd == "left") {
			crosshair.x = (crosshair.x > 0)? crosshair.x - 1 : 0;
		} else {
			crosshair.x = (crosshair.x < MAP_WIDTH - 1)? crosshair.x + 1: crosshair.x;
		}
		GREEN;
		for (int i = 0; i < MAP_HEIGHT; i ++) {
			if (i == crosshair.y) {
				AQUA;
			}
			locate(crosshair.x + 5, i + 4);
			texture_print(treasure_map[i].sub[crosshair.x]);
			if (i == crosshair.y) {
				GREEN;
			}
		}
		WHITE;
	} else if (cmd == "up" || cmd == "down") {
		WHITE;
		locate(5, crosshair.y + 4);
		for (int i = 0; i < MAP_WIDTH; i ++ ) {
			if (i == crosshair.x) {
				GREEN;
			}
			texture_print(treasure_map[crosshair.y].sub[i]);
			if (i == crosshair.x) {
				WHITE;
			}
		}
		if (cmd == "up") {
			crosshair.y = (crosshair.y > 0)? crosshair.y - 1 : 0;
		} else {
			crosshair.y = (crosshair.y < MAP_HEIGHT - 1)? crosshair.y + 1 : crosshair.y;
		}
		BLUE;
		locate(5, crosshair.y + 4);
		for (int i = 0; i < MAP_WIDTH; i ++) {
			if (i == crosshair.x) {
				AQUA;
			}
			texture_print(treasure_map[crosshair.y].sub[i]);
			if (i == crosshair.x) {
				BLUE;
			}
		}
		WHITE;
	}
}

time_container interact() {
	string cmd;
	time_container start_time, current;
	bool actionflag = 0;
	locate(1, 23);
	cout << "You have " << sensor_amount << " sensors in total and " << treasure_left << " treasure hide in map at all.\nPress A S D W to move, ENTER to place the sensor.";
	while (treasure_left > 0 && sensor_amount > 0) {
		Sleep(100);
		cmd = "";
		if (!actionflag) {
			start_time = read_time(GetTickCount());
		}
		current = sub(read_time(GetTickCount()), start_time);
		locate(70, 2);
		cout << setfill('0') << setw(2) << current.min << ":" << setw(2) << current.sec << "." << setw(3) << current.msec << "  ";
		
		if (GetKeyState(65) < 0) {
			cmd = "left";
		} else if (GetKeyState(83) < 0) {
			cmd = "down";
		} else if (GetKeyState(68) < 0) {
			cmd = "right";
		} else if (GetKeyState(87) < 0) {
			cmd = "up";
		} else if (GetKeyState(13) < 0) {
			if (treasure_map[crosshair.y].sub[crosshair.x].state) {
				continue;
			}
			cmd = "place";
			sensor_amount --;
			locate(70, 8);
			cout << sensor_amount << " remain ";
		}
		if (cmd != "") {
			if (!actionflag) {
				actionflag = 1;
			}
			motion(cmd);
			locate(70, 11);
			cout << crosshair.x << " : " << crosshair.y << "  ";
			locate(70, 14);
			cout << cmd << "   ";
		}
	}
	return current;
}

//---------------------------------------------------------------------------------------------------------

void start_screen() {
	typer(33, 12, "FORTUNE   HUNTER", 50);
	typer(26, 14, "by Wumian, E.Tianyuan Workshop", 20);
	Sleep(1000);
}

void GUIDE() {
	// COMING SOON
}

void show_rule() {
	system("cls");
	WHITE;
	typer(1, 1, "This is a treasure-find game, which you own limited chance and have to figure   out where the treasures are.", 10);
	typer(1, 4, "You can press A S D W to move your crosshair, and Enter to place a sensor (cost 1 chance).", 10);
	string temp = "Once you placed a sensor, the '";
	typer(1, 7, temp + char(219) + "' block will change to a number ('1' to '9'),   represent the distance to the NEARest treasure, a '@', the treasure icon, or    a '-' to show nothing in the 9 blocks range.", 10);
	cout << "\n\n";
	system("pause");
}

void settings() {
	// beta 1
}

void game() {
	system("cls");
	YELLOW;
	srand((unsigned)time(NULL));
	cout << "Random initialized.\n";
	customize();
	cout << "Map memory announced.\n";
	generate_rand_pos();
	cout << "Treasure initialized.\n";
	initialize_map(treasure_map, MAP_HEIGHT, MAP_WIDTH);
	cout << "Map initialized.\n";
	system("pause");
	WHITE;
	
	print_map(treasure_map, MAP_HEIGHT, MAP_WIDTH);
	time_container game_time = interact();
	endgame_review(treasure_map, MAP_HEIGHT, MAP_WIDTH);
	cout << "\n\n";
	for (int i = 0; i < 80; i ++) {
		cout << char(234);
	}
	cout << "\n";
	if (treasure_left == 0) {
		cout << "Congrats!\nYou have found all treasures using " << 18 - sensor_amount << " sensors.\n";
	} else {
		cout << "What a pity!\nStill " << treasure_left << " treasure(s) not found, but you are running out of sensors.\n";
	}
	cout << "You spend " << setfill('0') << setw(2) << game_time.min << ":" << setw(2) << game_time.sec << "." << setw(3) << game_time.msec << " on this match.\n";
	system("pause");
}

void start_button() {
	locate(7, 21);
	cout << char(201);
	for (int i = 0; i < 5; i ++)
		cout << char(205);
	cout << char(187);
	locate(7, 22);
	cout << char(186) << "START" << char(186);
	locate(7, 23);
	cout << char(200);
	for (int i = 0; i < 5; i ++)
		cout << char(205);
	cout << char(188);
}

void guide_button() {
	locate(22, 21);
	cout << char(201);
	for (int i = 0; i < 5; i ++)
		cout << char(205);
	cout << char(187);
	locate(22, 22);
	cout << char(186) << "GUIDE" << char(186);
	locate(22, 23);
	cout << char(200);
	for (int i = 0; i < 5; i ++)
		cout << char(205);
	cout << char(188);
}

void rules_button() {
	locate(37, 21);
	cout << char(201);
	for (int i = 0; i < 5; i ++)
		cout << char(205);
	cout << char(187);
	locate(37, 22);
	cout << char(186) << "RULES" << char(186);
	locate(37, 23);
	cout << char(200);
	for (int i = 0; i < 5; i ++)
		cout << char(205);
	cout << char(188);
}

void settings_button() {
	locate(52, 21);
	cout << char(201);
	for (int i = 0; i < 8; i ++)
		cout << char(205);
	cout << char(187);
	locate(52, 22);
	cout << char(186) << "SETTINGS" << char(186);
	locate(52, 23);
	cout << char(200);
	for (int i = 0; i < 8; i ++)
		cout << char(205);
	cout << char(188);
}

void exit_button() {
	locate(69, 21);
	cout << char(201);
	for (int i = 0; i < 4; i ++)
		cout << char(205);
	cout << char(187);
	locate(69, 22);
	cout << char(186) << "EXIT" << char(186);
	locate(69, 23);
	cout << char(200);
	for (int i = 0; i < 4; i ++)
		cout << char(205);
	cout << char(188);
}

void auto_button_print(int cmd) {
	switch (cmd) {
		case(1):
			start_button();
			break;
		case(2):
			guide_button();
			break;
		case(3):
			rules_button();
			break;
		case(4):
			settings_button();
			break;
		case(5):
			exit_button();
			break;
		default:
			break;
	}
}

int menu() {
	// start guide rules settings

	system("cls");
	int cmd = 1;
	
	YELLOW;
	locate(1, 6);
	typer(13, 6, "#####  ###  ####  ##### #   # #   # #####", 1);
	typer(13, 7, "#     #   # #   #   #   #   # ##  # #    ", 1);
	typer(13, 8, "###   #   # ####    #   #   # # # # #### ", 1);
	typer(13, 9, "#     #   # #  #    #   #   # #  ## #    ", 1);
	typer(13, 10, "#      ###  #   #   #    ###  #   # #####", 1);
	typer(33, 12, "#   # #   # #   # ##### ##### #### ", 1);
	typer(33, 13, "#   # #   # ##  #   #   #     #   #", 1);
	typer(33, 14, "##### #   # # # #   #   ####  #### ", 1);
	typer(33, 15, "#   # #   # #  ##   #   #     #  # ", 1);
	typer(33, 16, "#   #  ###  #   #   #   ##### #   #", 1);

	GREEN;
	auto_button_print(1);
	WHITE;
	for (int i = 2; i < 6; i ++)
		auto_button_print(i);
	
	int left = GetKeyState(VK_LEFT), right = GetKeyState(VK_RIGHT);
	while (1) {
		if (GetKeyState(13) < 0) {
			break;
		}
		if (GetKeyState(VK_LEFT) == 1 - left) {
			left = 1 - left;
			WHITE;
			auto_button_print(cmd);
			cmd = (cmd == 1)? 5 : cmd - 1;
			GREEN;
			auto_button_print(cmd);
		}
		if (GetKeyState(VK_RIGHT) == 1 - right) {
			right = 1 - right;
			WHITE;
			auto_button_print(cmd);
			cmd = (cmd == 5)? 1 : cmd + 1;
			GREEN;
			auto_button_print(cmd);	
		}
	}
	return cmd;
}

int main() {
	start_screen();
	while (1) {
		int cmd = menu();
		switch (cmd) {
			case(1):
				game();
				reset();
				break;
			case(2):
				break;
			case(3):
				show_rule();
				break;
			case(4):
				break;
			case(5):
				return 0;
			default:
				break;
		}
	}
}




#include <cstdlib>
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <format>
#include <algorithm>
#include <limits>

#include <conio.h>
#include <windows.h>

//function macro max was defined in windows.h or its relative files
#ifdef max
#undef max
#endif

using namespace std;

//----------

//const string  img_src("D:\\Raiscies\\BadApple Project\\HexImageData.txt"); 
const string  img_src(".\\HexImageData.txt");
const uint8_t img_fps = 30;

uint32_t current_frame = 0;
ifstream stm(img_src, ios::in);
//----------


HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
COORD pos = {0, 0}; //pos.X = 0; pos.Y = 0;
CONSOLE_CURSOR_INFO cursor_info = {1, 0};

void clean_screen() {
	SetConsoleCursorPosition(handle, pos);
	//hide cursor
	SetConsoleCursorInfo(handle, &cursor_info);
}

template <char32_t light_char = L'¡ñ', char32_t dark_char = L'¡¡'>
void play_all() {

	//traverse frames
	while (!stm.eof()) {
		string frame_data;
		getline(stm, frame_data);
		const char* pos = frame_data.c_str();
		
		//traverse rows
		for(int i = 0; i < 32; i++){
			//purse string to integer
			//pos shifted at here
			uint32_t hex_row = strtoul(pos, const_cast<char**>(&pos), 16);

			//purse hex integer to binary string
			wstring row = format(L"{0:>32b}\n", hex_row);
			//replace chars
			ranges::replace(row, L'0', dark_char);
			ranges::replace(row, L'1', light_char);

			//print
			wcout << row;
			
		}
	
		//system("cls");
		clean_screen();
	}
}

void skip_line(uint32_t row) {

	for (uint32_t i = 0; i < row; i++) stm.ignore(numeric_limits<std::streamsize>::max(), '\n');

}

//return if is NOT at end
template <char32_t light_char = L'¡ñ', char32_t dark_char = L'¡¡'>
bool update_frame(uint32_t frame) {
	if (stm.eof()) return false;
	if (frame <= current_frame) return true;
	skip_line(frame - current_frame - 1);
	current_frame = frame;
	
	string frame_data;
	getline(stm, frame_data);
	const char* pos = frame_data.c_str();
	
	//system("cls");
	clean_screen();
	 
	//traverse rows
	for (int i = 0; i < 32; i++) {
		//purse string to integer
		//pos is shifted at here
		uint32_t hex_row = strtoul(pos, const_cast<char**>(&pos), 16);

		//purse hex integer to binary string
		wstring row = format(L"{:¡¡>32b}\n", hex_row);
		//replace chars
		ranges::replace(row, L'0', dark_char);
		ranges::replace(row, L'1', light_char);

		//print
		wcout << row;

	}
	return true;
}

void play_image(uint8_t fps = 0) {
	//fps == 0 means using max speed to play image

	if(!stm.is_open()) {
		
		cout << "Failed to open file: " << img_src << endl;
		return;
	}

	if (fps == 0) {
		play_all();
		return;
	}
	
	auto start = chrono::steady_clock::now();
	while (update_frame(static_cast<uint32_t>(chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - start).count() * fps / 1000.0)));


}

int main() {
	
	//set local envirnment
	wcout.imbue(std::locale("chs"));
	
	clean_screen();
	play_image(img_fps);
	cout << "finished." << endl;
	return 0;
}
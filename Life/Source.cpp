#include <iostream>
#include <windows.h>
#include <fstream>
void life_cycle(char** galaxy_tab, char** galaxy_tab_buf, int galaxy_tab_height, int galaxy_tab_length, int* alive_cells, bool& flag);
char** create_array(int x, int y);
void delete_array(char** arr, int x);
void print_array(char** arr, int x, int y);

int main() {
	std::ifstream fin("input_life.txt");
	if (fin.is_open()) {
		int galaxy_tab_height, galaxy_tab_length;
		fin >> galaxy_tab_height >> galaxy_tab_length;
		char** galaxy_tab = create_array(galaxy_tab_height, galaxy_tab_length);
		char** galaxy_tab_buf = create_array(galaxy_tab_height, galaxy_tab_length);
		for (int i = 0; i < galaxy_tab_height; i++) {
			for (int j = 0; j < galaxy_tab_length; j++) {
				galaxy_tab[i][j] = '-';
				galaxy_tab_buf[i][j] = '-'; // buffer table
			}
		}
		int active_cell_x, active_cell_y; // input active cells in the galaxy_tab
		int alive_cells = 0;
		while (!(fin.eof())) {
			alive_cells++;
			fin >> active_cell_x >> active_cell_y;
			galaxy_tab[active_cell_x][active_cell_y] = '*';
			galaxy_tab_buf[active_cell_x][active_cell_y] = '*';
		}
		print_array(galaxy_tab, galaxy_tab_height, galaxy_tab_length);

		int generation = 1;
		std::cout << "Generation: " << generation << " Alive cells: " << alive_cells << '\n';
		bool flag; // flag is needed to compare the new table (galaxy_tab_buf) with the old table (galaxy_tab) 
		do {
			Sleep(10);
			if (alive_cells == 0) {
				std::cout << "All cells are dead. Game over";
				break;
			}
			flag = true;
			life_cycle(galaxy_tab, galaxy_tab_buf, galaxy_tab_height, galaxy_tab_length, &alive_cells, flag);
			print_array(galaxy_tab, galaxy_tab_height, galaxy_tab_length);
			generation++;
			std::cout << "Generation: " << generation << " Alive cells: " << alive_cells << '\n';
		} while (!flag);
		if (alive_cells > 0) {
			std::cout << "The world has stagnated. Game over";
		}
		delete_array(galaxy_tab, galaxy_tab_height);
		delete_array(galaxy_tab_buf, galaxy_tab_height);

	}
	else std::cout << "File input.txt isn't opened";
	fin.close();
	return 0;
}


void life_cycle(char** galaxy_tab, char** galaxy_tab_buf, int galaxy_tab_height, int galaxy_tab_length, int* alive_cells, bool& flag) {
	int count_alive;
	int count_dead;
	for (int i = 0; i < galaxy_tab_height; i++) {
		for (int j = 0; j < galaxy_tab_length; j++) {
			if (galaxy_tab[i][j] == '-') {
				count_alive = 0;
				count_dead = -1;
			}
			else {
				count_alive = -1;
				count_dead = 0;
			}
			// check every cell around a chosen cell
			// k = i - 1; k < i + 2 and l = j - 1; l < j + 2 are used for checking 8 cells around a chosen cell
			// if k = -1 or l <= -1 or k = i + 1 or l = j + 1 then the cell in the corner of the galaxy_tab is checking
			for (int k = i - 1; k < i + 2; k++) {
				if (k < 0)
					continue;
				for (int l = j - 1; l < j + 2; l++) {
					if (k >= 0 && k < galaxy_tab_height && l >= 0 && l < galaxy_tab_length) {
						if (galaxy_tab[k][l] == '-') {
							count_dead++;
						}
						else {
							count_alive++;
						}
					}
				}
			}
			if (galaxy_tab[i][j] == '-' && count_alive == 3) {
				galaxy_tab_buf[i][j] = '*';
				*alive_cells += 1;
			}
			else if ((galaxy_tab[i][j] == '*' && count_alive > 3) || (galaxy_tab[i][j] == '*' && count_alive < 2)) {
				galaxy_tab_buf[i][j] = '-';
				*alive_cells -= 1;
			}
		}
	}
	for (int i = 0; i < galaxy_tab_height; i++) {
		for (int j = 0; j < galaxy_tab_length; j++) {
			if (galaxy_tab[i][j] != galaxy_tab_buf[i][j]) {
				galaxy_tab[i][j] = galaxy_tab_buf[i][j];
				flag = false;
			}
		}
	}
}

char** create_array(int x, int y) {
	char** arr = new char* [x];
	for (int i = 0; i < x; i++)
		arr[i] = new char[y];
	return arr;
}

void delete_array(char** arr, int x) {
	for (int i = 0; i < x; i++)
		delete[] arr[i];
	delete[] arr;
	arr = nullptr;
}

void print_array(char** arr, int x, int y) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			std::cout << arr[i][j] << ' ';
		}
		std::cout << '\n';
	}
}
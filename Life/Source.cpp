#include <iostream>
#include <windows.h>
#include <fstream>
void life_cycle(char** galaxy_tab, char** galaxy_tab_buf, int n, int m, int* alive_cells, bool& flag);


int main() {
	std::fstream f("Galaxy.txt", std::ios::in | std::ios::out | std::ios::trunc);
	std::ifstream fin("input.txt");
	if (fin.is_open()) {
		int n, m;
		fin >> n >> m;
		char** galaxy_tab = new char* [n];
		for (int i = 0; i < n; i++)
			galaxy_tab[i] = new char[m];
		char** galaxy_tab_buf = new char* [n];
		for (int i = 0; i < n; i++)
			galaxy_tab_buf[i] = new char[m];

		if (f.is_open())
		{
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					galaxy_tab[i][j] = '-'; 
					galaxy_tab_buf[i][j] = '-'; // buffer table
				}
			}
			int x, y;
			int alive_cells = 0;
			while (!(fin.eof())) {
				alive_cells++;
				fin >> x >> y;
				galaxy_tab[x][y] = '*';
				galaxy_tab_buf[x][y] = '*';
			}

			// fill in the Galaxy by init conditions
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					f << galaxy_tab[i][j] << ' ';
				}
				f << '\n';
			}
			f.seekg(0);
			// print the first generation of the Galaxy
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					f >> galaxy_tab[i][j];
					std::cout << galaxy_tab[i][j] << ' ';
				}
				std::cout << '\n';
			}

			int generation = 1;
			std::cout << "Generation: " << generation << " Alive cells: " << alive_cells << '\n';
			bool flag;
			do {
				Sleep(1000);
				if (alive_cells == 0) {
					std::cout << "All cells are dead. Game over";
					break;
				}
				flag = true;
				f.seekg(0);
				life_cycle(galaxy_tab, galaxy_tab_buf, n, m, &alive_cells, flag);
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						f << galaxy_tab[i][j] << ' ';
					}
					f << '\n';
				}
				f.seekg(0);
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						f >> galaxy_tab[i][j];
						std::cout << galaxy_tab[i][j] << ' ';
					}
					std::cout << '\n';
				}
				generation++;
				std::cout << "Generation: " << generation << " Alive cells: " << alive_cells << '\n';
			} while (!flag);
			if (alive_cells > 0)
				std::cout << "The world has stagnated. Game over";
		}
		else
			std::cout << "File Galaxy.txt isn't opened";
		for (int i = 0; i < n; i++)
			delete[] galaxy_tab[i];
		delete[] galaxy_tab;
		galaxy_tab = nullptr;
		for (int i = 0; i < n; i++)
			delete[] galaxy_tab_buf[i];
		delete[] galaxy_tab_buf;
		galaxy_tab_buf = nullptr;
	}
	else std::cout << "File input.txt isn't opened";
	f.close();
	fin.close();
	return 0;
}


void life_cycle(char** galaxy_tab, char** galaxy_tab_buf, int n, int m, int* alive_cells, bool& flag) {
	int count_alive;
	int count_dead;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (galaxy_tab[i][j] == '-') {
				count_alive = 0;
				count_dead = -1;
			}
			else {
				count_alive = -1;
				count_dead = 0;
			}
			for (int k = i - 1; k < i + 2; k++) {
				for (int l = j - 1; l < j + 2; l++) {
					if (k >= 0 && k < n && l >= 0 && l < m) {
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
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (galaxy_tab[i][j] != galaxy_tab_buf[i][j]) {
				galaxy_tab[i][j] = galaxy_tab_buf[i][j];
				flag = false;
			}
		}
	}
}
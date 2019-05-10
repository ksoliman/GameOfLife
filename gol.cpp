/*
 * CSc103 Project 3: Game of Life
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References:
 *
 *
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 
 */
#include<iostream>
#include <cstdio>
#include <stdlib.h> // for exit();
#include <getopt.h> // to parse long arguments.
#include <unistd.h> // sleep
#include <vector>
#include<fstream>
using namespace std;
using std::vector;
#include <string>
using std::string;
using std::cin;
using std::cout;

 static const char* usage =
"Usage: %s [OPTIONS]...\n"
"Text-based version of Conway's game of life.\n\n"
"   --seed,-s     FILE     read start state from FILE.\n"
"   --world,-w    FILE     store current world in FILE.\n"
"   --fast-fw,-f  NUM      evolve system for NUM generations and quit.\n"
"   --help,-h              show this message and exit.\n"; 

size_t max_gen = 0; /* if > 0, fast forward to this generation. */
string wfilename =  "/tmp/gol-world-current"; /* write state here */
FILE* fworld = 0; /* handle to file wfilename. */
string initfilename = "/tmp/gol-world-current"; /* read initial state from here. */

size_t nbrCount(size_t i, size_t j, const vector<vector<bool> > &g);
void update();
int initFromFile(const string& fname);
void mainLoop();
vector<vector<bool> > copyGrid(); 
vector<vector<bool> > grid;
void dumpstate(FILE* f);

char text[3] = ".O";


int initFromFile(const string& fname){
	vector<bool> vec1;
	char c;

		/* Open the file (for reading), storing the handle in f: */
		FILE* f = fopen(fname.c_str(),"rb");
		/* Read a single character from the file: */

	while(fread(&c,1,1,f) != 0){
		if(c == '\n') {
			grid.push_back(vec1);
			vec1.clear();
		}
		else 
		{
			if(c == '.'){
				vec1.push_back(false);
			}

			else{
				vec1.push_back(true);
			}
		}
	}

	/* when you're done with the file, be sure to close it like this: */
	fclose(f);
}

int main(int argc, char *argv[]) {

	// define long options
	static struct option long_opts[] = {
		{"seed",    required_argument, 0, 's'},
		{"world",   required_argument, 0, 'w'},
		{"fast-fw", required_argument, 0, 'f'},
		{"help",    no_argument,       0, 'h'},
		{0,0,0,0}
	};
	// process options:
	char c;
	int opt_index = 0;
	while ((c = getopt_long(argc, argv, "hs:w:f:", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 'h':
				printf(usage,argv[0]);
				return 0;
			case 's':
				cout << "param is s";
				initfilename = optarg;
				break;
			case 'w':
				wfilename = optarg;
				break;
			case 'f':
				max_gen = atoi(optarg);
				break;
			case '?':
				printf(usage,*argv[0]);
				return 1;
		}
	}
	initFromFile(initfilename);
	for (unsigned int i = 0 ; i < max_gen ; i++)
	{
	mainLoop();
	}
	return 0;
}



bool computeCellValueAtPos(int currentX, int currentY, string position){
			int gridLength = grid.size();
			int gridWidth = grid[currentX].size();
			if(position == "upper"){

				return grid[(currentX - 1 + gridLength)%gridLength][currentY];

			}else if(position == "lower"){
				return grid[(currentX + 1)%gridLength][currentY];

			}else if(position== "left"){
				return grid[currentX][(currentY - 1 + gridWidth) % gridWidth];

			}else if (position == "right"){
				return grid[currentX][(currentY + 1)% gridWidth];

			}else if(position == "upperLeft"){
				return grid[(currentX -1 + gridLength)%gridLength][(currentY - 1 + gridWidth)% gridWidth];

			}else if(position == "upperRight"){
				return grid[(currentX - 1 + gridLength)%gridLength][(currentY + 1)%gridWidth];

			}else if(position == "lowerRight"){
				return grid[(currentX + 1) % gridLength ][(currentY + 1 ) % gridWidth];

			}else if (position =="lowerLeft"){
				return grid[(currentX + 1) % gridLength][(currentY -1 + gridWidth) % gridWidth];
			}
	}

void update(){
	vector<vector<bool> > newGrid = copyGrid();

	for(unsigned int x = 0; x < grid.size() ; x++){

		for(unsigned int y = 0; y < grid[x].size() ; y++){

			int neighbor_sum =  
				computeCellValueAtPos(x , y, "left") + 
				computeCellValueAtPos(x , y, "upperLeft") + 
				computeCellValueAtPos(x , y, "upper") +
				computeCellValueAtPos(x , y, "upperRight") + 
				computeCellValueAtPos(x , y, "right") + 
				computeCellValueAtPos(x , y, "lowerRight") + 
				computeCellValueAtPos(x , y, "lower") + 
				computeCellValueAtPos(x , y, "lowerLeft");

			int live_state = grid[x][y];
		
			if(live_state == 0){
				if (neighbor_sum == 3){
					newGrid[x][y] = 1;
				}
			}
			else if(live_state == 1){
				if (neighbor_sum < 2 || neighbor_sum > 3){
					newGrid[x][y] = 0;
				}
			}
		}

	}

	grid = newGrid;
}
	

void writeState(){
for (unsigned int x = 0; x < grid.size(); x++)
{
	for (unsigned int y = 0; y < grid[x].size(); y++)
	{
		if (grid[x][y] == true)
		{
			cout << "0";
		}
		else
			cout << ".";
	}
	cout << "\n";
}

}


vector<vector<bool> >  copyGrid(){

	vector<vector<bool> > newGrid;
	vector<bool> row;
	
	for (unsigned int k = 0 ; k < grid.size() ; k++) {
		for(unsigned int i=0 ;  i< grid[k].size() ; i++){
		
			row.push_back(grid[k][i]);
        
		}
		newGrid.push_back(row);
		row.clear();
	}
return newGrid;

}

void mainLoop() {

update();
writeState();
sleep(1);


		

	}
	/* update, write, sleep */


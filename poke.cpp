#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

struct Move {
    string name;
    int powerPoints;
    int power;
    double accurancy;
    string type;
};

struct Pokemon {
    string name;
    int id;
    int pokedexNo;
    int health;
    string type;
    vector<Move> moves;
}; 

// Game's Pokedex
vector<Pokemon> pokedex = {
     {"Squirtle", 2, 1, 30, "water"},
     {"Pikachu", 4, 2, 40, "electric"},
     {"Charmander", 5, 3, 45, "fire"},
     {"Venusaur", 3, 4, 35, "grass"},
};
// Grass moves
vector<Move> grassMoves = {
     {"Seed Bomb", 30, 50, 1.00, "grass"},
     {"Frenzy Plant", 10, 200, 1.00, "grass"},
     {"Leaf Blade", 25, 25, 0.9, "grass",}
};
// Water moves
vector<Move> waterMoves = {
      {"Clamp", 10, 35, 0.92, "water"},
      {"Dive", 10, 80, 1.00, "water"},
      {"Liquidation", 10, 10, 0.85, "water"},  // lower the target's defense state.
};
// Fire moves
vector<Move> fireMoves = {
      {"Blast Burn", 25, 50, 1.00, "fire"},
      {"Blaze Kick", 30, 56, 1.00, "fire"},
      {"Fire Spin", 15, 35, 0.85, "fire"},
}; 
// Electric moves
vector<Move> electricMoves = {
      {"Zapdos", 10, 200, 1.00 ,"electric" },   // flying and electric type
      {"Thunderbolt", 10, 110, 0.7, "electric"},
      {"Jolteon", 10, 150, 1.00, "electric"},   // fur is unique as it’s sharp and amplify electricity
};

// Global Variables
const int x = 10;
const int y = 10;
int row;              // stores the previous row
int col;              // stores the previous column
char grid[x][y];
char display_grid[x][y];
int mapOpt;           // choose the map option of the grid
int howMany = 7;      // how many wild pokemons in the field?
int pokemonMet = 0;   // how many pokemon have the user met?
int energy = 3;       // how many energy in the field?
char citySymbol = ' ';
Pokemon getPokemon();
vector<Move> determineMoves(vector<string> m);  // determine the movie and push it back in to vector

// Function Prototype
void initializeDisplay_grid();
void printGrid_NY();
void printGrid_WA();
void printGrid_LA();
void printPokemon(Pokemon p);
void readPokedex();
void showGrid();
bool enterGuess();
int formatWorld_map();
bool checkWorldMap();
void DetermineChoice();
void displayMoves(Pokemon p);
void placePokemon(int num);
void print_energy(int num);

bool check_WP_Poistion (int row, int col);
bool pokemonBattle(Pokemon& p, Pokemon w);
bool useMove(Pokemon& p, Pokemon& e, int m);
bool is_hit_energy(int row, int col, Pokemon& p);

// Main program
int main(){

srand(time(NULL));  

// The user's starter Pokemon
Pokemon me;
me.name = "Eevee";
me.id = 7;
me.pokedexNo = 133; 
me.type = "Normal";
me.health = 100;
me.moves = determineMoves ({"electric","fire","water","grass"});
// the user's location 
row = x - 1; // starts at the position bottom
col = y - 5; // starts at the position middle

// ASCII art to illustrate "Welcome to the world of Pokemon!!!"
cout << R"(
===================================================================================
 _      _____ _     ____  ____  _      _____   _____  ____                      
/ \  /|/  __// \   /   _\/  _ \/ \__/|/  __/  /__ __\/  _ \                     
| |  |||  \  | |   |  /  | / \|| |\/|||  \      / \  | / \|                     
| |/\|||  /_ | |_/\|  \__| \_/|| |  |||  /_     | |  | \_/|                     
\_/  \|\____\\____/\____/\____/\_/  \|\____\    \_/  \____/                     
             _____  _     _____   _      ____  ____  _     ____    ____  _____  
            /__ __\/ \ /|/  __/  / \  /|/  _ \/  __\/ \   /  _ \  /  _ \/    /  
              / \  | |_|||  \    | |  ||| / \||  \/|| |   | | \|  | / \||  __\  
              | |  | | |||  /_   | |/\||| \_/||    /| |_/\| |_/|  | \_/|| |     
              \_/  \_/ \|\____\  \_/  \|\____/\_/\_\\____/\____/  \____/\_/   
                      ____  ____  _  __ _____ _      ____  _        _  _  _ 
                     /  __\/  _ \/ |/ //  __// \__/|/  _ \/ \  /|  / \/ \/ \
                     |  \/|| / \||   / |  \  | |\/||| / \|| |\ ||  | || || |
                     |  __/| \_/||   \ |  /_ | |  ||| \_/|| | \||  \_/\_/\_/
                     \_/   \____/\_|\_\\____\\_/  \|\____/\_/  \|  (_)(_)(_)   

==================================================================================== )" << endl;
cout << endl;  
cout << " Your GOAL is to meet all " << howMany << " Pokemon on the field. " << endl;
cout << endl;
char enter;
cout << enter << " LET'S GO ~ ARE YOU READY??? ";
cin >> enter;

checkWorldMap();
DetermineChoice();
initializeDisplay_grid();
print_energy(energy);
display_grid[row][col]= '@'; // '@' Pokemon Eevee (it's me)
//display_grid[row][col] = 'X';
//placePokemon(howMany - 1);
placePokemon(howMany);
showGrid();

// Checks how many pokemon the user meet and how many still left. Also determines the user lost or win.
bool lost = false;
while (pokemonMet < howMany){
      if (enterGuess()){
          if (pokemonBattle(me, getPokemon())){  
                cout << " The wild pokemon fainted!" << endl;
                showGrid();
          }
          else {
                lost = true;
                break;
          }
      }
      else {
            cout << "Try again! (Press any key to continue...)" << endl;
            showGrid();
         }
      }
if (lost) {
      cout << "Sorry ~ Your Pokemon fainted." << endl;
   }
      else if (pokemonMet >= howMany){
            cout << endl;
            cout << "OMG ~ you've beaten all the wild Pokemon! Congratulation !!! " << endl;
 }
return 0;
}

// Checks if grid contain wild pokemon and found HP 
bool check_WP_Poistion (int row, int col){
      if (display_grid[row][col]== 'X'){
            cout << "Pokemon Battle!" << endl;
            display_grid[row][col]= citySymbol;
            return true;
      }
      if (display_grid[row][col]== '$'){
            cout << "Yesss ~ You gain energy points!(+50 HP)" << endl;
            display_grid[row][col]= citySymbol;
            return false;
      }
      else {
            cout << "There is no wild pokemon" << endl; 
            return false;
      }
}

// Battle between two Pokemon
bool pokemonBattle( Pokemon& p, Pokemon w){
      cout << "A wild " << w.name << " appeared! " << endl;
       while (w.health > 0 && p.health > 0){
            printPokemon(p);
            printPokemon(w);
            cout << endl;
            cout << p.name << " ~ What are you gonna do?" << endl;
            displayMoves(p);
            
            int moveSelection;
            cin >> moveSelection;
            while (!useMove(p,w,moveSelection-1)){
                  cout << "Please Try Again: " << endl;
                  cin.clear();
                  cin >> moveSelection;
            }
            if(w.health > 0){
                  int randEnemyMove = rand() % w.moves.size();
                  while (w.moves.at(randEnemyMove).powerPoints <= 0){
                        randEnemyMove = rand() % w.moves.size();
                        cout << "Please wait ~ recalculating" << endl;
                  }
                  useMove(w,p,randEnemyMove);
                  cout << w.name << " used " << w.moves.at(randEnemyMove).name << endl;
            }
       }
       if (p.health > 0){
             pokemonMet ++;
             return true;
       }
       return false;
}

// Check Pokemon move
bool useMove(Pokemon& p, Pokemon& e, int m){
    if (m < 4 && p.moves.at(m).powerPoints > 0) {
        p.moves.at(m).powerPoints--;
        e.health = e.health - p.moves.at(m).power;
        return true;
    }
    return false;
}

// Print gain energy symbol on the grid
void print_energy(int num){  
    for (int i = 0; i < num; i++ ){
          int row = rand() % x;
          int col = rand() % y;
                display_grid[row][col] = '$'; // Energy = $
          }
    }

// Check the user hit the energy symbol and gain health points
bool is_hit_energy(int row, int col, Pokemon& p){
      bool hit = false;
        if (display_grid[row][col]== '$'){
           hit = true;
           p.health += 50;
           cout << " Yessss, You've gain HP ! " << endl;
     }
      return false;
}

// Print detail information about a given Pokemon
void printPokemon(Pokemon p) {
    cout << "Name: " << p.name << "\t";
    cout << "Pokedex No.: " << p.pokedexNo << "\t";
    cout << "Type: " << p.type << "\t";
    cout << "HP: " << p.health << "\t"; 
    cout << endl;
}

// Print full Pokedex
void readPokedex() {
    for (int i = 0; i < pokedex.size(); i++) {
        printPokemon(pokedex.at(i));
    }
}

// Place wild Pokemons randomely in the grid and it's informaion
Pokemon getPokemon() {
    int randomPokedexNo = rand() % pokedex.size();
    Pokemon p;
    p.name = pokedex.at(randomPokedexNo).name;
    p.id = pokedex.at(randomPokedexNo).id;
    p.pokedexNo = pokedex.at(randomPokedexNo).pokedexNo;
    p.type = pokedex.at(randomPokedexNo).type;
    p.health = pokedex.at(randomPokedexNo).health;
    p.moves = determineMoves({p.type, p.type, "water", "fire"});
    return p;
}

// Print the Pokemon symbol on the grid
void placePokemon(int num) {
    for (int i = 0; i < num; i++){
        int row = rand() % x;
        int col = rand() % y;
        display_grid[row][col] = 'X';
    }
}

// Determine the moves and push back in to the vector and return back to moves
vector<Move> determineMoves(vector<string> m) {
    vector<Move> moves;
    for (int i = 0; i < m.size(); i++) {
        if (m.at(i) == "electric") {
            moves.push_back(electricMoves.at(rand() % electricMoves.size()));
        } else if (m.at(i) == "fire") {
            moves.push_back(fireMoves.at(rand() % fireMoves.size()));
        } else if (m.at(i) == "water") {
            moves.push_back(waterMoves.at(rand() % waterMoves.size()));
        } else if (m.at(i) == "grass") {
            moves.push_back(grassMoves.at(rand() % grassMoves.size()));
        }
    }
    return moves;
}

// Display possible move choices
void displayMoves(Pokemon p) {
      for (int i = 0; i < p.moves.size(); i++ ){
            cout << (i + 1) << ". " << p.moves.at(i).name << " (" << p.moves.at(i).powerPoints << "left: " << p.moves.at(i).power << " HP)" << endl;
      }
}

/*Shows the pokemon's world map and allows the user 
     to choose an option with city he/she want to visit.*/
int formatWorld_map() {
      int choice;
      cout << endl;
      cout << "+++++++++++++++++++++++++++++++++++ " << endl;
      cout << "  Welcome To Pokémon's World Map!   " << endl;
      cout << "+++++++++++++++++++++++++++++++++++ " << endl;
      cout << " Which city you want to travel? "     << endl;
      cout << " 1. New York City ! "                 << endl;
      cout << " 2. Seattle ! "                       << endl;
      cout << " 3. Los Angeles ! "                   << endl;
      cout << endl;
      cout << " Please enter your choice: "  ;
      cin >> choice;
    return choice;   
}
// Check to see if valid world map options were entered
bool checkWorldMap(){
      int choice = formatWorld_map();
      while (!(choice > 0 && choice < 4)) {
            cout << "Try again." << endl;
            choice = formatWorld_map();
      }
      mapOpt = choice;
      return true;
}
// Determine the user choice and display it.
void DetermineChoice(){
      switch(mapOpt){
            case 1:
            cout << " 1 " << endl;
            //printGrid_NY();
            citySymbol = '.';
            break;
            case 2:
            cout << " 2 " << endl;
            //printGrid_WA();
            citySymbol = '^';
            break;
            case 3:
            cout << " 3 " << endl;
            //printGrid_LA();
            citySymbol = '+';
            break;
      }
}
// Initializes game and its display
void initializeDisplay_grid(){
      for (int i = 0; i < x ; i++) {
            for (int j = 0; j < y; j++){
                  display_grid[i][j] = citySymbol;
                  grid[i][j] = ' '; 
            }
      }
}

/* Print the grid and ask the user which city he/she want to visit. 
      Tell the user how many Pokemons and HP gain on the grid, 
       also update how many Pokemon the user has meet */
void showGrid(){
      cin.ignore();
      string city;
      if (citySymbol == '.'){
            city = "New York City! ";
      } else if (citySymbol == '^'){
            city = "Seattle! ";
      } else if (citySymbol == '+'){
            city = "Los Angeles! ";
      }
      cout << endl;
      cout << "-------------------------------------------- "  << endl;
      cout <<  city << "   Meet: " << pokemonMet << " / Wild Pokemons: " << howMany << endl;
      cout <<   "\t" "\t" "\t"          "Energy: " << energy  << endl;
      cout << "---------------------------------------------" << endl;
      cout << "   ";                                 // Move over a space before printing the 'A'
       for (char a = 'A'; a <= 'J'; a++){
             cout << a << "  ";
       } cout << endl;
    
      for (int i = 0 ; i < 10 ; i++){
            cout << i << "  ";
            for (int y = 0; y < 10; y++){
                  cout << display_grid[i][y] << "  ";
      }
        cout << endl;
      }
}

// Ask the user to enter the row and coloum to guess where the ship had assigned.
bool enterGuess(){
    //cin.ignore();
    string guess;
    cout << "Please Enter the Location: ";
    getline(cin,guess);
    int n = guess.at(1)- 48;   // in ASCII 48 is the char of '0', n = row 
    int i = guess.at(0) - 65;  // in ASCII 65 is the char of 'A', i = column 
    display_grid[row][col] = citySymbol;
    row = n; // store the new previous location (row)
    col = i; // store the new previous location (col)
    bool hasPokemon;  // check the wild Pokemon on the grid and return ture 
    if(check_WP_Poistion (n,i)){
      hasPokemon = true;
    }
    else 
      hasPokemon = false;
      display_grid[n][i] = '@';
return hasPokemon;
}
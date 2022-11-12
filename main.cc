//Partners: Khushkaranpreet Grewal, Karma ???
//Bullet Points:
//Extra Credit:

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <sstream>
#include <ctype.h>
using namespace std;

void die(string s = "INVALID INPUT!") {
	cout << s << endl;
	exit(EXIT_FAILURE);
}

//Holds a record for a single species of Pokemon, such as Pikachu
class Pokemon {
  public:
	//YOU: Add member variable ints for each of the following:
	//index - what index it is in the pokedex
	int index = 0;
	//name (this is a string not an int)
	string name;
	//hp - how much damage it takes before dying fainting
	int hp = 0;
	//attack - this number gets multiplied by the move's power to determine damage
	int attack = 0;
	//defense - incoming damage gets divided by this number
	int defense = 0;
	//speed - whichever pokemon has the highest speed attacks first, flip a coin on a tie
	int speed = 0;
	//special - like attack and defense both, but for special moves
	int special = 0;
	//Note: this is also the order the data is ordered in the file
	//YOU: Add member variables of type string for type1 and type2
	string type1;
	string type2;
};
vector<Pokemon> pokemon_db; //Holds all pokemon known to mankind

//This function reads from the "pokemon.txt" file in the directory and loads the data into the pokemon_db global vector
//Each row has: pokemon number, name, hp, etc.
void load_pokemon_db(string filename = "pokemon.txt", string filename2 = "types.txt") {
	ifstream ins(filename);
	if (!ins) die("Couldn't load file "s + filename);
	ifstream ins2(filename2);
	if (!ins2) die("Couldn't load file"s + filename2);
	while (ins and ins2) {
		//YOU: Read from the files the data into a temporary Pokemon, getting the stats from pokemon.txt and its types from types.txt
		Pokemon temp;

		//Reads in primary pokemon data.
		string pokemonData;

		getline(ins, pokemonData);
		if (!ins) break;

		stringstream ssPokemonData(pokemonData);
		string index, name, hp, attack, defense, speed, special;

		getline(ssPokemonData, index, '\t');
		getline(ssPokemonData, name, '\t');
		getline(ssPokemonData, hp, '\t');
		getline(ssPokemonData, attack, '\t');
		getline(ssPokemonData, defense, '\t');
		getline(ssPokemonData, speed, '\t');
		getline(ssPokemonData, special, '\n');

		if (isdigit(index.at(0))) temp.index = stoi(index);
		temp.name = name;
		if (isdigit(hp.at(0))) temp.hp = stoi(hp);
		if (isdigit(attack.at(0))) temp.attack = stoi(attack);
		if (isdigit(defense.at(0))) temp.defense = stoi(defense);
		if (isdigit(speed.at(0))) temp.speed = stoi(speed);
		if (isdigit(special.at(0))) temp.special = stoi(special);

		//Reads in pokemon types.
		string pokemonTypes;

		getline(ins2, pokemonTypes);
		if (!ins2) break;

		stringstream ssPokemonTypes(pokemonTypes);
		string type1, type2;

		//These are here to get rid of the index and name of the pokemon so that we can actually get the type1 and potential type2.
		getline(ssPokemonTypes, temp.type1, '\t');
		getline(ssPokemonTypes, temp.type1, '\t');

		//Gets actual type1 and potential type2.
		getline(ssPokemonTypes, temp.type1, '\t');
		getline(ssPokemonTypes, temp.type2, '\n');

		//YOU: After vetting it (make sure you handle Mr. Mime properly) push it back into pokemon_db
		pokemon_db.push_back(temp);
	}
}

//This class holds a record for each move in the game
class Move {
  public:
	//YOU: Add member variable for each of these:
	//int index, string name, string type, string category, int PP, int power, int accuracy
	int index = 0;
	string name;
	string type;
	string category;
	int PP = 0;
	int power = 0;
	int accuracy = 0;

};
vector<Move> move_db;

//Reads all moves from moves.txt into move_db
//Be sure to discard all status moves
void load_move_db(string filename = "moves.txt") {
	ifstream ins(filename);
	if (!ins) die("Couldn't load "s + filename);
	while (ins) {
		//YOU: Read from the file, make a temp Move with all the data read in
		Move temp;

		string moveData;

		getline(ins, moveData);
		if (!ins) break;

		stringstream ssMoveData(moveData);
		string index, name, type, category, PP, power, accuracy;

		getline(ssMoveData, index, '\t');
		getline(ssMoveData, name, '\t');
		getline(ssMoveData, type, '\t');
		getline(ssMoveData, category, '\t');
		getline(ssMoveData, PP, '\t');
		getline(ssMoveData, power, '\t');
		getline(ssMoveData, accuracy, '\t');

		if (isdigit(index.at(0))) temp.index = stoi(index);
		temp.name = name;
		temp.type = type;
		if (category != "Status") temp.category = category;
		if (isdigit(PP.at(0))) temp.PP = stoi(PP);
		if (power != "—" and isdigit(power.at(0))) temp.power = stoi(power);
		if (accuracy != "—" and isdigit(accuracy.at(0))) temp.accuracy = stoi(accuracy);

		//YOU: Push it back into the move_db
		move_db.push_back(temp);
	}
}

//PART 2
//This conducts a one versus one battle between two pokemon of your choice
void battle_mode() {
	//User pokemon data.
	Pokemon userPokemon;
	Move userMove1, userMove2, userMove3, userMove4;
	vector<Move> userMoves;
	string userTemp, tempMove1, tempMove2, tempMove3, tempMove4;
	//Enemy pokemon data.
	Pokemon enemyPokemon;
	Move enemyMove1, enemyMove2, enemyMove3, enemyMove4;
	vector<Move> enemyMoves;
	string enemyTemp, tempEnemyMove1, tempEnemyMove2, tempEnemyMove3, tempEnemyMove4;

	//Gets user pokemon.
	//Prompt user as long as they haven't given valid input.
	while (!cin) {
		try {
			cout << "Choose a pokemon to use name or index: ";
			cin >> userTemp;
		} catch (...) {
			cout << "ERROR: Invalid name or index." << endl;
		}
	}
	//If they used an index, assign stats for their pokemon based off index.
	if (isdigit(userTemp.at(0))) {
		userTemp = stoi(userTemp);
		userPokemon = pokemon_db.find(userTemp);
	}
	//Else if they used a name, assign stats for their pokemon based off name.
	else userPokemon = pokemon_db.find(userTemp);

	//Gets moves for user's pokemon.
	cout << "Choose up to 4 moves for your pokemon name or index (press Q when done):" << endl;
	//Prompt user as long as they haven't given valid input.
	while (!cin) {
		try {
			cout << "Move #1: ";
			cin >> tempMove1;
			if (tempMove1 == "q" or tempMove1 == "Q") {
				tempMove1 = "Struggle";
				tempMove2 = "Struggle";
				tempMove3 = "Struggle";
				tempMove4 = "Struggle";
				break;
			}
			cout << "Move #2: ";
			cin >> tempMove2;
			if (tempMove2 == "q" or tempMove2 == "Q") {
				tempMove2 = "Struggle";
				tempMove3 = "Struggle";
				tempMove4 = "Struggle";
				break;
			}
			cout << "Move #3: ";
			cin >> tmpMove3;
			if (tempMove3 == "q" or tempMove3 == "Q") {
				tempMove3 = "Struggle";
				tempMove4 = "Struggle";
				break;
			}
			cout << "Move #4: ";
			cin >> tempMove4;
			if (tempMove4 == "q" or tempMove4 == "Q") {
				tempMove4 = "Struggle";
				break;
			}
		} catch (...) {
			cout << "ERROR: Invalid name or index." << endl;
		}
	}
	//If they used an index, assign stats based off index of move, otherwise assign stats based off name of name. (For all 4 moves.)
	//Move 1
	if (isdigit(tempMove1.at(0))) userMove1 = moves_db.find(stoi(tempMove1));
	else userMove1 = moves_db.find(tempMove1);
	userMoves.push_back(userMove1);
	//Move 2
	if (isdigit(tempMove2.at(0))) userMove2 = moves_db.find(stoi(tempMove2));
	else userMove2 = moves_db.find(tempMove2);
	userMoves.push_back(userMove2);
	//Move 3
	if (isdigit(tempMove3.at(0))) userMove3 = moves_db.find(stoi(tempMove3));
	else userMove3 = moves_db.find(tempMove3);
	userMoves.push_back(userMove3);
	//Move 4
	if (isdigit(tempMove4.at(0))) userMove4 = moves_db.find(stoi(tempMove)4);
	else userMove4 = moves_db.find(tempMove4);
	userMoves.push_back(userMove4);

	//Gets enemy pokemon.
	//Prompt user as long as they haven't given valid input.
	while (!cin) {
		try {
			cout << "Choose a pokemon to fight using name or index: ";
			cin >> enemyTemp;
		} catch (...) {
			cout << "ERROR: Invalid name or index." << endl;
		}
	}
	//If they used an index, assign stats for enemy pokemon based off index.
	if (isdigit(enemyTemp.at(0))) {
		enemyTemp = stoi(enemyTemp);
		enemyPokemon = pokemon_db.find(enemyTemp);
	}
	//Else if they used a name, assign stats for enemy pokemon based off name.
	else enemyPokemon = pokemon_db.find(enemyTemp);

	//Gets moves for enemy's pokemon.
	cout << "Choose up to 4 moves for the enemy pokemon using name or index (press Q when done):" << endl;
	//Prompt user as long as they haven't given valid input.
	while (!cin) {
		try {
			cout << "Move #1: ";
			cin >> tempEnemyMove1;
			if (tempEnemyMove1 == "q" or tempEnemyMove1 == "Q") {
				tempEnemyMove1 = "Struggle";
				tempEnemyMove2 = "Struggle";
				tempEnemyMove3 = "Struggle";
				tempEnemyMove4 = "Struggle";
				break;
			}
			cout << "Move #2: ";
			cin >> tempEnemyMove2;
			if (tempEnemyMove2 == "q" or tempEnemyMove2 == "Q") {
				tempEnemyMove2 = "Struggle";
				tempEnemyMove3 = "Struggle";
				tempEnemyMove4 = "Struggle";
				break;
			}
			cout << "Move #3: ";
			cin >> tempEnemyMove3;
			if (tempEnemyMove3 == "q" or tempEnemyMove3 == "Q") {
				tempEnemyMove3 = "Struggle";
				tempEnemyMove4 = "Struggle";
				break;
			}
			cout << "Move #4: ";
			cin >> tempEnemyMove4;
			if (tempEnemyMove4 == "q" or tempEnemyMove4 == "Q") {
				tempEnemyMove4 = "Struggle";
				break;
			}
		} catch (...) {
			cout << "ERROR: Invalid name or index." << endl;
		}
	}
	//If they used an index, assign stats based off index of move, otherwise assign stats based off name of name. (For all 4 moves.)
	//Move 1
	if (isdigit(tempEnemyMove1.at(0))) enemyMove1 = moves_db.find(stoi(tempEnemyMove1));
	else enemyMove1 = moves_db.find(tempEnemyMove1);
	enemyMoves.push_back(enemyMove1);
	//Move 2
	if (isdigit(tempEnemyMove2.at(0))) enemyMove2 = moves_db.find(stoi(tempEnemyMove2));
	else enemyMove2 = moves_db.find(tempEnemyMove2);
	enemyMoves.push_back(enemyMove2);
	//Move 3
	if (isdigit(tempEnemyMove3.at(0))) enemyMove3 = moves_db.find(stoi(tempEnemyMove3));
	else enemyMove3 = moves_db.find(tempEnemyMove3);
	enemyMoves.push_back(enemyMove3);
	//Move 4
	if (isdigit(tempEnemyMove4.at(0))) enemyMove4 = moves_db.find(stoi(tempEnemyMove4));
	else enemyMove4 = moves_db.find(tempEnemyMove4);
	enemyMoves.push_back(enemyMove4);

	map<string, int> m;
	m["Normal"] = 0;
	m["Fighting"] = 1;
	m["Flying"] = 2;
	m["Poison"] = 3;
	m["Ground"] = 4;
	m["Rock"] = 5;
	m["Bug"] = 6;
	m["Ghost"] = 7;
	m["Steel"] = 8;
	m["Fire"] = 9;
	m["Water"] = 10;
	m["Grass"] = 11;
	m["Electric"] = 12;
	m["Psychic"] = 13;
	m["Ice"] = 14;
	m["Dragon"] = 15;
	m["Dark"] = 16;
	m["Fairy"] = 17;

	//Lets get to fighting these pokemon like they're rabbid dogs!

	//If userPokemon's speed and enemyPokemon's speed are the same, roll a dice.
	int diceRoll = -1;
	if (userPokemon.speed == enemyPokemon.speed) {
		diceRoll = rand() % 2;
	}

	//If user's pokemon's speed is higher or the dice rolled a 1, they go first.
	if (userPokemon.speed > enemyPokemon.speed or diceRoll == 1) {
		string userChoice;
		int damageDealt = 0;
		double stab = 0, typeMod = 0;
		while (userPokemon.hp > 0 or enemyPokemon.hp > 0) {
			//Give user choice of what move to attack with.
			cout << "Choose a move for you to use: " << endl << userMove1 << endl << userMove2 << endl << userMove3 << endl << userMove4 << endl;
			cin >> userChoice;

			Pokemon userPower.power = 0;
			if (userMoves.find(userChoice)) userPower.power = userMoves.find(userChoice);
			//TODO: STAB AND TYPE MODIFIER
			//if (userPokemon.type == userMoves.at(something.type)) stab = 1.5;
			//else stab = 1.0;
			//if (something something type_modifier) ;
			
			//Calculate damage dealt then substract that from enemy pokemon's health.
			damageDealt = (userPower * userPokemon.attack) / (enemyPokemon.defense * /*STAB*/ * /*type_modifier*/);
			enemyPokemon.hp -= damageDealt;

			cout << "You dealt " << damageDealt << " , resulting in " << enemyPokemon.name << "'s HP being " << enemyPokemon.hp << endl << endl;

			//Give user choice of what move to attack with.
			cout << "Choose a move for the enemy to use: " << endl << enemyMove1 << endl << enemyMove2 << endl << enemyMove3 << endl << enemyMove4 << endl;
			cin >> userChoice;

			Pokemon enemyPower.power = 0;
			if (enemyMoves.find(userChoice)) enemyPower.power = enemyMoves.find(userChoice);
			//TODO: STAB AND TYPE MODIFIER
			//if (enemyPokemon.type == userMoves.at(something.type)) stab = 1.5;
			//else stab = 1.0;
			//if (something something type_modifier) ;
			
			//Calculate damage dealt then substract that from user pokemon's health.
			damageDealt = (enemyPower * enemyPokemon.attack) / (userPokemon.defense * /*STAB*/ * /*type_modifier*/);
			userPokemon.hp -= damageDealt;

			cout << "The enemy dealt " << damageDealt << " , resulting in " << userPokemon.name << "'s HP being " << userPokemon.hp << endl; << endl;
		}
	}
	//Else if the enemyPokemon speed is higher or the dice rolled a 0, they go first.
	else if (enemyPokemon.speed > userPokemon.speed or diceRoll == 0) {
		while (userPokemon.hp > 0 or enemyPokemon.hp > 0) {
			//TODO: Copy and paste above once finished, and switch order so that enemy pokemon attacks first.
		}
	}


	/*	cout << "Do you want to look up a pokemon? (y for yes)" << endl;
		cin >> userInput;
		while (userInput == "y") {
			//This is how you look up which row/col the type is
			cout << "Please enter a type:\n";
			string s;
			cin >> s;
			cout << "Its row/col is: " << m[s] << endl;
			cout << "Do you want to look up a pokemon? (y for yes)" << endl;
			cin >> userInput;
		} */



	//Pick a Pokemon and a move for team one
	//Pick a Pokemon and a move for team two
	//Whichever Pokemon has the higher speed goes first
	//Have them do damage to each other based on their move * power * type modifier
	//Target Pokémon reduces damage based on its defense or special defense
}

//PART 2
void explore_mode() {
}

int main() {
	srand(0); //Fixed seed - don't change this
	system("figlet POKEMON");
	system("figlet ++ and \\#");
	cout << "Do you want to use the Gen1 Pokémon? (Type \"NO\" for no, anything else for yes.)\n";
	string answer;
	getline(cin, answer);
	for (char &c : answer) c = toupper(c); //Uppercaseify
	string filename1, filename2;
	if (answer == "NO") {
		cout << "Please enter the two files containing the Pokémon and types for the Pokémon.\n";
		cin >> filename1 >> filename2;
		load_pokemon_db(filename1, filename2);
	} else
		load_pokemon_db();

	/*
	// Debug the load - uncomment this to see everything
	for (Pokemon p : pokemon_db) {
		cerr << "Pokedex Entry " << p.index << ": " << p.name << " hp: " << p.hp << " attack: " << p.attack;
		cerr << " defense: " << p.defense << " speed: " << p.speed << " special: " << p.special;
		cerr << " type1: " << p.type1 << " type2: " << p.type2 << endl;
	}
	*/


	string filename3;
	if (answer == "NO") {
		cout << "Please enter the file containing the moves for your Pokémon.\n";
		cin >> filename3;
		load_move_db(filename3);
	} else
		load_move_db();

	/*
	// DEBUG the load - uncomment this to see the results
	for (Move m : move_db) {
		if (m.index == 0) continue;
		cerr << "Move Index " << m.index << ": " << m.name << " " << " type: " << m.type << " category: " << m.category << " PP: " << m.PP << " power: " << m.power << " accuracy: " << m.accuracy << "%\n";
	}
	*/


	cout << "Do you want to\n1) Print Pokémon Data?\n2) Print Move Data?\n3) Pokemon Battle (1v1)\n4) Explore the World?\n";
	int choice = 0;
	cin >> choice;
	if (!cin || choice < 1 || choice > 4) die();
	if (choice == 1) {
		cout << "Please enter the Pokedex number of the Pokémon whose data you want to print:\n";
		int index = 0;
		cin >> index;
		//NOTE: DSD ADDED or index <= 0 TO PASS TEST #4
		if (!cin or index <= 0) die();
		Pokemon temp;
		temp.index = -1;
		for (const Pokemon &poke : pokemon_db) if (index == poke.index) temp.index = poke.index;
		if (temp.index == -1) die();
		//  Pokemon p = pokemon_db.at(index + 1); //Pokedex starts at 1, this only works for Gen1 Pokémon
		Pokemon p;
		for (const Pokemon &poke : pokemon_db) if (index == poke.index) p = poke;
		cout << "Pokedex Entry " << p.index << ": " << p.name << " hp: " << p.hp << " attack: " << p.attack;
		cout << " defense: " << p.defense << " speed: " << p.speed << " special: " << p.special;
		cout << " type1: " << p.type1 << " type2: " << p.type2 << endl;
	} else if (choice == 2) {
		cout << "Please enter the move number of the move whose data you want to print:\n";
		int index = 0;
		cin >> index;
		if (!cin) die();
		Move m;
		for (const Move &mo : move_db) if (index == mo.index) m = mo;
		if (m.name == "Error") cout << "No move loaded with that index.\n";
		else
			cout << "Move Index " << m.index << ": " << m.name << " " << " type: " << m.type << " category: " << m.category << " PP: " << m.PP << " power: " << m.power << " accuracy: " << m.accuracy << "%\n";
	}
	if (choice == 3) battle_mode(); //PART 2
	else explore_mode(); //PART 2
}

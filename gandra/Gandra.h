#include "Card.h"
#include "Player.h"
#include "Server.h"

//main functions

void gandrac_same_screen();
void gandrac_network();

//Defines the order by which the players play
void ntwk_plyr(SOCKET, unsigned int);

//Ask if a player want to retry game
char ask_retry();

//display menus

void display_glbl_menu();
void display_ntwk_menu();

unsigned int ask_number_of_players(unsigned int, unsigned int);
void refuse_input(const char*);

vector<Player*> create_players(unsigned int);
vector<Card*> create_deck(unsigned int);

Card::Suit get_privileged_suit(const unsigned int, const vector<Card*>&);

void deal_cards(const vector<Card*>&, const vector<Player*>&);
void deal_cards(const vector<Card*>&, unsigned int&, const vector<Player*>, const unsigned int);

void display_cards_left(unsigned int, unsigned int);

void display_card(const Card*, const Card::Suit, unsigned int, unsigned int);
void display_cards(const vector<Card*>&, const Card::Suit); 
void display_cards(const vector<Card*>&, const Card::Suit, unsigned int, unsigned int);
char* display_upcard(const unsigned int, const vector<Card*>&);

void display_round(const unsigned int);
void display_player_turn(const unsigned int);

void display_hand(const vector<Card*>&);
void display_winner_card(const Card*);
void display_round_winner(const Card*);

unsigned int ask_choice(unsigned int);
Card* compare_cards(Card*, Card*, const Card::Suit);
Card* round_winner_card(vector<Card*>, const Card::Suit);

vector<unsigned int> get_scores(const vector<Player*>, const vector<Card*>);
void display_final_scores(const vector<unsigned int>);
bool is_draw(const vector<unsigned int>);
unsigned int game_winner(const vector<unsigned int>);
void display_game_winner(const vector<unsigned int>);

//Functions used for testing

void display_deck(const vector<Card*>&);
void displaying_hands(const vector<Player*>&);
vector<unsigned int> compute_scores(vector<Card*>, unsigned int);

//Data Transmission functions

void send_card(const SOCKET&, const char*);
void send_hand_size(const SOCKET&, unsigned int);
unsigned int recieve_hand_size(const SOCKET&);
void send_cards(const SOCKET&, const vector<Card*>&);
void send_cards(const SOCKET&, const vector<Card*>&, const Card::Suit&);
void display_cards(const SOCKET&, unsigned int, unsigned int);
void send_round_results(const SOCKET&, const Card*);
void display_round_results(const SOCKET&);
void send_choice(const SOCKET&, unsigned int);
unsigned int recieve_choice(const SOCKET&);
void send_eldest(const SOCKET&, unsigned int);
unsigned int recieve_eldest(const SOCKET&);
void send_final_scores(const SOCKET&, const vector<unsigned int>&);
void display_final_scores(const SOCKET&);

void display_upcard(const SOCKET&);
void display_hand(const SOCKET&);
#include "Card.h"
#include "Player.h"
#include "Server.h"

//main functions

void gandrac_same_screen();
void gandrac_network();

//display the menum

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

void display_card(Card*, Card::Suit, unsigned int, unsigned int);
void display_cards(const vector<Card*>&, Card::Suit); 
void display_cards(const vector<Card*>&, Card::Suit, unsigned int, unsigned int);
char* display_upcard(const unsigned int, const vector<Card*>&);

void display_round(const unsigned int);
void display_player_turn(const unsigned int);

void display_hand(const vector<Card*>&);
void display_winner_card(const Card*);
void display_round_winner(Card*);

unsigned int ask_choice(unsigned int);
Card* compare_cards(const Card*, const Card*, Card::Suit);
Card* round_winner_card(vector<Card*>, Card::Suit);

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

void send_card(SOCKET, char*);
void send_hand_size(SOCKET, unsigned int);
unsigned int recieve_hand_size(SOCKET);
void send_cards(SOCKET, const vector<Card*>&);
void send_cards(SOCKET, const vector<Card*>&, Card::Suit);
void display_cards(SOCKET, unsigned int, unsigned int);
void send_round_results(SOCKET, Card*);
void display_round_results(SOCKET);
void send_choice(SOCKET, unsigned int);
unsigned int recieve_choice(SOCKET);
void send_eldest(SOCKET, unsigned int);
unsigned int recieve_eldest(SOCKET);
void send_final_scores(SOCKET, const vector<unsigned int>);
void display_final_scores(SOCKET);

void display_upcard(SOCKET);
void display_hand(SOCKET);
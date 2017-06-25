#include "Card.h"
#include "Player.h"

unsigned int ask_number_of_players(unsigned int, unsigned int);
void refuse_input(const char*);

vector<Card*> create_deck(unsigned int);
vector<Player*> create_players(unsigned int);

Card::Suit get_privileged_suit(const unsigned int, const vector<Card*>&);

void deal_cards(const vector<Card*>&, const vector<Player*>&);
void deal_cards(const vector<Card*>&, unsigned int&, const vector<Player*>, const unsigned int);

//void display_cards(const vector<Card*>&); //Not Used
void display_cards(const vector<Card*>&, Card::Suit); 
void display_upcard(const unsigned int, const vector<Card*>&);
void display_round(const unsigned int);
void display_player_turn(const unsigned int);
//void display_hand(const unsigned int, const vector<Player*>&); //Not Used
void display_hand(const vector<Card*>&);
void display_winner_card(const Card*);
void display_round_winner(Card*);

unsigned int ask_choice(unsigned int);
Card* compare_cards(const Card*, const Card*, Card::Suit);
Card* round_winner_card(vector<Card*>, Card::Suit);

Player* compare_players(const Player*, const Player*, const vector<Card*>);
Player* winner(const vector<Player*>, const vector<Card*>);

void display_final_scores(const vector<Player*>, const vector<Card*>);
void display_game_winner(const vector<Player*>, const vector<Card*>);

//Functions used for testing
void display_deck(const vector<Card*>&);
void displaying_hands(const vector<Player*>&);
vector<unsigned int> compute_scores(vector<Card*>, unsigned int);
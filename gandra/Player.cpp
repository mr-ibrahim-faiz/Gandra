#include "Player.h"
#include "Card.h"
#include "Errors.h"

unsigned int Player::next_player {1};

// simple constructor
Player::Player() {
	player_number = next_player++;
}


// free memory block that the pointers stored in vector are pointing to
template<typename T>
void cleanup(vector<T*>& vec)
// the stored pointers should point to a block of memory allocated
{
	for (T* t : vec)
		if (t != nullptr)
			delete t;
}

// destructor
Player::~Player() {
	cleanup(hand);
}

unsigned int Player::get_player_number() const {
	return player_number;
}

unsigned int Player::get_score(vector<Card*> deck) const {
	unsigned int score {0};
	for (unsigned int i = 0; i < deck.size() - 1; ++i) {
		if (deck[i]->player_number == player_number)
			score += (unsigned int) deck[i]->value;
	}
	return score;
}

vector<Card*>& Player::get_hand() {
	return hand;
}

Card* Player::play_card(const unsigned int hand_index) {
	if (hand_index < 0 || hand_index >= hand.size())
		error("Unable to play card: Out Of Range");

	Card* card = hand[hand_index];
	card->is_in_hand = false;
	card->is_played = true;
	update_hand();
	return card;
}

void Player::claim_cards(Card* card) {
	if (card->is_in_hand == false && card->is_played == true)
		card->player_number = player_number;
}

void Player::update_hand() {
	vector<Card*> updated_hand;
	for (Card* card : hand)
		if (card->get_status() == true)
			updated_hand.push_back(card);
	hand = updated_hand;
}
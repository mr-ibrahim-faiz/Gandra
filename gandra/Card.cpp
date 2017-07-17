#include "Card.h"
#include "Player.h"
#include "Errors.h"

Card::Card(Suit suit, Rank rank) {
	this->suit = suit;
	this->rank = rank;
	set_name();
	set_value();
	is_in_hand = false;
	is_played = false;
	player_number = 0;
}

Card::Rank Card::get_rank() const {
	return rank;
}

Card::Suit Card::get_suit() const {
	return suit;
}

char* Card::get_name()const {
	return name;
}

double Card::get_value() const {
	return value;
}

bool Card::get_status() const {
	return is_in_hand;
}

unsigned int Card::get_player_number() const {
	return player_number;
}

void Card::add_card(Player* player) {
	if (player == nullptr)
		error("Unable to add card: Nullpointer");
	is_in_hand = true;
	player_number = player->player_number;
	player->hand.push_back(this);
}

vector<Card*> Card::create_deck() {
	//Reinitialize player numbers (useful for replay)
	Player::next_player = 1; 

	vector<Card*> deck;
	for(int i = 0; i < 4; ++i)
		for (int j = 0; j < 9; ++j) {
			Card* card = new Card(static_cast<Suit>(i), static_cast<Rank>(j));
			deck.push_back(card);
		}
	return deck;
}

void Card::set_name() {
	switch (rank) {
	case Ace:
		switch (suit) {
		case Clubs:
			name = "Ace of Clubs";
			break;
		case Diamonds:
			name = "Ace of Diamonds";
			break;
		case Hearts:
			name = "Ace of Hearts";
			break;
		case Spades:
			name = "Ace of Spades";
			break;
		}
		break;
	case King:
		switch (suit) {
		case Clubs:
			name = "King of Clubs";
			break;
		case Diamonds:
			name = "King of Diamonds";
			break;
		case Hearts:
			name = "King of Hearts";
			break;
		case Spades:
			name = "King of Spades";
			break;
		}
		break;
	case Queen:
		switch (suit) {
		case Clubs:
			name = "Queen of Clubs";
			break;
		case Diamonds:
			name = "Queen of Diamonds";
			break;
		case Hearts:
			name = "Queen of Hearts";
			break;
		case Spades:
			name = "Queen of Spades";
			break;
		}
		break;
	case Jack:
		switch (suit) {
		case Clubs:
			name = "Jack of Clubs";
			break;
		case Diamonds:
			name = "Jack of Diamonds";
			break;
		case Hearts:
			name = "Jack of Hearts";
			break;
		case Spades:
			name = "Jack of Spades";
			break;
		}
		break;
	case Seven:
		switch (suit) {
		case Clubs:
			name = "Seven of Clubs";
			break;
		case Diamonds:
			name = "Seven of Diamonds";
			break;
		case Hearts:
			name = "Seven of Hearts";
			break;
		case Spades:
			name = "Seven of Spades";
			break;
		}
		break;
	case Three:
		switch (suit) {
		case Clubs:
			name = "Three of Clubs";
			break;
		case Diamonds:
			name = "Three of Diamonds";
			break;
		case Hearts:
			name = "Three of Hearts";
			break;
		case Spades:
			name = "Three of Spades";
			break;
		}
		break;
	case Four:
		switch (suit) {
		case Clubs:
			name = "Four of Clubs";
			break;
		case Diamonds:
			name = "Four of Diamonds";
			break;
		case Hearts:
			name = "Four of Hearts";
			break;
		case Spades:
			name = "Four of Spades";
			break;
		}
		break;
	case Five:
		switch (suit) {
		case Clubs:
			name = "Five of Clubs";
			break;
		case Diamonds:
			name = "Five of Diamonds";
			break;
		case Hearts:
			name = "Five of Hearts";
			break;
		case Spades:
			name = "Five of Spades";
			break;
		}
		break;
	case Six:
		switch (suit) {
		case Clubs:
			name = "Six of Clubs";
			break;
		case Diamonds:
			name = "Six of Diamonds";
			break;
		case Hearts:
			name = "Six of Hearts";
			break;
		case Spades:
			name = "Six of Spades";
			break;
		}
		break;
	}
}

void Card::set_value() {
	switch (rank) {
	case Ace:
		value = 11.0;
		break;
	case King:
		value = 4.0;
		break;
	case Queen:
		value = 2.0;
		break;
	case Jack:
		value = 3.0;
		break;
	case Seven:
		value = 10.0;
		break;
	case Three:
		value = 0.3;
		break;
	case Four:
		value = 0.4;
		break;
	case Five:
		value = 0.5;
		break;
	case Six:
		value = 0.6;
		break;
	}
}

ostream& operator<<(ostream& os, const Card& card) {
	os << card.name;
	return os;
}
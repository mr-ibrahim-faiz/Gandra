#include "Card.h"
#include "Player.h"
#include "Errors.h"

#define NAMELEN 24

Card::Card(Suit suit, Rank rank)
	: suit(suit), rank(rank), name(nullptr)
	, is_in_hand(false), is_played(false)
	, player_number(false)
{
	set_name();
	set_value();
}

Card::~Card(){
	if (name != nullptr)
		delete[] name;
}

Card::Card(Card&& other) {
	*this = move(other);
}

Card& Card::operator=(Card&& other) {
	if (this != &other) {
		suit = move(other.suit);
		rank = move(other.rank);
		name = move(other.name);
		value = move(other.value);
		is_in_hand = move(other.is_in_hand);
		is_played = move(other.is_played);
		player_number = move(other.player_number);

		other.name = nullptr;
	}
	return *this;
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
			name = new char[NAMELEN] { "Ace of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Ace of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Ace of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Ace of Spades" };
			break;
		}
		break;
	case King:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "King of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "King of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "King of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "King of Spades" };
			break;
		}
		break;
	case Queen:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "Queen of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Queen of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Queen of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Queen of Spades" };
			break;
		}
		break;
	case Jack:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "Jack of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Jack of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Jack of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Jack of Spades" };
			break;
		}
		break;
	case Seven:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "Seven of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Seven of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Seven of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Seven of Spades" };
			break;
		}
		break;
	case Three:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "Three of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Three of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Three of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Three of Spades" };
			break;
		}
		break;
	case Four:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "Four of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Four of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Four of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Four of Spades" };
			break;
		}
		break;
	case Five:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "Five of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Five of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Five of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Five of Spades" };
			break;
		}
		break;
	case Six:
		switch (suit) {
		case Clubs:
			name = new char[NAMELEN] { "Six of Clubs" };
			break;
		case Diamonds:
			name = new char[NAMELEN] { "Six of Diamonds" };
			break;
		case Hearts:
			name = new char[NAMELEN] { "Six of Hearts" };
			break;
		case Spades:
			name = new char[NAMELEN] { "Six of Spades" };
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
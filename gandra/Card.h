#pragma once

#include<vector>
using std::vector;

#include<iostream>
using std::ostream;

class Player;

class Card {
public:
	enum Suit {
		Clubs, Diamonds, Hearts, Spades
	};

	enum Rank {
		Ace, King, Queen, Jack, Three, Four, Five, Six, Seven
	};

	Card(Suit, Rank);

	Rank get_rank();
	Suit get_suit();
	char* get_name();
	double get_value();
	bool get_status();
	unsigned int get_player_number();

	void add_card(Player*);

	static vector<Card*> create_deck();
private:
	Suit suit;
	Rank rank;
	char* name;
	double value;
	bool is_in_hand;
	bool is_played;
	
	unsigned int player_number;

	void set_name();
	void set_value();

	friend ostream& operator<<(ostream&, const Card&);
	friend class Player;
};
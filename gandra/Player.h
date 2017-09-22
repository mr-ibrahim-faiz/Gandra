#pragma once

#include<vector>
using std::vector;

class Card;

class Player {
public:
	Player();
	~Player();
	unsigned int get_player_number() const;
	unsigned int get_score(vector<Card*>) const;
	vector<Card*>& get_hand();
	Card* play_card(const unsigned int);
	void claim_cards(Card*);
private:
	unsigned int player_number;
	vector<Card*> hand;

	void update_hand();
	static unsigned int next_player;
	friend class Card;
};
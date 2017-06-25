#include "Gandra.h"

#include<iostream>
using std::cout;
using std::cin;
using std::endl;
using std::streamsize;

#include<limits>
using std::numeric_limits;

#include<ctime>
using std::time;

#include<algorithm>
using std::random_shuffle;

#include "Errors.h"

unsigned int ask_number_of_players(unsigned int min, unsigned int max) {
	unsigned int number_of_players {0};
	cout << "Please enter a number of players (2-4):" << endl;
	cin >> number_of_players;

	while (!cin || (number_of_players<min) || (number_of_players>max)) {
		refuse_input("Please enter a valid number !");
		cin >> number_of_players;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return number_of_players;
}

void refuse_input(const char* message) {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << message << endl;
}

vector<Card*> create_deck(unsigned int number_of_players) {
	vector<Card*> deck = Card::create_deck();

	srand((unsigned int)time(NULL));
	random_shuffle(deck.begin(), deck.end());
	
	unsigned int upcard_index = 3 * number_of_players;
	if (upcard_index < 0 || upcard_index >= deck.size())
		error("Unable to get upcard index: Out Of Range");

	//Put upcard at the end of the deck
	Card* card = deck[upcard_index];
	deck.push_back(card);
	
	return deck;
}

vector<Player*> create_players(unsigned int number_of_players) {
	vector<Player*> players;
	for (unsigned int i = 0; i < number_of_players; ++i) {
		Player* player = new Player();
		players.push_back(player);
	}
	return players;
}

Card::Suit get_privileged_suit(const unsigned int number_of_players, const vector<Card*>& deck) {
	unsigned int upcard_index = 3 * number_of_players;
	if (upcard_index < 0 || upcard_index >= deck.size())
		error("Unable to get upcard index: Out Of Range");
	return deck[upcard_index]->get_suit();
}

void deal_cards(const vector<Card*>& deck, const vector<Player*>& players) {
	unsigned int next_card {0};
	for (unsigned int i = 0; i < players.size(); ++i) {
		while (next_card < 3 * (i + 1)) {
			if (next_card < 0 || next_card >= deck.size())
				error("Unable to deal card: Out Of Range");
			deck[next_card++]->add_card(players[i]);
		}		
	}
}

void deal_cards(const vector<Card*>& deck, unsigned int& deck_index, const vector<Player*> players, const unsigned int player_number) {
	if (deck_index < deck.size()) {
		unsigned int player_index = player_number - 1;
		for (unsigned int i = 0; i < players.size(); ++i) {
			if (player_index == players.size())
				player_index = 0;
			if (deck_index < 0 || deck_index >= deck.size())
				error("Unable to deal card: Out Of Range");
			deck[deck_index++]->add_card(players[player_index++]);
		}
	}
}

//void display_cards(const vector<Card*>& cards) {
//	cout << endl;
//	for (Card* card : cards)
//		cout << *card << endl;
//} //Not Used

void display_cards(const vector<Card*>& cards, Card::Suit suit) {
	cout << endl;
	for (Card* card : cards)
		cout << *card << ((card->get_suit() == suit)? "*" : "") << endl;
}


void display_upcard(const unsigned int number_of_players, const vector<Card*>& deck) {
	unsigned int deck_index = 3 * number_of_players;
	if (deck_index < 0 || deck_index >= deck.size())
		error("Unable to display upcard: Out Of Range");
	cout << "\nUpcard: " << *deck[deck_index] << " !" << endl;
}

void display_round(const unsigned int round) {
	cout << "\nRound " << round << endl;
}

void display_player_turn(const unsigned int player_number) {
	cout << "\nPlayer " << player_number << "\'s turn: " << endl;
}

//void display_hand(const unsigned int player_number, const vector<Player*>& players) {
//	vector<Card*>& hand = players[player_number-1]->get_hand();
//	for (unsigned int i = 0; i < hand.size(); ++i)
//		//cout << i + 1 << ": " << *hand[i] << endl;
//		cout << i + 1 << ": " << *hand[i] << "," << hand[i]->get_status() << endl; //For Test
//} //Not Used

void display_hand(const vector<Card*>& hand) {
	for (unsigned int i = 0; i < hand.size(); ++i)
		cout << i + 1 << ": " << *hand[i] << endl;
}

void display_winner_card(const Card* card) {
	cout << "\n" << *card << " beats the other card(s)." << endl;
}

void display_round_winner(Card* card) {
	cout << "Player " << card->get_player_number() << " wins round !" << endl;
}

unsigned int ask_choice(unsigned int hand_size) {
	unsigned int choice {0};
	cin >> choice;
	while (!cin || (choice < 1) || (choice > hand_size)) {
		refuse_input("Please enter a valid choice !");
		cin >> choice;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return choice;
}

Card* compare_cards(Card* card_a, Card* card_b, Card::Suit privileged_suit) {
	if (card_a->get_suit() == privileged_suit) {
		if (card_a->get_suit() != card_b->get_suit()) return card_a;
		else {
			if (card_a->get_value() > card_b->get_value()) return card_a;
		}
	}
	else {
		if (card_a->get_suit() == card_b->get_suit()) {
			if (card_a->get_value() > card_b->get_value()) return card_a;
		}
		else {
			if (!(card_b->get_suit() == privileged_suit)) return card_a;
		}
	}
	return card_b;
}

Card* round_winner_card(vector<Card*> round_cards, Card::Suit privileged_suit) {
	unsigned int i {0};
	Card* winner_card;

	while (i < round_cards.size()) {
		if (i == 0)
			winner_card = round_cards[i++];
		else
			winner_card = compare_cards(winner_card, round_cards[i++], privileged_suit);
	}
	return winner_card;
}

Player* compare_players(Player* player_a, Player* player_b, const vector<Card*> deck) {
	if (player_a->get_score(deck) > player_b->get_score(deck))
		return player_a;
	return player_b;
}

Player* winner(const vector<Player*> players, const vector<Card*> deck) {
	Player* game_winner = players[0];
	for (Player* player : players)
		game_winner = compare_players(game_winner, player, deck);
	return game_winner;
}

void display_final_scores(const vector<Player*> players, const vector<Card*> deck) {
	cout << "\nPlayer's final scores: " << endl;
	for (unsigned int i = 0; i < players.size(); ++i) {
		cout << i + 1 << ": " << players[i]->get_score(deck) << endl;
	}
}

void display_game_winner(const vector<Player*> players, const vector<Card*> deck) {
	Player* game_winner = winner(players, deck);
	cout << "\nPlayer " << game_winner->get_player_number() << " wins game !" << endl;
}

//Functions used for testing
void display_deck(const vector<Card*>& deck) {
	for (unsigned int i = 0; i < deck.size(); ++i)
		cout << i << ": " << *deck[i] << "," << deck[i]->get_player_number() << endl;
}

void displaying_hands(const vector<Player*>& players) {
	for (Player* player : players) {
		vector<Card*> hand = player->get_hand();
		for (Card* card : hand)
			cout << *card << "," << card->get_status() << endl;
		cout << endl;
	}
}

vector<unsigned int> compute_scores(vector<Card*> deck, unsigned int number_of_players) {
	vector<unsigned int> scores(number_of_players);
	for (unsigned int i = 0; i < deck.size() - 1; ++i) {
		scores[deck[i]->get_player_number() - 1] += (unsigned int)deck[i]->get_value();
	}
	return scores;
}
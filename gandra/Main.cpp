#include "Gandra.h"

#include<iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "Errors.h"

int main()
try {
	//Get number of players
	unsigned int number_of_players = ask_number_of_players(2, 4);

	//Create players
	vector<Player*> players = create_players(number_of_players);

	//Create a shuffle deck of cards
	vector<Card*> deck = create_deck(number_of_players);

	//Get privileged suit
	Card::Suit privileged_suit = get_privileged_suit(number_of_players, deck);

	//Deal cards to players
	deal_cards(deck, players);

	//Display visible card
	display_upcard(number_of_players, deck);

	unsigned int round {1};
	unsigned int eldest {1};
	unsigned int next_card {3 * number_of_players + 1};
	bool is_finished {false};

	while (!is_finished) {
		display_round(round++);
		vector<Card*> round_cards;

		for (unsigned int i = 0; i < number_of_players; ++i) {
			display_player_turn(eldest);

			getchar();
			Player* player = players[eldest++ - 1];
			vector<Card*>& hand = player->get_hand();
			display_hand(hand);

			if (hand.size() > 0) {
				unsigned int choice = ask_choice(hand.size());
				Card* played_card = player->play_card(choice - 1);
				round_cards.push_back(played_card);
				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl; //Delete This After Test
				display_cards(round_cards, privileged_suit);
			}

			if (eldest > number_of_players)
				eldest = 1;

			if (i == number_of_players - 1 && hand.size() == 0)
				is_finished = true;
		}

		Card* winner_card = round_winner_card(round_cards, privileged_suit);
		display_winner_card(winner_card);
		display_round_winner(winner_card);

		unsigned int winner_number = winner_card->get_player_number();
		Player* round_winner = players[winner_number - 1];

		for (Card* card : round_cards)
			round_winner->claim_cards(card);

		//Identify the eldest player for the next round
		eldest = winner_card->get_player_number();
		deal_cards(deck, next_card, players, eldest);

	}

	display_final_scores(players, deck);
	display_game_winner(players, deck);

	return 0;
}
catch (runtime_error &e) {
	cerr << "Error: " << e.what() << endl;
}
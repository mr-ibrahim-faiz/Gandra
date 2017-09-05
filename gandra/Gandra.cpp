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

#include<string>
using std::string;

#include "Errors.h"

void gandrac_same_screen() {
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
	unsigned int next_card { 3 * number_of_players + 1 };
	bool is_finished {false};

	while (!is_finished) {
		display_round(round++);

		//display the number of cards left in deck
		display_cards_left(round - 1, number_of_players);

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
		deal_cards(deck, next_card, players, eldest); //Comment this to ease testing

	}

	vector<unsigned int> scores = get_scores(players, deck);
	display_final_scores(scores);
	display_game_winner(scores);
}

void gandrac_network() {
	char choice;

	display_ntwk_menu();

	while (cin >> choice) {
		char next = cin.peek();

		if (next != '\n') {
			choice = '?';
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}

		switch (choice) {
		case '1':
		{
			cout << "\nWaiting for a friend..." << endl;
			SOCKET ClientSocket = server();
			
			char retry {'y'};
			while (retry == 'y') {
				srand((unsigned int)time(NULL));
				unsigned int player_number = rand() % 2 + 1;

				//send eldest
				send_eldest(ClientSocket, player_number);

				ntwk_plyr(ClientSocket, player_number);

				retry = ask_retry();
			}

			shutdown(ClientSocket, SD_BOTH); //REVIEW THIS (Do I want to close the connection here?)
		}
		break;

		case '2':
		{
			string ip_address {""};
			cout << "\nPlease enter an IP address: " << endl;
			cin >> ip_address;

			SOCKET ConnectSocket = client(ip_address);

			char retry {'y'};
			while (retry == 'y') {
				//recieve eldest
				unsigned int player_number = recieve_eldest(ConnectSocket);

				if (player_number == 1)
					ntwk_plyr(ConnectSocket, 2);
				else
					ntwk_plyr(ConnectSocket, 1);

				retry = ask_retry();
			}
		}
		break;

		case 'x':
			return;
			break;

		default:
			cout << "\nPlease enter a valid choice." << endl;
			break;
		}

		cout << endl;
		display_ntwk_menu();
	}
	
}

void ntwk_plyr(SOCKET socket, unsigned int player_number) {

	switch (player_number) {
	case 1:
	{

		//Fix number of players
		unsigned int number_of_players = 2;

		//Create players
		vector<Player*> players = create_players(number_of_players);

		//Create a shuffle deck of cards
		vector<Card*> deck = create_deck(number_of_players);

		//Get privileged suit
		Card::Suit privileged_suit = get_privileged_suit(number_of_players, deck);

		//Deal cards to players
		deal_cards(deck, players);

		//Display visible card
		char* upcard = display_upcard(number_of_players, deck);
		send_card(socket, upcard);

		unsigned int round {1};
		unsigned int eldest {1};
		unsigned int next_card { 3 * number_of_players + 1 };
		bool is_finished {false};

		while (!is_finished) {
			display_round(round++);

			//display the number of cards left in deck
			display_cards_left(round - 1, number_of_players);

			vector<Card*> round_cards;

			for (unsigned int i = 0; i < number_of_players; ++i) {

				Player* player = players[eldest++ - 1];
				unsigned int player_number = player->get_player_number();
				vector<Card*>& hand = player->get_hand();

				//wait for friend message
				if (player_number == 2 && i == 0)
					cout << "\nWaiting for friend...\n" << endl;

				//send hand size
				send_hand_size(socket, hand.size());

				if (player_number == 1) {
					if (i == 0)
						cout << endl;
					display_hand(hand);
				}

				//send player 2's hand
				if (player_number == 2)
					send_cards(socket, hand);

				if (hand.size() > 0) {
					unsigned int choice;
					if (player_number == 1)
						choice = ask_choice(hand.size());
					else if (player_number == 2)
						choice = recieve_choice(socket);
					Card* played_card = player->play_card(choice - 1);
					round_cards.push_back(played_card);

					//send cards
					send_cards(socket, round_cards, privileged_suit);

					if (player_number == 1 && i == 0)
						cout << endl;

					if (player_number == 1 && i == 1)
						display_cards(round_cards, privileged_suit, player_number, i);
					else
						display_card(played_card, privileged_suit, player_number, i);

				}

				if (eldest > number_of_players)
					eldest = 1;

				if (i == number_of_players - 1 && hand.size() == 0)
					is_finished = true;
			}

			Card* winner_card = round_winner_card(round_cards, privileged_suit);
			display_winner_card(winner_card);
			display_round_winner(winner_card);

			//send round results
			send_round_results(socket, winner_card);

			unsigned int winner_number = winner_card->get_player_number();
			Player* round_winner = players[winner_number - 1];

			for (Card* card : round_cards)
				round_winner->claim_cards(card);

			//Identify the eldest player for the next round
			eldest = winner_card->get_player_number();
			deal_cards(deck, next_card, players, eldest);

			//send eldest
			send_eldest(socket, eldest);
		}

		vector<unsigned int> scores = get_scores(players, deck);
		display_final_scores(scores);
		display_game_winner(scores);

		//send final scores
		send_final_scores(socket, scores);
	}
	break;

	case 2:
	{

		//Fix number of players
		unsigned int number_of_players = 2;

		//Recieve and display upcard
		display_upcard(socket);

		unsigned int round {1};
		unsigned int eldest {1};

		bool is_finished {false};

		while (!is_finished) {
			display_round(round++);

			//display the number of cards left in deck
			display_cards_left(round - 1, number_of_players);

			for (unsigned int i = 0; i < number_of_players; ++i) {
				unsigned int player_number = eldest++;

				//recieve hand size
				unsigned int hand_size = recieve_hand_size(socket);

				//wait for friend message
				if (player_number == 1 && i == 0)
					cout << "\nWaiting for friend...\n" << endl;

				if (player_number == 2) {
					cout << endl;
					display_hand(socket);
				}


				if (hand_size > 0) {
					if (player_number == 2) {
						send_choice(socket, hand_size);
						if (i == 1)
							cout << endl;
					}

					//display_cards
					display_cards(socket, player_number, i);
				}

				if (eldest > number_of_players)
					eldest = 1;

				if (i == number_of_players - 1 && hand_size == 1)
					is_finished = true;
			}

			//display round results
			display_round_results(socket);

			//recieve eldest
			eldest = recieve_eldest(socket);
		}

		//recieve final scores
		display_final_scores(socket);
	}
	break;

	default:
		error("Unable to launch network play !");
		break;
	}
}

char ask_retry() {
	char retry;
	cout << "\nRetry? (y/n)" << endl;
	
	while (!cin  || cin >> retry) {
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');

		switch (retry) {
		case 'y': case 'n':
			break;

		default:
			cout << "\nPlease enter a valid choice." << endl;
			continue;
		}

		break;
	}
	
	return retry;
}

void display_glbl_menu() {
	cout << "[1] Same Screen" << endl;
	cout << "[2] Network" << endl;
	cout << "[x] Exit" << endl;
}

void display_ntwk_menu() {
	cout << "[1] Start Game" << endl;
	cout << "[2] Join Game" << endl;
	cout << "[x] Menu" << endl;
}

unsigned int ask_number_of_players(unsigned int min, unsigned int max) {
	unsigned int number_of_players {0};
	cout << "Please enter a number of players (2-4):" << endl;
	cin >> number_of_players;

	while (!cin || (number_of_players<min) || (number_of_players>max)) {
		refuse_input("Please enter a valid number !");
		cin >> number_of_players;
	}
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	return number_of_players;
}

void refuse_input(const char* message) {
	cin.clear();
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');
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
				error("Unable to deal card [1]: Out Of Range");
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
				error("Unable to deal card [2]: Out Of Range");
			deck[deck_index++]->add_card(players[player_index++]);
		}
	}
}

void display_cards_left(unsigned int round, unsigned int number_of_players) {
	if (round == 36/number_of_players - 3)
		cout << "\nOnly " << number_of_players << " cards left." << endl;
}

void display_card(const Card* card, const Card::Suit suit, unsigned int player_number, unsigned int turn) {
	cout << *card << ((card->get_suit() == suit) ? "*" : "") << endl;
	if (player_number == 2 && turn == 0)
		cout << endl;
}

void display_cards(const vector<Card*>& cards, const Card::Suit suit) {
	cout << endl;
	for (Card* card : cards)
		cout << *card << ((card->get_suit() == suit)? "*" : "") << endl;
}

void display_cards(const vector<Card*>& cards, const Card::Suit suit, unsigned int player_number, unsigned int turn) {
	if (player_number == 1 && turn == 1)
		cout << endl;
	for (Card* card : cards)
		cout << *card << ((card->get_suit() == suit) ? "*" : "") << endl;
}

char* display_upcard(const unsigned int number_of_players, const vector<Card*>& deck) {
	unsigned int deck_index = 3 * number_of_players;
	if (deck_index < 0 || deck_index >= deck.size())
		error("Unable to display upcard: Out Of Range");
	cout << "\nUpcard: " << *deck[deck_index] << " !" << endl;

	char* upcard = deck[deck_index]->get_name();

	return upcard;
}

void display_round(const unsigned int round) {
	cout << "\nRound " << round << endl;
}

void display_player_turn(const unsigned int player_number) {
	cout << "\nPlayer " << player_number << "\'s turn: " << endl;
}

void display_hand(const vector<Card*>& hand) {
	for (unsigned int i = 0; i < hand.size(); ++i)
		cout << i + 1 << ": " << *hand[i] << endl;
}

void display_winner_card(const Card* card) {
	cout << "\n" << *card << " beats the other card(s)." << endl;
}

void display_round_winner(const Card* card) {
	cout << "Player " << card->get_player_number() << " wins round !" << endl;
}

unsigned int ask_choice(unsigned int hand_size) {
	unsigned int choice {0};
	cin >> choice;
	while (!cin || (choice < 1) || (choice > hand_size)) {
		refuse_input("Please enter a valid choice !");
		cin >> choice;
	}
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	return choice;
}

Card* compare_cards(Card* card_a, Card* card_b, const Card::Suit privileged_suit) {
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

Card* round_winner_card(vector<Card*> round_cards, const Card::Suit privileged_suit) {
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

vector<unsigned int> get_scores(const vector<Player*> players, const vector<Card*> deck) {
	vector<unsigned int> scores;
	for (Player* player : players) {
		unsigned int score = player->get_score(deck);
		scores.push_back(score);
	}
	return scores;
}

void display_final_scores(const vector<unsigned int> scores) {
	cout << "\nPlayer's final scores: " << endl;
	for (unsigned int i = 0; i < scores.size(); ++i) {
		cout << i + 1 << ": " << scores[i] << endl;
	}
}

bool is_draw(const vector<unsigned int> scores) {
	unsigned int draw_score = scores[0];
	for (unsigned int i = 1; i < scores.size(); ++i)
		if (draw_score != scores[i])
			return false;
	return true;
}

unsigned int game_winner(const vector<unsigned int> scores) {
	unsigned int winner_number = 1;
	unsigned int winner_score = scores[0];
	for (unsigned int i = 1; i < scores.size(); ++i) 
		if (scores[i] > winner_score) {
			winner_score = scores[i];
			winner_number = i + 1;
		}
	return winner_number;
}

void display_game_winner(const vector<unsigned int> scores) {
	if (is_draw(scores))
		cout << "\nDraw !" << endl;
	else
		cout << "\nPlayer " << game_winner(scores) << " wins game !" << endl;
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

//Data Transmission functions

void send_card(const SOCKET& socket, const char* card_name) {
	char sendbuf[DEFAULT_BUFLEN] = {0};
	strcpy_s(sendbuf, card_name);

	unsigned int isendResult {0};

	isendResult = send(socket, sendbuf, DEFAULT_BUFLEN, 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send card.");
		WSACleanup();
	}
}

void send_hand_size(const SOCKET& socket, unsigned int hand_size) {
	unsigned int isendResult {0};

	isendResult = send(socket, (char*)&hand_size, sizeof(unsigned int), 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send hand size.");
		WSACleanup();
	}
}

unsigned int recieve_hand_size(const SOCKET& socket) {
	unsigned int irecvResult {0};
	unsigned int hand_size;

	irecvResult = recv(socket, (char*)&hand_size, sizeof(unsigned int), MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve hand size.");
		WSACleanup();
	}

	return hand_size;
}

void send_cards(const SOCKET& socket, const vector<Card*>& cards) {
	char sendbuf[DEFAULT_BUFLEN] = {0};
	char *card_name;
	unsigned int cards_size = cards.size();

	for (unsigned int i = 0; i < cards_size; ++i) {
		card_name = cards[i]->get_name();
		if (i == 0)
			strcpy_s(sendbuf, card_name);
		else
			strcat_s(sendbuf, card_name);

		if (i != cards_size - 1)
			strcat_s(sendbuf, "|");
	}

	unsigned int isendResult = send(socket, sendbuf, DEFAULT_BUFLEN, 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send cards [1].");
		WSACleanup();
	}
}

void send_cards(const SOCKET& socket, const vector<Card*>& cards, const Card::Suit& suit) {
	char sendbuf[DEFAULT_BUFLEN] = {0};
	char *card_name;
	unsigned int cards_size = cards.size();

	for (unsigned int i = 0; i < cards_size; ++i) {
		card_name = cards[i]->get_name();
		if (i == 0)
			strcpy_s(sendbuf, card_name);
		else
			strcat_s(sendbuf, card_name);

		if (cards[i]->get_suit() == suit)
			strcat_s(sendbuf, "*");
		if (i != cards_size - 1)
			strcat_s(sendbuf, "|");
	}

	unsigned int isendResult = send(socket, sendbuf, DEFAULT_BUFLEN, 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send cards [2].");
		WSACleanup();
	}
}

void display_cards(const SOCKET& socket, unsigned int player_number, unsigned int turn) {
	char recvbuf[DEFAULT_BUFLEN] = {0};
	unsigned int irecvResult {0};

	irecvResult = recv(socket, recvbuf, DEFAULT_BUFLEN, MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve cards.");
		WSACleanup();
	}

	if (player_number == 2 && turn == 0)
		cout << endl;

	if (player_number == 1 && turn == 1) {
		unsigned int i = 0;
		while (recvbuf[i++] != '|');
		while (i < strlen(recvbuf))
			cout << recvbuf[i++];
	}
	else {
		for (unsigned int i = 0; i < strlen(recvbuf); ++i) {
			if (recvbuf[i] == '|')
				cout << "\n";
			else
				cout << recvbuf[i];
		}
	}
	cout << endl;
}

void send_round_results(const SOCKET& socket, const Card* card) {
	char sendbuf[DEFAULT_BUFLEN] = {0};
	char* card_name = card->get_name();

	strcpy_s(sendbuf, "\n");
	strcat_s(sendbuf, card_name);
	strcat_s(sendbuf, " beats the other card(s).\n");

	strcat_s(sendbuf, "Player ");
	if (card->get_player_number() == 1)
		strcat_s(sendbuf, "1");
	else if (card->get_player_number() == 2)
		strcat_s(sendbuf, "2");

	strcat_s(sendbuf, " wins round !");

	unsigned int isendResult = send(socket, sendbuf, DEFAULT_BUFLEN, 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send round_results.");
		WSACleanup();
	}
}

void display_round_results(const SOCKET& socket) {
	char recvbuf[DEFAULT_BUFLEN] = {0};
	unsigned int irecvResult {0};

	irecvResult = recv(socket, recvbuf, DEFAULT_BUFLEN, MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve round results.");
		WSACleanup();
	}

	cout << recvbuf << endl;
}

void send_choice(const SOCKET& socket, unsigned int hand_size) {
	unsigned int isendResult {0};
	unsigned int choice = ask_choice(hand_size);

	isendResult = send(socket, (char*)&choice, sizeof(unsigned int), 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send choice.");
		WSACleanup();
	}
}

unsigned int recieve_choice(const SOCKET& socket) {
	unsigned int irecvResult {0};
	unsigned int choice;

	irecvResult = recv(socket, (char*)&choice, sizeof(unsigned int), MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve choice.");
		WSACleanup();
	}

	return choice;
}

void send_eldest(const SOCKET& socket, unsigned int eldest) {
	unsigned int isendResult = send(socket, (char*)&eldest, sizeof(unsigned int), 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send eldest.");
		WSACleanup();
	}
}

unsigned int recieve_eldest(const SOCKET& socket) {
	unsigned int eldest;
	unsigned int irecvResult;

	irecvResult = recv(socket, (char*)&eldest, sizeof(unsigned int), MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve eldest.");
		WSACleanup();
	}

	return eldest;
}

void send_final_scores(const SOCKET& socket, const vector<unsigned int>& scores) {
	unsigned int sendbuf[2];
	for (unsigned int i = 0; i < scores.size(); ++i)
		sendbuf[i] = scores[i];
	unsigned int isendResult = send(socket, (char*)sendbuf, sizeof(int) * 2, 0);
	if (isendResult == SOCKET_ERROR) {
		error("Unable to send final scores.");
		WSACleanup();
	}
}

void display_final_scores(const SOCKET& socket) {
	unsigned int scores[2];
	unsigned int irecvResult = recv(socket, (char*)scores, sizeof(int) * 2, MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve final scores.");
		WSACleanup();
	}

	cout << "\nPlayer's final scores:" << endl;
	for (int i = 0; i < 2; ++i)
		cout << i + 1 << ": " << scores[i] << endl;

	if (scores[0] == scores[1])
		cout << "\nDraw !" << endl;
	else
		cout << "\nPlayer " << ((scores[0] > scores[1]) ? 1 : 2) << " wins game !" << endl;
}

void display_upcard(const SOCKET& socket) {
	char recvbuf[DEFAULT_BUFLEN];
	unsigned int irecvResult;

	irecvResult = recv(socket, recvbuf, DEFAULT_BUFLEN, MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve upcard.");
		WSACleanup();
	}

	string upcard_name = string(recvbuf);
	cout << "\nUpcard: " << upcard_name << " !" << endl;
}

void display_hand(const SOCKET& socket) {
	char recvbuf[DEFAULT_BUFLEN] = {0};
	unsigned int irecvResult {0};

	irecvResult = recv(socket, recvbuf, DEFAULT_BUFLEN, MSG_WAITALL);
	if (irecvResult == SOCKET_ERROR) {
		error("Unable to recieve hand.");
		WSACleanup();
	}

	unsigned int card_number {1};

	for (unsigned int i = 0; i < strlen(recvbuf); ++i) {
		if (i == 0)
			cout << card_number++ << ": ";
		if (recvbuf[i] == '|')
			cout << "\n" << card_number++ << ": ";
		else
			cout << recvbuf[i];
	}
	cout << endl;
}
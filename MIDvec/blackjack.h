//MIDvec Library.
	//Machine learning.
	//Image development.
	//Digital signal processing.
	//Vectors.
		//(and more)

//Commercial license for MIDvec: Terms stated at Binpress site.
//Contact is available through the Binpress site.

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "cpx_vec.h"
#include "dsp.h"
#include "misc_math.h"
#include "ann.h"
#include "bitmap.h"
#include "sort.h"

#define BLACKJACK_PAYOUT_BJ (6.0/5.0) //What's the return on an (A,10) pair.
#define NUM_DECKS_BJ 1 //How many decks in the shoe?
#define MAX_PENETRATION_BJ 0.7 //What proportion of the deck do we go through before shuffling?
#define TABLE_MIN 5.0 //What are the table min and max bets?
#define TABLE_MAX 100.0

#define SURRENDER_ALLOWED 1 //Does this casino allow surrender?
#define DEALER_HITS_ON_SOFT_17 1 //Does the dealer hit on soft 17?
#define DEALER_BLACKJACK_CHECK_PREGAME 1 //Does the dealer let the game continue if he has a natural?

#define MAX_CARDS_PER_HAND 20 //Max cards per hand, arbitrarily high
#define MAX_HANDS 20 //Max hands per game as a result of splitting

#define NUM_GAMES_TOTAL 1000000

typedef struct card {
	int faceval;
		//-1: card dealt
		//0: empty spot in vec
		//1: ace, for purpose of this project
		//2-10: number cards
		//10: J Q K
	int suit;
		//club, spade, heart, diamond (0, 1, 2, 3)
} card_o;
typedef struct vec_card {
	int len; //How many cards CAN be held
	card_o* arr;
} vec_card_o;
typedef struct bjhand {
	int len; //How many cards CAN be held
	card_o* arr;
	double bet;
	int is_originalhand;
		//0: Hand is a result of splitting from a previous hand
		//1: Hand is an original hand from the dealer
			//Basically from the first split you make every hand for the rest of the game is a 0.
} bjhand_o;

//Constructors.
vec_card_o new_vec_card(int length) {
	//Generates a vector of cards.
		//All cards have face value 0, considered to be empty.
	
	vec_card_o cards;
	cards.len = length;
	cards.arr = (card_o*)malloc(sizeof(card_o) * cards.len);
	
	int n = 0;
	for (n = 0; n < cards.len; n++)
		cards.arr[n].faceval = 0;
	
	return cards;
}
bjhand_o new_bjhand(int length) {
	bjhand_o bjhand;
	
	bjhand.len = length;
	bjhand.arr = (card_o*)malloc(sizeof(card_o) * bjhand.len);
	bjhand.bet = 0;
	
	int n = 0;
	for (n = 0; n < bjhand.len; n++) {
		bjhand.arr[n].faceval = 0;
		bjhand.arr[n].suit = 0;
	}
	bjhand.is_originalhand = 0;
	
	return bjhand;
}
vec_card_o new_deck(vec_card_o cardvec) {
	int n = 0;
	int cur_val = 0;
	
	for (n = 0; n < cardvec.len; n++) {
		
		if ((n % ((cardvec.len / 52)*4)) == 0)
			cur_val++;
		
		if (cur_val > 10)
			cur_val = 10;
		
		cardvec.arr[n].faceval = cur_val;
	}
	for (n = 0; n < cardvec.len; n++) {
		if (cardvec.arr[n].faceval == 1)
			cardvec.arr[n].faceval = 11; 
	}
	return cardvec;
}

//Menial operations.
vec_card_o shuffle(vec_card_o cur_deck) {
	
	vec_card_o new_deck = new_vec_card(cur_deck.len);
		int n = 0;
		int new_addr = -1;
		
	//For each card in cur_deck.
		//Randomly select a new address to put that card at in the new deck.
		//If that spot is empty in new_deck, you can fill that spot with this card.
		
	for (n = 0; n < cur_deck.len; n++) {
		while (new_deck.arr[new_addr].faceval != 0)
			new_addr = int_rand_given_bound(0, cur_deck.len - 1);
		new_deck.arr[new_addr] = cur_deck.arr[n];
	}
	
	return new_deck;
}
int num_nonzero_cards(card_o* arr, int len) {
	int n = 0;
	int ct = 0;
	for (n = 0; n < len; n++) {
		if (arr[n].faceval != 0)
			ct++;
	}
	////PRINT_LOCK("%d\r\n", ct);
	return ct;
}
card_o deal_card(vec_card_o shoe, int remove, bjhand_o hand) {
	
	int addr = int_rand_given_bound(0, shoe.len - 1);
	while (shoe.arr[addr].faceval == 0)
		addr = int_rand_given_bound(0, shoe.len - 1);
	
	(hand).arr[num_nonzero_cards((hand).arr, (hand).len)] = shoe.arr[addr];
		card_o card_out = shoe.arr[addr];
		
	if (remove == 1) {
		shoe.arr[addr].faceval = 0;
		shoe.arr[addr].suit = 0;
	}
	
	return card_out;
}
int score_hand(bjhand_o hand) {
	int sum = 0;
	int n = 0;
	for (n = 0; n < hand.len; n++)
		sum += hand.arr[n].faceval;
	
	return sum;
}
int count_faceval_occurrences(card_o* arr, int value) {
	int n = 0;
	int ct = 0;
	while (1) {
		if (arr[n].faceval == value)
			ct++;
		if (arr[n].faceval == 0) //Better be an empty spot in there.
			break;
		n++;
	}
	return ct;
}
int find_faceval_occurrence(card_o* arr, int value) {
	int n = 0;
	while (1) {
		if (arr[n].faceval == value)
			return n;
		else if (arr[n].faceval == 0)
			return -1;
		n++;
	}
}

void print_bjhand(bjhand_o hand) {
	int n = 0;
	while (hand.arr[n].faceval != 0) {
		printf("%d, ", hand.arr[n].faceval);
		n++;
	}
}
void clear_bjhand(bjhand_o hand) {
	int n = 0;
	for (n = 0; n < hand.len; n++) {
		hand.arr[n].faceval = 0;
		hand.arr[n].suit = 0;
		hand.bet = 0;
	}
}
void print_all_hands(bjhand_o* hands) {
	int n = 0;
	while (num_nonzero_cards(hands[n].arr, MAX_CARDS_PER_HAND) != 0) {
		print_bjhand(hands[n]);
		n++;
	}
}
void clear_card_vec(vec_card_o cardvec) {
	int n = 0;
	for (n = 0; n < cardvec.len; n++) {
		cardvec.arr[n].faceval = 0;
		cardvec.arr[n].suit = 0;
	}
}
void print_cardvec(vec_card_o cardvec) {
	int n = 0;
	for (n = 0; n < cardvec.len; n++)
		printf("%d, ", cardvec.arr[n].faceval);
}

//Menial hand categorizations.
int is_natural_blackjack(bjhand_o hand) {
	//A natural blackjack has two cards and totals 21. Only way for that to happen is A,10.
	if ((num_nonzero_cards(hand.arr, MAX_CARDS_PER_HAND) == 2) && (score_hand(hand) == 21))
		return 1;
	else
		return 0;
}

//Card counting.
int get_cardcount(card_o c) {
	if ((c.faceval >= 2) && (c.faceval <= 6))
		return 1;
	if ((c.faceval >= 7) && (c.faceval <= 9))
		return 0;
	if ((c.faceval == 10) || (c.faceval == 11))
		return -1;
}
int get_handcount(bjhand_o cc) {
	int n = 0;
	int sum = 0;
	for (n = 0; n < cc.len; n++)
		sum += get_cardcount(cc.arr[n]);
	return sum;
}
int sum_handcount(bjhand_o* hands, int len) {
	int n = 0;
	int sum = 0;
	for (n = 0; n < len; n++)
		sum += get_handcount(hands[n]);
	return sum;
}
double calc_truecount(int running_count) {
	return (double)(running_count) / (double)(NUM_DECKS_BJ);
}

//Direct gameplay.
double calc_bet(double truecount) {
	
	double bet;
	
	//Ploppy betting scheme:
	//bet = 20.0;
	
	//Simple card counting betting scheme.
	if (truecount <= 1)
		bet = TABLE_MIN;
	else if (truecount <= 3)
		bet = 20.0;
	else if (truecount <= 5)
		bet = 40.0;
	else
		bet = TABLE_MAX;
		
	return bet;
}
void aces_to_ones(bjhand_o hand) {
	int pp = score_hand(hand);
	if (pp > 21) {
		if (count_faceval_occurrences(hand.arr, 11) >= 1) {
			hand.arr[find_faceval_occurrence(hand.arr, 11)].faceval = 1;
		}
	}
}

int should_SURRENDER(bjhand_o hand, card_o upcard) {
	int pp = score_hand(hand);
	
	//0 -> Hit
	//1 -> Stand
	//2 -> Double
	//3 -> Surrender
	
	//If you have TWO CARDS and NO ACES... AND surrender is allowed at this casino... then you can be considered for late surrender. INITIAL, HARD TOTAL.
	if (((num_nonzero_cards(hand.arr, MAX_CARDS_PER_HAND) == 2) && (count_faceval_occurrences(hand.arr, 11) == 0)) && (SURRENDER_ALLOWED == 1)) {
		//Dealer has [2,A].
		//Table considers [17,14].
		if ((pp <= 17) && (pp >= 14)) { //If you're within the table, consider SURRENDER.
			int table[4][10] = {
			//   2  3  4  5  6  7  8  9  T  A
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//17
				{0, 0, 0, 0, 0, 0, 0, 3, 3, 3},//16
				{0, 0, 0, 0, 0, 0, 0, 0, 3, 0},//15
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}//14
			};
			if (table[3 - (pp - 14)][upcard.faceval - 2] == 3) //If you should SURRENDER, do so.
				return 3;
		}
		else //If you're not within the table, DON'T SURRENDER.
			return 0;
	}
	else
		return 0;
}
int should_SPLIT(bjhand_o hand, int upcardval) {
	
	//This is where I put the table.
	//Aces & 8s -> Always split.
	//10s & 5s -> Never split.
	//9s -> Always split, unless upcardval == 7, 10, or A.
	//7s -> Always split, unless upcardval == [8,A]
	//6s -> Always split, unless upcardval == [7,A]. Caveat at 2.
	//4s -> Never split, unless upcardval == [5,6]. Caveat at both.
	//3s & 2s -> Alway split, unless upcardval == [8,A]. Caveat at 2 & 3.
	
	//# rows in table is # cases :: Ace, 10, 9 ... 2.
	//# cols is # upcard values :: 2, 3, 4, ... Ace. Equal, square, 10x10.
	
	int table[10][10] = {
	//	 2  3  4  5  6  7  8  9  T  A..........Dealer upcard
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //Aces
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //10s
		{1, 1, 1, 1, 1, 0, 1, 1, 0, 0}, //9s
		
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //8s
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, //7s
		{1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, //6s
		
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //5s
		{0, 0, 0, 1, 1, 0, 0, 0, 0, 0}, //4s
		
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, //3s
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0} //2s
	};
	
	if ((hand.arr[0].faceval == hand.arr[1].faceval) && (num_nonzero_cards(hand.arr, MAX_CARDS_PER_HAND) == 2)) {
//		//PRINT_LOCK("used table\n");
		return table[9 - (hand.arr[0].faceval - 2)][upcardval - 2];
	}
	else {
//		//PRINT_LOCK("arent even equal\n");
		return 0;
	}
}
int should_DOUBLE_HIT_STAND(bjhand_o playerhand, card_o upcard) {
	
	//Order of priority:
		//Can I surrender?
		//Can I split?
		//Can I double?
		//Can I hit/stand?
	
	//3 options:
		//0 -> Hit.
		//1 -> Stand.
		//2 -> Double.
		//3 -> Surrender. 
	
	int pp = score_hand(playerhand);
	//PRINTLOCKprint_bjhand(playerhand);
	
	//1. Soft hand.
		//With A,9, always stand.
		//With A,8, always stand except if the dealer has a 6. In that case, double.
		//With A,7, double for [2,6], stand for [7,8], hit for [9,A].
		//With A,6, always hit except for [3,6]. In that case, double.
		//With A,5, always hit except for [4,6]. In that case, double.
			//A,4 is the same.
		//With A,3, always hit except for [5,6]. In that case, double.
			//A,2 is the same.
			
		//Dealer upcard. Ranges [2,A].
		//Player card. A,9 to A,2.
	int n = 0;
	if (count_faceval_occurrences(playerhand.arr, 11) == 1) {
		
		int table[8][10] = {
		//	 2  3  4  5  6  7  8  9  T  A
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//A,9
			{1, 1, 1, 1, 2, 1, 1, 1, 1, 1},//A,8
			{2, 2, 2, 2, 2, 1, 1, 0, 0, 0},//A,7
			{0, 2, 2, 2, 2, 0, 0, 0, 0, 0},//A,6
										   
			{0, 0, 2, 2, 2, 0, 0, 0, 0, 0},//A,5
			{0, 0, 2, 2, 2, 0, 0, 0, 0, 0},//A,4
			{0, 0, 0, 2, 2, 0, 0, 0, 0, 0},//A,3
			{0, 0, 0, 2, 2, 0, 0, 0, 0, 0}//A,2
		};
		
		//Row transformation:
		//[9,2] => [7,0] => [0,7].
		//7 - (score_hand(playerhand) - 11 - 2)
		
		//Column transformation:
		//[2,11] => [0,9].
		////PRINT_LOCK("Soft total of %d\n", pp);
		int row = 7 - (pp - 11 - 2);
		if (row < 0) {
			////PRINT_LOCK("Soft total, just standing.\n");
			return 1;
		}
		else {
			////PRINT_LOCK("Reaching into row %d, col %d\n", row, upcard.faceval - 2);
			return table[row][upcard.faceval - 2];
			
		}
	}
	
	//2. Hard hand.
		//17 or above: Always stand.
		//16: Always stand unless dealer has [7,A]. Then, hit.
			//15, 14, 13: Same.
		//12: Always hit unless dealer has [4,6]. Then, stand.
		//11: Always double.
		//10: Always double, unless dealer has [10,A]. Then, hit.
		//9: Always hit, unless dealer has [3,6]. Then double.
		//8 or below: Always hit.
	
	if (count_faceval_occurrences(playerhand.arr, 11) == 0) {
		int table[10][10] = {
		//   2  3  4  5  6  7  8  9  T  A
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//17
			{1, 1, 1, 1, 1, 0, 0, 0, 0, 0},//16
			{1, 1, 1, 1, 1, 0, 0, 0, 0, 0},//15
			{1, 1, 1, 1, 1, 0, 0, 0, 0, 0},//14
			
			{1, 1, 1, 1, 1, 0, 0, 0, 0, 0},//13
			{0, 0, 1, 1, 1, 0, 0, 0, 0, 0},//12
			{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//11
			
			{2, 2, 2, 2, 2, 2, 2, 2, 0, 0},//10
			{0, 2, 2, 2, 2, 0, 0, 0, 0, 0},//9
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}//8
		};
		////PRINT_LOCK("Hard total of %d\n", pp);
		if (pp > 17) {
			return 1;
		}
		if (pp < 8)
			return 0;
		else {
			return table[9 - (pp - 8)][upcard.faceval - 2];
		}
	}
	return 0;
}

int SPLIT(bjhand_o* playerhands, int cur_hand_ind, int num_hands, double new_bet) {
	
	//Move one card from this hand to the nearest empty hand.
	playerhands[num_hands].arr[0] = playerhands[cur_hand_ind].arr[1];
		playerhands[cur_hand_ind].arr[1].faceval = 0;
		playerhands[cur_hand_ind].arr[1].suit = 0;
	
	//Generate a bet for that hand.
	playerhands[num_hands].bet = new_bet;
	
	//Note we have an extra hand.
	return num_hands + 1;
}
void HIT(vec_card_o shoe, bjhand_o hand, int* running_count, int should_count, int allow_aces_to_ones) {
	//Deal a card.
	//Ace to one conversion if necessary and possible.
	
	card_o card_out = deal_card(shoe, 1, hand);
	int a = get_cardcount(card_out);
	
	if ((allow_aces_to_ones == 1) && (!((count_faceval_occurrences(hand.arr, 11) == 2) && (num_nonzero_cards(hand.arr, MAX_CARDS_PER_HAND) == 2))))
		aces_to_ones(hand);
	
	if (should_count == 1)
		(*running_count) += a;
}

//Evaluation.
int endhand_ANALYSIS(bjhand_o playerhand, bjhand_o dealerhand, vec_card_o shoe, int* running_count) {
	//Output codes.
	//REGULAR LOSS :: 0 (player bust, player lose by point)
	//REGULAR WIN :: 1 (dealer bust, player win by point)
	//BLACKJACK :: 2 (player blackjack)
	//TIE :: 3
	//SURRENDER LOSS :: 4
	
	//bjdecision value codes.
	//0 -> Hit
	//1 -> Stand
	//2 -> Double
	//3 -> Surrender
	
	//Buffer dealer points.
	
	if (DEALER_HITS_ON_SOFT_17 == 0) {
		while (score_hand(dealerhand) < 17)
			HIT(shoe, dealerhand, running_count, 1, 1);
	}
	if (DEALER_HITS_ON_SOFT_17 == 1) {
		while ((score_hand(dealerhand) < 17) || ((count_faceval_occurrences(dealerhand.arr, 11) == 1) && (score_hand(dealerhand) == 17)))
			HIT(shoe, dealerhand, running_count, 1, 1);
	}
	
	//Take score and #cards in each hand.
	int d = score_hand(dealerhand);
	int p = score_hand(playerhand);
		int np = num_nonzero_cards(playerhand.arr, playerhand.len);
		int nd = num_nonzero_cards(dealerhand.arr, dealerhand.len);
	
	//Show hands.
	//PRINT_LOCK("Player's cards: "); print_bjhand(playerhand); //PRINT_LOCK(" -> %d\r\n", p);
	//PRINT_LOCK("Dealer's cards: "); print_bjhand(dealerhand); //PRINT_LOCK(" -> %d\r\n", d);
	
	//Evaluate.
	if (p > 21) {
		//PRINT_LOCK("****LOSE BY BUST****");
		return 0;
	}
	if (p == 21) {
		if (d > 21) {
			if (np == 2) {
				//PRINT_LOCK("****WIN BY BLACKJACK****");
				return 2;
			}
			if (np != 2) {
				//PRINT_LOCK("****WIN BY POINT****");
				return 1;
			}
		}
		if (d == 21) {
			if ((np == 2) && (nd != 2)) {
				//PRINT_LOCK("****WIN BY BLACKJACK****");
				return 2;
			}
			if ((np != 2) && (nd == 2)) {
				//PRINT_LOCK("****LOSE BY BLACKJACK****");
				return 0;
			}
			else {
				//PRINT_LOCK("****TIE****");
				return 3;
			}
		}
		if (d < 21) {
			if (np == 2) {
				//PRINT_LOCK("****WIN BY BLACKJACK****");
				return 2;
			}
			if (np != 2) {
				//PRINT_LOCK("****WIN BY POINT****");
				return 1;
			}
		}
	}
	if (p < 21) {
		if (d > 21) {
			//PRINT_LOCK("****WIN BY DEALER BUST****");
			return 1;
		}
		if (d == 21) {
			//PRINT_LOCK("****LOSE BY POINT****");
			return 0;
		}
		if (d < 21) {
			if (p > d) {
				//PRINT_LOCK("****WIN BY POINT****");
				return 1;
			}
			if (p < d) {
				//PRINT_LOCK("****LOSE BY POINT****");
				return 0;
			}
			if (p == d) {
				//PRINT_LOCK("****TIE****");
				return 3;
			}
		}
	}
}
double endhand_PAYOUT(bjhand_o hand, int EHA) {
	
	//REGULAR LOSS :: 0 :: Lose bet.
	//REGULAR WIN :: 1 :: Gain back bet.
	//BLACKJACK WIN :: 2 :: Gain back 1.5*bet.
	//TIE :: 3 :: No money exchange.
	//SURRENDER LOSS :: 4 :: Lose 0.5*bet.

	//0 -> Hit
	//1 -> Stand
	//2 -> Double
	//3 -> Surrender
	
	if (EHA == 0) //REGULAR LOSS :: 0 :: Lose bet.
		return -hand.bet;
		
	if (EHA == 1) //REGULAR WIN :: 1 :: Gain bet.
		return hand.bet;
		
	if (EHA == 2) //BLACKJACK WIN :: 2 :: Gain 1.5*bet.
		return BLACKJACK_PAYOUT_BJ*hand.bet;
		
	if (EHA == 3) //TIE :: 3 :: No money exchange.
		return 0;
		
	if (EHA == 4) //SURRENDER LOSS :: 4 :: Lose 0.5*bet.
		return -0.5*hand.bet;
}
void print_bj_outcomes(ivec_o outcomes) {
	//REGULAR LOSS :: 0
	//REGULAR WIN :: 1
	//BLACKJACK :: 2
	//TIE :: 3
	//SURRENDER LOSS :: 4
	
	printf("\nLose: %d\nWin: %d\nBlackjack: %d\nTie: %d\nSurrender: %d\n", outcomes.arr[0], outcomes.arr[1], outcomes.arr[2], outcomes.arr[3], outcomes.arr[4]);
}

//Play.
void blackjack_sim() {
	
	//Blackjack: Automated card counting.

	//Junk test iterator variables, unused.
	int n = 0; int n1 = 0; int n2 = 0; int n3 = 0;
	
	//Cards objects.
	vec_card_o shoe = new_vec_card(NUM_DECKS_BJ*52);
		shoe = new_deck(shoe);
		shoe = shuffle(shoe);
	
	bjhand_o* playerhands = (bjhand_o*)malloc(sizeof(bjhand_o) * MAX_HANDS);
		for (n = 0; n < MAX_HANDS; n++) {
			playerhands[n] = new_bjhand(MAX_CARDS_PER_HAND);
			clear_bjhand(playerhands[n]);
		}
		int num_hands = 1;
		int cur_hand_ind = 0;
	bjhand_o dealerhand = new_bjhand(MAX_CARDS_PER_HAND);
		clear_bjhand(dealerhand);
		card_o upcard;
	
	//Basic play.
	int running_count = 0;
	int DHS = 0;
	//Represents choice to Double, Hit or Stand, when applicable.
		//0 -> Hit
		//1 -> Stand
		//2 -> Double
	int EHA = -1;
	//Represents output of Endhand Analysis function. One of the below numbers describes the result of any hand.
		//REGULAR LOSS :: 0
		//REGULAR WIN :: 1
		//BLACKJACK :: 2
		//TIE :: 3
		//SURRENDER LOSS :: 4
	
	int num_games_ELAPSED = 0;
	
	dvec_o dcash_pergame = new_dvec(NUM_GAMES_TOTAL);
	double total_cash_betted = 0;
	
	//Review.
	ivec_o count_outcomes = new_ivec(5);
	
	//Play.
	while (num_games_ELAPSED < NUM_GAMES_TOTAL) {
		
		printf("STARTING num_game %d\r\n", num_games_ELAPSED);
		
		//Deal cards.
			//Don't allow aces to ones, yet.
			//Aces to ones will happen for the dealer if necessary in EHA.
			//Aces to ones will happen for the player during the hand or from a split.
		HIT(shoe, playerhands[0], &running_count, 1, 0);
		HIT(shoe, playerhands[0], &running_count, 1, 0);
		HIT(shoe, dealerhand, &running_count, 1, 0);
		HIT(shoe, dealerhand, &running_count, 0, 0);
			upcard = dealerhand.arr[0];
			
		playerhands[0].bet = calc_bet(calc_truecount(running_count));
		total_cash_betted += playerhands[0].bet;
	
		if (!should_SURRENDER(playerhands[0], upcard)) {
			
			if (DEALER_BLACKJACK_CHECK_PREGAME == 1) {
				if ((upcard.faceval == 10) || (upcard.faceval == 11)) {
					if (is_natural_blackjack(dealerhand)) {
						EHA = endhand_ANALYSIS(playerhands[cur_hand_ind], dealerhand, shoe, &running_count);
						dcash_pergame.arr[num_games_ELAPSED] += endhand_PAYOUT(playerhands[cur_hand_ind], EHA);
							count_outcomes.arr[EHA]++;
						cur_hand_ind = num_hands; //Skip below for-loop
					}
				}
			}
			
			for (cur_hand_ind = 0; cur_hand_ind < num_hands; cur_hand_ind++) {
			
				//If this hand only has one card, force hit.
				if (num_nonzero_cards(playerhands[cur_hand_ind].arr, MAX_CARDS_PER_HAND) == 1)
					HIT(shoe, playerhands[cur_hand_ind], &running_count, 1, 0);
					//Don't allow aces to ones while adding a second card to a hand.
			
				while (1) {
					
					//Split this hand as necessary.
					while (should_SPLIT(playerhands[cur_hand_ind], upcard.faceval)) {
						num_hands = SPLIT(playerhands, cur_hand_ind, num_hands, calc_bet(calc_truecount(running_count)));
						HIT(shoe, playerhands[cur_hand_ind], &running_count, 1, 0); //Don't allow aces to ones while adding a second card to a hand.
					}
					
					//Decide DHS.
					DHS = should_DOUBLE_HIT_STAND(playerhands[cur_hand_ind], upcard);
					if (DHS == 2) {
						playerhands[cur_hand_ind].bet *= 2;
						HIT(shoe, playerhands[cur_hand_ind], &running_count, 1, 1);
						
						EHA = endhand_ANALYSIS(playerhands[cur_hand_ind], dealerhand, shoe, &running_count);
						dcash_pergame.arr[num_games_ELAPSED] += endhand_PAYOUT(playerhands[cur_hand_ind], EHA);
							count_outcomes.arr[EHA]++;
						break;
					}
					if (DHS == 1) {
						EHA = endhand_ANALYSIS(playerhands[cur_hand_ind], dealerhand, shoe, &running_count);
						dcash_pergame.arr[num_games_ELAPSED] += endhand_PAYOUT(playerhands[cur_hand_ind], EHA);
							count_outcomes.arr[EHA]++;
						break;
					}
					if (DHS == 0) {
						HIT(shoe, playerhands[cur_hand_ind], &running_count, 1, 1);
						//This HIT uses aces to ones because there is no situation where we would want to hit AND we would want to keep an ace.
						//If this HIT causes the player to bust, the player will automatically choose to stand in the should_DHS() function,
						//taking us to EHA to show that the player lost by bust.
					}
				}
			}
		}
		else {
			printf("****LOSE BY SURRENDER****\n");
			dcash_pergame.arr[num_games_ELAPSED] += endhand_PAYOUT(playerhands[0], 4);
				count_outcomes.arr[4]++;
		}
		
		//Reset.
		num_hands = 1; cur_hand_ind = 0; EHA = 0; DHS = 0;
		
		for (cur_hand_ind = 0; cur_hand_ind < MAX_HANDS; cur_hand_ind++)
			clear_bjhand(playerhands[cur_hand_ind]);
		clear_bjhand(dealerhand);
		
		if (((double)(shoe.len - num_nonzero_cards(shoe.arr, shoe.len)) / (double)(shoe.len)) > MAX_PENETRATION_BJ) {
			printf("Generating new shoe...\n\n");
			shoe = new_deck(shoe); shoe = shuffle(shoe);
			running_count = 0;
		}
		
		num_games_ELAPSED++;
		printf("\n\n");
	}
	
	//Endsession analysis.
	printf("\n\n\n");
	
	//File data.
	FILE* fout = fopen("bjoutput.txt", "w");
	fprint_dvec(dcash_pergame, fout);
	fclose(fout);
	
	//Console data.
	print_bj_outcomes(count_outcomes);
	printf("Total profit: %lf\r\n", sum_d(dcash_pergame));
	printf("House edge: %lf%%\r\n\r\n", -100.0 * sum_d(dcash_pergame) / total_cash_betted);
}
#endif
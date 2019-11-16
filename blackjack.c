/* 	Assigment 8
	Ryan Vagle
	11/30/2018    
	BlackJack game FULL 
	
	Flow of gameplay:
	1. Alternate dealing initial 2 cards to each hand
	2. Player's turn
	3. Dealer's turn
	4. Determine winner
	5. Ask to play again
		
*/
	
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// function prototypes
void shuffle(int deck[]);
void deal(int deck[], int hand[], int cardsDealt, int handAmount);
void displayCard(int card);
void displayDealerHand(int hand[], int length);
void displayDealerHand2(int hand[], int length);
void displayPlayerHand(int hand[], int length);
void scrRefresh();
int scoreHand(int hand[], int cardsInHand);
int blackJackValue(int card_id);
void displayWinner(int dealerHand[], int playerHand[], int dealerCards, int playerCards);

// char array to print suits
char suits [4][9] = { "Hearts",
 "Diamonds",
 "Clubs",
 "Spades" };

// keep track of total wins and ties
int playerWins = 0;		
int dealerWins = 0;		
int ties = 0;			

int main(){
	
	// declare variables
	int deck[52]; 				// deck array
	int seed = time(NULL); 		// randomize seed for rand() function
	int i; 						// for loop variable
	int dealerHand[5];			// hand number 1 initiated to empty
	int playerHand[5];			// hand number 2 initiated to empty
	int playAgain = 'y';		// character for play again input
	
	//seed random for shuffle function
    srand(seed);

    while(playAgain == 'Y' || playAgain == 'y'){
    	int numberOfCardsDealt = 0;				// track the number of cards dealt
		int dealerHandCards = 0;				// track the number of cards in dealer's hand
		int playerHandCards = 0;				// track the number of cards in player's hand
		int hitOrStay = 'a';					// character for user input
		int dealer21 = 0;						// flag for dealer scoring 21 to start
    
	    // Initiate deck of cards and shuffle
		for(i = 0; i < 52; i++){
			deck[i] = i;
		}
		shuffle(deck);
		
		// Initialize the play screen
		scrRefresh();
		displayDealerHand(dealerHand, dealerHandCards);	
		displayPlayerHand(playerHand, playerHandCards);
		
		// 1. Alternate dealing initial 2 cards to each hand
		for(i=0; i < 2; i++){

			deal(deck, playerHand, ++numberOfCardsDealt, playerHandCards++); // card to player
			
			scrRefresh();
			displayDealerHand(dealerHand, dealerHandCards);	
			displayPlayerHand(playerHand, playerHandCards);
			
			deal(deck, dealerHand, ++numberOfCardsDealt, dealerHandCards++); // card to dealer
			
			scrRefresh();
			displayDealerHand(dealerHand, dealerHandCards);	
			displayPlayerHand(playerHand, playerHandCards);
		}

		printf("\n");
		
		// Dealer gets 21 exception
		if(scoreHand(dealerHand, dealerHandCards) == 21){
			dealer21 = 1;
			scrRefresh();
			displayDealerHand2(dealerHand, dealerHandCards);	
			displayPlayerHand(playerHand, playerHandCards);
			printf("Dealer has 21! Play to match!\n\n");
		}
	
		// 2. Player's turn
		while(hitOrStay != 'S' && hitOrStay != 's' && playerHandCards < 5){
			
			printf("Would you like to HIT? (H) or STAY? (S)\n");
			scanf(" %c", &hitOrStay);
			
			if(hitOrStay == 'H' || hitOrStay == 'h'){
				
				printf("\nYou've chosen to hit!\n");
				scrRefresh();
				deal(deck, playerHand, ++numberOfCardsDealt, playerHandCards++);
				
				if(dealer21){
					displayDealerHand2(dealerHand, dealerHandCards);	
				}
				else{
					displayDealerHand(dealerHand, dealerHandCards);
				}	
				displayPlayerHand(playerHand, playerHandCards);	
				
			} else if(hitOrStay == 'S' || hitOrStay == 's'){
				printf("\nYou've chosen to stay. The dealer will now play\n");
				sleep(1);
				scrRefresh();
				if(dealer21){
					displayDealerHand2(dealerHand, dealerHandCards);	
				}
				else{
					displayDealerHand(dealerHand, dealerHandCards);
				}	
				displayPlayerHand(playerHand, playerHandCards);	
				break;
				
			} else{
				printf("You didn't enter an available option. Try again.\n");
			}
			
			if(scoreHand(playerHand, playerHandCards) > 21){
				printf("Sorry, you're over 21. You bust. DEALER WINS.");
				break;
			}
			
			if(playerHandCards == 5){
				printf("Maximum allowable cards. Dealer's turn\n");
				sleep(1);
			}
		}
		
		// 3. Dealer's turn
		scrRefresh();
		displayDealerHand2(dealerHand, dealerHandCards);	
		displayPlayerHand(playerHand, playerHandCards);
		
		if(scoreHand(playerHand, playerHandCards) > 21){
			printf("Sorry, you're over 21. You bust. DEALER WINS.");
		}
		else{
			while(scoreHand(dealerHand, dealerHandCards) < 17 && dealerHandCards < 5){
				sleep(1);
				printf("Dealer hits!");
				deal(deck, dealerHand, ++numberOfCardsDealt, dealerHandCards++);
				scrRefresh();
				displayDealerHand2(dealerHand, dealerHandCards);	
				displayPlayerHand(playerHand, playerHandCards);
				
				if(scoreHand(dealerHand, dealerHandCards) > 21){
					printf("Dealer busts! PLAYER WINS.");
					sleep(2);
				}
			}
		}
		
		// 4. Determine winner
		scrRefresh();
		displayDealerHand2(dealerHand, dealerHandCards);	
		displayPlayerHand(playerHand, playerHandCards);
		displayWinner(dealerHand, playerHand, dealerHandCards, playerHandCards);
		
		// Show total wins for each
		printf("\t\t\t\tTotal dealer wins: %d\n", dealerWins);
		printf("\t\t\t\tTotal player wins: %d\n", playerWins);
		printf("\t\t\t\tTotal ties: %d\n", ties);
		
		// 5. Ask to play again
		printf("\nWould you like to play again? Y for Yes, anything else to quit.\n");
		scanf(" %c", &playAgain);
	}
	return 0;
}

// This function pauses and then resets the screen for a cleaner gameplay
void scrRefresh(){
	sleep(1);
	system("CLS");
	printf("-------Blackjack--------\n\n");
}

// Function to score a hand
int scoreHand(int hand[], int cardsInHand){
	int i;
	int score = 0;		// Score sum
	int aceCount = 0;	// Track aces in the event they need to be re-scored
	
	// Scoring
	for(i = 0; i < cardsInHand; i++){
		score += blackJackValue(hand[i]);
		if(blackJackValue(hand[i]) == 11){
			aceCount++;
		}
	}

	// Ace handling
	while(score > 21 && aceCount > 0){
		score -= 10;
		aceCount--;
	}	
	return score;
}

// Function to compute the Black Jack value of a card
int blackJackValue(int card_id){
	int value = card_id % 13;
 	if(value < 9){
 		return value + 2;
 	}
 	else if (value < 12){
 		return 10;
 	}
 	else{
 		return 11;
    }
}

// Function to shuffle the deck
void shuffle(int deck[]){
	int i;
	for(i = 0; i < 520; i++){
		int card1 = rand() % 52;
		int card2 = rand() % 52;	
		int temp = deck[card1];
		deck[card1] = deck[card2];
		deck[card2] = temp;
	}
}

// Function to deal a card from the deck
void deal(int deck[], int hand[], int cardsDealt, int handAmount){
	int i;
	hand[handAmount] = deck[0];
	for(i = 0; i < (52-cardsDealt); i++){
		deck[i] = deck[i+1];
	}
	deck[52-cardsDealt] = 99;
}

// Function to display a card
void displayCard(int card){
	if(card == 99){
		printf("   EMPTY");
	}
	else{
		//display face
		int value = card % 13;
		
		if(value < 9){
			printf("%2d ", value+2);
		}
		else if(value == 9){
			printf(" J ");
		}
		else if(value == 10){
			printf(" Q ");
		}
		else if(value == 11){
			printf(" K ");
		}
		else{
			printf(" A ");
		}
		//display suit
		printf("%s", suits[card/13]);
	}	
}

// Function to display dealer's hand with hidden card
void displayDealerHand(int hand[], int length){
	int i;
	if(length == 0){
		printf("Dealer---------Score: 0\n");
	}
	else if(length == 1){
		printf("Dealer---------Score: ??\n");
		printf(" ??????\n");
	}
	else{
		printf("Dealer---------Score: %d\n", blackJackValue(hand[1]));
		for(i = 0; i < length; i++){
			if(i==0){
				printf(" ??????\n");
			}
			else{
			displayCard(hand[i]);
			printf("\n");
			}
		}
	}
	printf("\n");
}

// Function to display dealer's hand without hidden card
void displayDealerHand2(int hand[], int length){
	int i;
	printf("Dealer---------Score: %d\n", scoreHand(hand, length));
	for(i = 0; i < length; i++){
		displayCard(hand[i]);
		printf("\n");
	}
	printf("\n");
}

// Function to display the player's hand
void displayPlayerHand(int hand[], int length){
	int i;
	printf("Player---------Score: %d\n", scoreHand(hand, length));
	for(i = 0; i < length; i++){
		displayCard(hand[i]);
		printf("\n");
	}
	printf("\n");
}

// Function to compute a winner
void displayWinner(int dealerHand[], int playerHand[], int dealerCards, int playerCards){
	
	int score1 = scoreHand(dealerHand, dealerCards);
	int score2 = scoreHand(playerHand, playerCards);
		
	printf("Dealer final score: ");
	if(score1 <= 21){
		printf("%d\n", score1);
	}
	else{
		printf("BUST\n");
		score1 = 0;
	}
	printf("Player final score: ");
	if(score2 <= 21){
		printf("%d\n", score2);
	}
	else{
		printf("BUST\n");
		score2 = 0;
	}
	
	if(score1 == score2){
		printf("\nNO WINNER -- PUSH!\n\n");
		ties++;
	}
	else if(score1 > score2){
		printf("\nDEALER WINS!\n\n");
		dealerWins++;
	}
	else{
		printf("\nPLAYER WINS!\n\n");
		playerWins++;
	}
}

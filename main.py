import random


class Deck:
    def __init__(self):
        self.suits = ['Hearts', 'Diamonds', 'Clubs', 'Spades']
        self.ranks = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
        self.cards = []
        self.build_deck()

    def build_deck(self):
        for suit in self.suits:
            for rank in self.ranks:
                self.cards.append(f"{rank} of {suit}")

    def shuffle(self):
        random.shuffle(self.cards)

    def draw_card(self):
        return self.cards.pop() if self.cards else None


class Card:
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank

    def __str__(self):
        return f"{self.rank} of {self.suit}"

    def get_value(self):
        if self.rank in ['J', 'Q', 'K']:
            return 10
        elif self.rank == 'A':
            return 11
        else:
            return int(self.rank)


class Shoe:
    def __init__(self, num_decks=6):
        self.num_decks = num_decks
        self.cards = []
        for deck_num in range(num_decks):
            deck = Deck()
            deck.shuffle()
            self.cards.extend(deck.cards)
        random.shuffle(self.cards)

    def draw(self):
        return self.cards.pop() if self.cards else None

    def needs_reshuffle(self):
        return len(self.cards) < 100


class Blackjack:
    def __init__(self):
        self.shoe = Shoe()

    def deal_card(self):
        if self.shoe.needs_reshuffle():
            self.shoe = Shoe()
        return self.shoe.draw()

    def calculate_value(self, hand):
        value = 0
        aces = 0
        for card in hand:
            rank = card.split()[0]
            if rank in ['J', 'Q', 'K']:
                value += 10
            elif rank == 'A':
                value += 11
                aces += 1
            else:
                value += int(rank)
        while value > 21 and aces:
            value -= 10
            aces -= 1
        return value


class Game:
    def __init__(self):
        self.blackjack = Blackjack()
        self.player_hand = []
        self.dealer_hand = []
        self.player_balance = 100000
        self.bet_amount = 1
        self.rounds_won = 0
        self.rounds_lost = 0
        self.detailed_results = {}

    def start_game(self):
        self.player_hand = [self.blackjack.deal_card(), self.blackjack.deal_card()]
        self.dealer_hand = [self.blackjack.deal_card(), self.blackjack.deal_card()]

        # displays your cards
        print("Your hand:", self.player_hand)
        print("Dealer shows:", self.dealer_hand[0])

        while True:
            choice = input("\nDo you want to [h]it or [s]tand? ").lower()
            if choice == 'h':
                card = self.blackjack.deal_card()
                self.player_hand.append(card)
                print("You drew:", card)
                print("Your hand:", self.player_hand)
                value = self.blackjack.calculate_value(self.player_hand)
                print("Your total:", value)
                if value > 21:
                    print("You busted!")
                    return
            elif choice == 's':
                print("You stand.")
                break
            else:
                print("Invalid input. Please enter 'h' or 's'.")

    def dealer_turn(self):
        while self.blackjack.calculate_value(self.dealer_hand) < 17:
            self.dealer_hand.append(self.blackjack.deal_card())

    def evaluate_winner(self):
        player_total = self.blackjack.calculate_value(self.player_hand)
        dealer_total = self.blackjack.calculate_value(self.dealer_hand)

        print("\nGame summary:")
        print("Your hand:", self.player_hand, "->", player_total)
        print("Dealer's hand:", self.dealer_hand, "->", dealer_total)

        if player_total > 21:
            print("You busted. Dealer wins.")
            self.player_balance -= self.bet_amount
            self.rounds_lost += 1
        elif dealer_total > 21 or player_total > dealer_total:
            print("You win!")
            self.player_balance += self.bet_amount
            self.rounds_won += 1
        elif player_total < dealer_total:
            print("Dealer wins.")
            self.player_balance -= self.bet_amount
            self.rounds_lost += 1
        else:
            print("Push (tie). No money won or lost.")

        print("Current balance: $", self.player_balance)


class AutoSimulator:
    def __init__(self, num_simulations=100000):
        self.num_simulations = num_simulations
        self.blackjack = Blackjack()
        self.results = {
            "hit_win": 0,
            "hit_loss": 0,
            "hit_draw": 0,
            "stand_win": 0,
            "stand_loss": 0,
            "stand_draw": 0
        }
        self.detailed_results = {}

    def simulate_game(self):
        player_hand = [self.blackjack.deal_card(), self.blackjack.deal_card()]
        dealer_hand = [self.blackjack.deal_card(), self.blackjack.deal_card()]

        player_start = self.blackjack.calculate_value(player_hand)

        # Get dealer upcard value
        dealer_upcard = dealer_hand[0].split()[0]
        if dealer_upcard in ['J', 'Q', 'K']:
            dealer_val = 10
        elif dealer_upcard == 'A':
            dealer_val = 11
        else:
            dealer_val = int(dealer_upcard)

        key = (player_start, dealer_val)

        decision = random.choice(['hit', 'stand'])

        if key not in self.detailed_results:
            self.detailed_results[key] = {
                "hit": {"win": 0, "loss": 0, "draw": 0},
                "stand": {"win": 0, "loss": 0, "draw": 0}
            }

        if decision == 'hit':
            player_hand.append(self.blackjack.deal_card())

        player_total = self.blackjack.calculate_value(player_hand)

        if player_total > 21:
            self.detailed_results[key][decision]["loss"] += 1
            self.results[f"{decision}_loss"] += 1
            return

        while self.blackjack.calculate_value(dealer_hand) < 17:
            dealer_hand.append(self.blackjack.deal_card())

        dealer_total = self.blackjack.calculate_value(dealer_hand)

        if dealer_total > 21 or player_total > dealer_total:
            self.detailed_results[key][decision]["win"] += 1
            self.results[f"{decision}_win"] += 1
        elif player_total < dealer_total:
            self.detailed_results[key][decision]["loss"] += 1
            self.results[f"{decision}_loss"] += 1
        else:
            self.detailed_results[key][decision]["draw"] += 1
            self.results[f"{decision}_draw"] += 1

    def run(self):
        for _ in range(self.num_simulations):
            if self.blackjack.shoe.needs_reshuffle():
                self.blackjack = Blackjack()
            self.simulate_game()

        self.print_detailed_summary()
        self.print_results()

    def print_detailed_summary(self):
        print("\n--- Detailed Outcome by Player and Dealer Starting Value ---")
        print(
            f"{'Player':<7} | {'Dealer':<7} | {'Action':<6} | {'Wins':<6} | {'Losses':<7} | {'Draws':<6} | {'Total':<6} | {'Win Rate':<9}")
        print("-" * 85)

        for (player_val, dealer_val) in sorted(self.detailed_results):
            for action in ['hit', 'stand']:
                wins = self.detailed_results[(player_val, dealer_val)][action]['win']
                losses = self.detailed_results[(player_val, dealer_val)][action]['loss']
                draws = self.detailed_results[(player_val, dealer_val)][action]['draw']
                total = wins + losses + draws
                win_rate = (wins / total) * 100 if total > 0 else 0
                print(
                    f"{player_val:<7} | {dealer_val:<7} | {action:<6} | {wins:<6} | {losses:<7} | {draws:<6} | {total:<6} | {win_rate:.2f}%")
            print("-" * 85)

    def print_results(self):
        total_wins = self.results["hit_win"] + self.results["stand_win"]
        total_losses = self.results["hit_loss"] + self.results["stand_loss"]
        total_draws = self.results["hit_draw"] + self.results["stand_draw"]
        net_gain = total_wins - total_losses
        final_balance = 100000 + net_gain

        print("\n--- Money Summary ---")
        print(f"{'Starting Balance':<25}: $100000")
        print(f"{'Total Wins (+$1 each)':<25}: ${total_wins}")
        print(f"{'Total Losses (-$1 each)':<25}: -${total_losses}")
        print(f"{'Total Draws (no money change)':<25}: {total_draws}")
        print(f"{'Final Balance':<25}: ${final_balance}")


def main():
    game = Game()

    while True:
        game.start_game()
        game.dealer_turn()
        game.evaluate_winner()

        print("\nSimulating results for 100,000 hands...")

        simulator = AutoSimulator()
        simulator.run()

        play_again = input("\nWould you like to run play another simulation (y/n): ").lower()

        if play_again != 'y':
            print("Thanks for playing!")
            break


if __name__ == "__main__":
    main()
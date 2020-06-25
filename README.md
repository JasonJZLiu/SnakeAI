# SnakeAI
The purpose of the Snek project is to develop an algorithm for an AI that maximizes the number of points it can score in a variant of the classic video game Snake. The algorithm aims to generate a sequence of desired moves for a snake called Magini on a 10 × 10 game board. On the board, one target (either a Moogle or a Hurry Pooter) will spawn at a random location for Magini to eat. However, Magini must eat that target before TIMEOUT, which is a function that sets the number of moves Magini can make after the appearance of the target. TIMEOUT resets whenever Magini eats the target while Magini increases its length by one. Magini scores one point for every move it makes, twenty points for eating a Moogle, and sixty points for eating a Hurry Pooter. The game ends when Magini’s head moves into itself, moves into the edge, or Magini runs out of moves due to TIMEOUT. Thus, the algorithm seeks to maximize the points Magini can acquire before the game ends with the augmentation to adjust the board game size.

The following YouTube link is a demonstration of this Snake AI.
https://www.youtube.com/watch?v=pKwhaKGeAgs

You may run to test it out:
	gcc -o snek_AI main.o snek_api.o
	./snek_AI

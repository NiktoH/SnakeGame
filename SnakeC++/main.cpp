#include <SDL.h>
#include "screen.hpp"
#include <numeric>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000
#define CELL_SIZE 10

struct SceneRect {
	int x, y, h, w;
};

void RenderOnScreen(SDL_Renderer* renderer, SceneRect& rect) {
	SDL_Rect screenRect{ rect.x * CELL_SIZE, rect.y * CELL_SIZE, rect.w * CELL_SIZE, rect.h * CELL_SIZE };
	SDL_RenderFillRect(renderer, &screenRect);
}



int SDL_main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	auto window = SDL_CreateWindow("Snake_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	auto renderer = SDL_CreateRenderer(window, -1, 0);

	

	enum Directions {
		DOWN,
		UP,
		LEFT,
		RIGHT
	};


	//sneak head
	SceneRect head{ 50,50, 1, 1 };
	
	//body cont
	std::deque<SceneRect> tail;
	
	//apple cont
	SceneRect apple = {(rand() % (WINDOW_WIDTH / CELL_SIZE), rand() % (WINDOW_HEIGHT / CELL_SIZE), 1, 1)};		

	bool running = true;
	int dir = 0;

	while (running) {
		//check input
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) { running = false; }
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_DOWN) { dir = DOWN; }
				if (e.key.keysym.sym == SDLK_UP) { dir = UP; }
				if (e.key.keysym.sym == SDLK_LEFT) { dir = LEFT; }
				if (e.key.keysym.sym == SDLK_RIGHT) { dir = RIGHT; }
			}
		}
		SceneRect prevHead = head;
		
		//move
		switch (dir) {
		case DOWN:
			head.y += 1; break;
		case UP:
			head.y -= 1;  break;
		case LEFT:
			head.x -= 1;break;
		case RIGHT:
			head.x += 1; break;
		}
		//50
		

		//detect apple and snake
		tail.push_back(prevHead);
		
		if (head.x == apple.x && head.y == apple.y)
		{
			
			apple.x = rand() % (WINDOW_WIDTH / CELL_SIZE);
			apple.y = rand() % (WINDOW_HEIGHT / CELL_SIZE);
			
		}
		else {
			
			tail.pop_front();
			
		}
		//detect snake and border
		if (head.x == 0 || head.x == 100) {
			running = false;
		}
		if (head.y == 100 || head.y == 0) {
			running = false;
		}
		
		

		/*
		
		  ### #

		*/

		//detect with himself
		std::for_each(tail.begin(), tail.end(), [&](auto& snake_segm) {
			if (head.x == snake_segm.x && head.y == snake_segm.y) {
				running = false;
			}
			});

		//don't spawn apples
		std::for_each(tail.begin(), tail.end(), [&](auto& snake_segm) {
			if (apple.x == snake_segm.x && apple.y == snake_segm.y) {
				running = false;
			}
		});
		
		
		//clear window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//drawbody
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		std::for_each(tail.begin(), tail.end(), [&](auto& snake_segm) {
			RenderOnScreen(renderer, snake_segm);
		});
		RenderOnScreen(renderer, head);
		//drawapples
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		RenderOnScreen(renderer, apple);
	

		SDL_RenderPresent(renderer);
		SDL_Delay(60);

	}
	return 0;
}
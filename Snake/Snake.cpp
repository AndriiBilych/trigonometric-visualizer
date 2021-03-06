#include "Snake.h"

Snake::Snake(int width, int height, int fontWidth, int fontHeight) 
	: Engine(width, height, fontWidth, fontHeight) {
	moveDir = north;
	increment = Position(0, -1);
	timePassedMove = .0f;
	timerMove = .25f;
	timePassedFood = .0f;
	timerFood = 3.0f;
	speed = 1.0f;
	maxSpeed = 3.0f;
	multiplier = 1.1f;
	ChangeTitle(L"Snake");
	scrHeight = height;
	scrWidth = width;
	bodySize = 10;
	foodMax = 5;
	score = 0;
	body.reserve(200);
	food.reserve(foodMax);

	//game variables
	isGameover = false;
}

bool Snake::Start() {
	body.emplace_back(scrWidth / 2, scrHeight / 2);

	for (int i = 1; i < bodySize; i++)
		body.emplace_back(body[i - 1] + Position(0, 1));

	return true;
}

bool Snake::Update(float timeElapsed) {
	if (!isGameover) {
		ClearScreen();

		DisplayGame();

		HandleInput();

		//Timer for snake movement
		timePassedMove += timeElapsed * speed;
		if (timePassedMove >= timerMove)
		{
			MoveSnake();

			timePassedMove -= timerMove;
		}
		
		//Timer for food spawning
		timePassedFood += timeElapsed;
		if (timePassedFood >= timerFood)
		{
			SpawnFood();

			timePassedFood -= timerFood;
		}
	}
	else {
		ChangeTitle(L"You lost!");
	}

	return true;
}

void Snake::DisplayGame() {
	//draw head
	Draw(body[0].x, body[0].y, ' ', BG_DARK_RED);
	//draw body
	std::for_each(std::next(begin(body)), end(body), [this](Position p) { Draw(p.x, p.y, ' ', BG_DARK_GREEN); });

	//draw food
	std::for_each(begin(food), end(food), [this](Position p) { Draw(p.x, p.y, ' ', BG_RED); });

	//Draw score
	ChangeTitle(L"Score: %d", score);
}

void Snake::HandleInput() {
	if (GetKey(VK_UP).pressed && moveDir != south) {
		moveDir = north;
		increment = Position(0, -1);
	}
	else if (GetKey(VK_DOWN).pressed && moveDir != north) {
		moveDir = south;
		increment = Position(0, 1);
	}
	else if (GetKey(VK_LEFT).pressed && moveDir != east) {
		moveDir = west;
		increment = Position(-1, 0);
	}
	else if (GetKey(VK_RIGHT).pressed && moveDir != west) {
		moveDir = east;
		increment = Position(1, 0);
	}
}

void Snake::MoveSnake() {
	auto stashedPos = body[body.size() - 1];
	for (int i = body.size() - 1; i >= 0; i--) 
		if (i > 0)
			body[i] = body[i - 1];

	body[0] += increment;

	//Food picking up
	auto it = std::find_if(begin(food), end(food), [=](Position p) { return p == body[0]; });

	if (it != end(food)) {
		score++;
		if (speed < maxSpeed && score % 5 == 0) speed *= multiplier;
		food.erase(it);
		body.emplace_back(stashedPos);
	}

	//Screen overflow protection
	if (!IsWithinScreen(body[0])) {
		if (body[0].x < 0) body[0].x = scrWidth - 1;
		else if (body[0].x > scrWidth - 1) body[0].x = 0;
		else if (body[0].y < 0) body[0].y = scrHeight - 1;
		else if (body[0].y > scrHeight - 1) body[0].y = 0;
	}
	
	//Detect collision with body
	if (std::any_of(std::next(begin(body)), end(body), [=](Position pos) { return body[0] == pos; })) isGameover = true;
}

void Snake::SpawnFood() {
	if (food.size() < foodMax) {
		Position pos;

		do pos = Position(rand() % scrWidth, rand() % scrHeight);
		while (std::any_of(begin(body), end(body), [=](Position p) { return pos == p; }));
		
		food.emplace_back(pos);
	}
}

bool Snake::IsWithinScreen(Position pos) const {
	return pos.x >= 0 &&
		pos.x <= scrWidth &&
		pos.y >= 0 &&
		pos.y <= scrHeight;
}

Snake::~Snake() {

}
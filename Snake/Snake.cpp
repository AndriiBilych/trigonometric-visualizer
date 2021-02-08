#include "Snake.h"

Snake::Snake(int width, int height, int fontWidth, int fontHeight) 
	: Engine(width, height, fontWidth, fontHeight) {
	moveDir = north;
	increment = Position(0, -1);
	timePassed = .0f;
	timer = .25f;
	ChangeTitle(L"Snake");
	scrHeight = GetScreenHeight();
	scrWidth = GetScreenWidth();
}

bool Snake::Start() {
	body.emplace_back(scrWidth / 2, scrHeight / 2);
	body.emplace_back(body[0] + Position(0, 1));
	body.emplace_back(body[1] + Position(0, 1));
	body.emplace_back(body[2] + Position(0, 1));
	body.emplace_back(body[3] + Position(0, 1));

	return true;
}

bool Snake::Update(float timeElapsed) {
	ClearScreen();

	DisplaySnake();

	HandleInput();

	//Timer for snake movement
	timePassed += timeElapsed;
    if (timePassed >= timer)
    {
		MoveSnake();
        timePassed -= timer;
    }

	return true;
}

void Snake::DisplaySnake() {
	std::for_each(begin(body), end(body), [this](Position p) { Draw(p.x, p.y, ' ', 0x20); });
	Draw(body[0].x, body[0].y, ' ', 0x40);
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
	for (int i = body.size() - 1; i >= 0; i--) 
		if (i > 0)
			body[i] = body[i - 1];

	body[0] += increment;
	
	//Screen overflow protection
	if (!IsWithinScreen(body[0])) {
		if (body[0].x < 0) body[0].x = scrWidth;
		else if (body[0].x > scrWidth) body[0].x = 0;
		else if (body[0].y < 0) body[0].y = scrHeight;
		else if (body[0].y > scrHeight) body[0].y = 0;
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
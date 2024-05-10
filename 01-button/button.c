#include "raylib.h"

#define WIDTH 800
#define HALF_WIDTH WIDTH / 2
#define HEIGHT 600
#define HALF_HEIGHT HEIGHT / 2

#define FPS 120

#define BRUSH_SIZE 12
#define BRUSH_HALF_SIZE BRUSH_SIZE / 2

#define EM 4


typedef enum {
	Released,
	Hovered,
	Pressed,
} ButtonState;

typedef struct {
	char* text;
	int text_width;

	Rectangle rect;

	int padding;
	float roundness;
	int font_size;

	Color foreground;
	Color released_color;
	Color hovered_color;
	Color pressed_color;

	ButtonState state;
} Button;

int pad(int value, int pad) {
	return value + 2 * pad * EM;
}

void UpdateButton(Button* button) {
	Vector2 pos = GetMousePosition();
	if (
		pos.x >= button->rect.x				&& 
		pos.x <= button->rect.x + button->rect.width	&&
		pos.y >= button->rect.y				&&
		pos.y <= button->rect.y + button->rect.height
	) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			button->state = Pressed;
		} else {
			button->state = Hovered;
		}
	} else {
		button->state = Released;
	}
}

Button NewButton(char* text, Color foreground, int font_size, int x, int y, int padding, float roundness, Color released, Color hovered, Color pressed) {
	int text_width = MeasureText(text, font_size);

	int width = pad(text_width, padding);
	int height = pad(font_size, padding);

	Button button = {
		.text = text,
		.text_width = text_width,
		.rect = (Rectangle){
			x - (int)(width / 2),
			y - (int)(height / 2),
			width,
			height,
		},

		.padding	= padding,
		.roundness	= roundness,
		.font_size	= font_size,

		.foreground = foreground,
		.released_color = released,
		.hovered_color	= hovered,
		.pressed_color	= pressed,
	};


	return button;
}


void DrawButton(Button button) {
	const int ROUND_BUTTON_SEGMENTS = 10;

	switch (button.state) {
		case Released:
			DrawRectangleRounded(button.rect, button.roundness, ROUND_BUTTON_SEGMENTS, button.released_color);
			break;
		case Hovered:
			DrawRectangleRounded(button.rect, button.roundness, ROUND_BUTTON_SEGMENTS, button.hovered_color);
			break;
		case Pressed:
			DrawRectangleRounded(button.rect, button.roundness, ROUND_BUTTON_SEGMENTS, button.pressed_color);
			break;
	}

	DrawText(
		button.text, 
		button.rect.x + button.rect.width / 2 - (int)(button.text_width / 2),
		button.rect.y + button.rect.height / 2 - (int)(button.font_size / 2), 
		button.font_size, 
		button.foreground
	);
}

int main(void) {

	InitWindow(WIDTH, HEIGHT, "Hello, Raylib!");

	SetTargetFPS(FPS);

	RenderTexture canvas = LoadRenderTexture(WIDTH, HEIGHT);

	Button button = NewButton(
		"Responsive Button!", 
		BLACK,
		24, 
		HALF_WIDTH, 
		HALF_HEIGHT, 
		8, 
		10.0f, 
		LIGHTGRAY, 
		PURPLE,
		GREEN
	);

	while (!WindowShouldClose()) {
		UpdateButton(&button);

		BeginTextureMode(canvas);

		DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, RED);
		DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, RED);


		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			Vector2 pos = GetMousePosition();


			DrawCircle((int)pos.x - BRUSH_HALF_SIZE, (int) pos.y - BRUSH_HALF_SIZE, BRUSH_SIZE, RED);
		}

		DrawButton(button);

		EndTextureMode();


		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawTextureRec(canvas.texture, (Rectangle) {0, 0, (float)canvas.texture.width, (float)-canvas.texture.height}, (Vector2) {0, 0}, WHITE);


		DrawFPS(10, 10);
		EndDrawing();
	}


	CloseWindow();
	
	return 0;
}

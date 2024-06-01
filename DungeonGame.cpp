#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <string>
#include <climits>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;
const float GRID_AREA_RATIO = 0.7; // 网格占窗口面积的比例
const float NEGATIVE_PROBABILITY = 0.8; // 负值出现的概率

std::vector<std::vector<int>> dungeon;
std::vector<std::vector<int>> path;
int M; // 行数
int N; // 列数
int pathSum = 0; // 路径之和
int minHP = 0; // 最小健康值

struct Button {
    SDL_Rect rect;
    const char* label;
};

Button generateButton = { {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT}, "Regen" };
Button calculateButton = { {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT}, "Calc" };

void generateDungeon(int M, int N) {
    dungeon.resize(M, std::vector<int>(N, 0));
    srand(time(0));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float randValue = static_cast<float>(rand()) / RAND_MAX;
            if (randValue < NEGATIVE_PROBABILITY) {
                dungeon[i][j] = rand() % 101 - 100; // 负值
            } else if (randValue < NEGATIVE_PROBABILITY + 0.15) {
                dungeon[i][j] = rand() % 101; // 正值
            } else {
                dungeon[i][j] = 0; // 零
            }
        }
    }
    path.assign(M, std::vector<int>(N, 0));
}

int calculateMinimumHP(const std::vector<std::vector<int>>& dungeon) {
    int M = dungeon.size();
    int N = dungeon[0].size();
    std::vector<std::vector<int>> dp(M, std::vector<int>(N, INT_MIN));
    dp[0][0] = dungeon[0][0];

    for (int i = 1; i < M; ++i) {
        dp[i][0] = dp[i - 1][0] + dungeon[i][0];
    }
    for (int j = 1; j < N; ++j) {
        dp[0][j] = dp[0][j - 1] + dungeon[0][j];
    }

    for (int i = 1; i < M; ++i) {
        for (int j = 1; j < N; ++j) {
            dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]) + dungeon[i][j];
        }
    }

    // 查找最大路径和
    pathSum = dp[M - 1][N - 1];

    // 记录路径
    int i = M - 1;
    int j = N - 1;
    path.assign(M, std::vector<int>(N, 0));
    path[i][j] = 1;
    while (i > 0 || j > 0) {
        if (i == 0) {
            j--;
        } else if (j == 0) {
            i--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
        path[i][j] = 1;
    }

    minHP = pathSum >= 0 ? 1 : -pathSum + 1;
    return minHP;
}

void renderButton(SDL_Renderer* renderer, TTF_Font* font, Button& button) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &button.rect);

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button.label, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = { button.rect.x + (BUTTON_WIDTH - textWidth) / 2, button.rect.y + (BUTTON_HEIGHT - textHeight) / 2, textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void renderDungeon(SDL_Renderer* renderer, TTF_Font* font, int offsetX, int offsetY, int cellSize) {
    int fontSize = std::min(cellSize / 3, 24); // 动态计算字体大小，最大为24
    TTF_Font* cellFont = TTF_OpenFont("arial.ttf", fontSize); // 请确保路径正确或更换为你的字体路径

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            SDL_Rect cell;
            cell.x = offsetX + j * cellSize;
            cell.y = offsetY + i * cellSize;
            cell.w = cellSize;
            cell.h = cellSize;

            if (path[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // 黄色表示路径
            } else if (dungeon[i][j] < 0) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 红色表示负值
            } else if (dungeon[i][j] > 0) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 绿色表示正值
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色表示零
            }

            SDL_RenderFillRect(renderer, &cell);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色网格
            SDL_RenderDrawRect(renderer, &cell);

            // 绘制数值
            std::string value = std::to_string(dungeon[i][j]);
            SDL_Surface* textSurface = TTF_RenderText_Solid(cellFont, value.c_str(), {0, 0, 0, 255});
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_Rect textRect = { cell.x + (cellSize - textWidth) / 2, cell.y + (cellSize - textHeight) / 2, textWidth, textHeight };

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }
    }

    TTF_CloseFont(cellFont);
}

void renderPathSum(SDL_Renderer* renderer, TTF_Font* font, int offsetX, int offsetY) {
    std::string text = "Path Sum: " + std::to_string(pathSum);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), {0, 0, 0, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = { offsetX + (WINDOW_WIDTH - textWidth) / 2, static_cast<int>(offsetY + M * (WINDOW_HEIGHT * GRID_AREA_RATIO / M) + 10), textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void renderMinHP(SDL_Renderer* renderer, TTF_Font* font, int offsetX, int offsetY) {
    std::string minHPText = "Min HP: " + std::to_string(minHP);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, minHPText.c_str(), {0, 0, 0, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = { offsetX + (WINDOW_WIDTH - textWidth) / 2, static_cast<int>(offsetY + M * (WINDOW_HEIGHT * GRID_AREA_RATIO / M) + 30), textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void renderKnightAndPrincess(SDL_Renderer* renderer, SDL_Texture* knightTexture, SDL_Texture* princessTexture, int offsetX, int offsetY, int cellSize) {
    // 绘制骑士图标
    SDL_Rect knightRect = {offsetX - cellSize, offsetY, cellSize, cellSize};
    SDL_RenderCopy(renderer, knightTexture, NULL, &knightRect);

    // 绘制公主图标
    SDL_Rect princessRect = {offsetX + N * cellSize, offsetY + (M - 1) * cellSize, cellSize, cellSize};
    SDL_RenderCopy(renderer, princessTexture, NULL, &princessRect);
}

bool isMouseOverButton(int x, int y, Button& button) {
    return x > button.rect.x && x < button.rect.x + button.rect.w && y > button.rect.y && y < button.rect.y + button.rect.h;
}

int main(int argc, char* argv[]) {
    std::cout << "Enter the number of rows (M): ";
    std::cin >> M;
    std::cout << "Enter the number of columns (N): ";
    std::cin >> N;

    if (M < 1 || M > 100 || N < 1 || N > 100) {
        std::cerr << "Invalid grid size. M and N must be between 1 and 100." << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Dungeon Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("arial.ttf", 25); // 请确保路径正确或更换为你的字体路径
    if (font == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface* knightSurface = IMG_Load("knight.png");
    if (!knightSurface) {
        std::cerr << "Failed to load knight image! IMG_Error: " << IMG_GetError() << std::endl;
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, knightSurface);
    SDL_FreeSurface(knightSurface);

    SDL_Surface* princessSurface = IMG_Load("princess.png");
    if (!princessSurface) {
        std::cerr << "Failed to load princess image! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_DestroyTexture(knightTexture);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Texture* princessTexture = SDL_CreateTextureFromSurface(renderer, princessSurface);
    SDL_FreeSurface(princessSurface);

    bool quit = false;
    SDL_Event e;

    generateDungeon(M, N);

    while (!quit) {
        int gridWidth = static_cast<int>(WINDOW_WIDTH * GRID_AREA_RATIO);
        int gridHeight = static_cast<int>(WINDOW_HEIGHT * GRID_AREA_RATIO);
        int cellSize = std::min(gridWidth / N, gridHeight / M);
        int offsetX = (WINDOW_WIDTH - N * cellSize) / 2;
        int offsetY = (WINDOW_HEIGHT - M * cellSize) / 2;

        generateButton.rect.x = offsetX;
        generateButton.rect.y = offsetY + M * cellSize + 40;
        calculateButton.rect.x = offsetX + BUTTON_WIDTH + 10;
        calculateButton.rect.y = offsetY + M * cellSize + 40;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (isMouseOverButton(x, y, generateButton)) {
                    generateDungeon(M, N);
                } else if (isMouseOverButton(x, y, calculateButton)) {
                    minHP = calculateMinimumHP(dungeon);
                    std::cout << "Minimum initial health points required: " << minHP << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色背景
        SDL_RenderClear(renderer);

        renderDungeon(renderer, font, offsetX, offsetY, cellSize);
        renderButton(renderer, font, generateButton);
        renderButton(renderer, font, calculateButton);
        renderPathSum(renderer, font, offsetX, offsetY);
        renderMinHP(renderer, font, offsetX, offsetY);
        renderKnightAndPrincess(renderer, knightTexture, princessTexture, offsetX, offsetY, cellSize);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(knightTexture);
    SDL_DestroyTexture(princessTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

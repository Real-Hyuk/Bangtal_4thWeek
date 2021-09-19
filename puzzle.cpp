#include <bangtal>
#include <stdlib.h>
#include <ctime>
#include <iostream>
using namespace bangtal;
using namespace std;

bool size(int i) {
	return(300 + (i % 4) * 150 && 470 - (i / 4) * 150);
}


int main() {

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	srand((unsigned int)time(NULL));
	auto room1 = Scene::create("그림 선택 화면", "Images/qorud.png");
	auto room2 = Scene::create("그림1", "Images/back.png"); //1번 문제
	auto room21 = Scene::create("그림1정답", "Images/pic_1.png"); //정답 그림
	auto room3 = Scene::create("그림2", "Images/pic_2.png"); //정답 그림 
	auto room31 = Scene::create("그림2정답", "Images/pic_2.png"); //정답 그림 
	auto button1 = Object::create("Images/button1.png", room1, 500, 270); // 그림1 선택
	auto button2 = Object::create("Images/button2.png", room1, 500, 120); //그림2 선택, 준비중입니다.
	auto button_a1 = Object::create("Images/ans_1.png", room2, 0, 0); //정답보기
	auto button_a2 = Object::create("Images/ans_1.png", room3, 0, 0); //정답보기
	auto button_d1 = Object::create("Images/back1.png", room21, 0, 0); //뒤로가기
	auto button_d2 = Object::create("Images/back2.png", room31, 0, 0);

	// 시작화면
		//1번 그림 선택
	auto closed1 = true;
	button1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room2->enter();

		return true;
		});

	//2번 그림 선택
	auto closed2 = true;
	button2->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room3->enter();

		return true;
		});

	//그림1
	//정답왔다갔다
	button_a1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room21->enter();

		return true;
		});
	button_d1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room2->enter();

		return true;
		});




	//그림2
	//정답왔다갔다
	button_a2->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room31->enter();

		return true;
		});
	button_d2->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room1->enter();

		return true;
		});


	//300 + (i % 4) * 150, 470 - (i / 4) * 150
	//그림1 게임
	ObjectPtr game[16];
	ObjectPtr init_board[16];
	int blank = 15;
	for (int i = 0; i < 16; i++) {
		string filename = "Images/" + to_string(i + 1) + ".png";
		cout << filename << endl;

		//그림소환
		game[i] = Object::create(filename, room2, 300 + (i % 4) * 150, 470 - (i / 4) * 150);

		//그림 섞기
		/*
		Ptr ran[16];
		int temp[16];
		for (int i = 0; i < 15; i++) {
			int random = rand() % (16 - i) + i; // i 부터 16-i 랜덤값
			temp[i] = game[i];
			game[i] = game[random];
			game[random] = temp[i]; //섞기

		}*/

		//퍼즐 맞추기
		game[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool {
			int j = 0;
			for (j = 0; j < 16; j++) {
				if (game[j] == object)
					break;
			}

			if ((j % 4 > 0 && j - 1 == blank) || (j % 4 < 3 && j + 1 == blank) || (j > 3 && j - 4 == blank) || (j < 12 && j + 4 == blank)) {
				cout << "move " << blank << "<->" << j << endl;

				game[j]->locate(room2, 300 + (blank % 4) * 150, 470 - (blank / 4) * 150);
				game[blank]->locate(room2, 300 + (j % 4) * 150, 470 - (j / 4) * 150);
				// 조각 이동
				game[j] = game[blank];
				game[blank] = object;
				blank = j;

				int k = 0;
				for (k = 0; k < 16; k++) {
					if (game[k] != init_board[k])
						break;
				}
				if (k == 16) {
					showMessage("완성!");
				}
			}

			return true;
			});
	}

	game[blank]->hide();


	//시간제한 카운터 만들기
	/* 
	auto count = 0;
	auto timer = Timer::create(0.1f);
	timer->setOnTimerCallback([&](TimerPtr t)-> bool {
		cout << "timeout" << count << endl;
		int n = rand() % 4;
		int j = 0;
		do {
			switch (rand() % 4) {
			case 0: //좌
				j = blank - 1; break;
			case 1: // 우
				j = blank + 1; break;
			case 2: j = blank - 4; break; //아래
			case 3: j = blank + 4; break; //위
			}
		} while ((j < 0 || j>15) || !(j % 4 > 0 && j - 1 == blank) || (j % 4 < 3 && j + 1 == blank) || (j > 3 && j - 4 == blank) || (j < 12 && j + 4 == blank));

		if ((j > 0 && j < 16) && (j % 4 > 0 && j - 1 == blank) || (j % 4 < 3 && j + 1 == blank) || (j > 3 && j - 4 == blank) || (j < 12 && j + 4 == blank))
		{
			cout << "move " << blank << "<->" << j << endl;

			game_board[j]->locate(room2, 300 + (blank % 4) * 150, 470 - (blank / 4) * 150);
			game_board[blank]->locate(room2, 300 + (j % 4) * 150, 470 - (j / 4) * 150);
			// 배열 바꾸기
			auto object = game_board[j];
			game_board[j] = game_board[blank];
			game_board[blank] = object;
			blank = j;

		}

		count++;
		if (count < 100) {
			t->set(0.1f);
			t->start();
		}

		return true;
		});


	timer->start();

	*/
	startGame(room1);
}
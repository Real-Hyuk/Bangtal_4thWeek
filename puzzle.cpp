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
	auto room1 = Scene::create("�׸� ���� ȭ��", "Images/qorud.png");
	auto room2 = Scene::create("�׸�1", "Images/back.png"); //1�� ����
	auto room21 = Scene::create("�׸�1����", "Images/pic_1.png"); //���� �׸�
	auto room3 = Scene::create("�׸�2", "Images/pic_2.png"); //���� �׸� 
	auto room31 = Scene::create("�׸�2����", "Images/pic_2.png"); //���� �׸� 
	auto button1 = Object::create("Images/button1.png", room1, 500, 270); // �׸�1 ����
	auto button2 = Object::create("Images/button2.png", room1, 500, 120); //�׸�2 ����, �غ����Դϴ�.
	auto button_a1 = Object::create("Images/ans_1.png", room2, 0, 0); //���亸��
	auto button_a2 = Object::create("Images/ans_1.png", room3, 0, 0); //���亸��
	auto button_d1 = Object::create("Images/back1.png", room21, 0, 0); //�ڷΰ���
	auto button_d2 = Object::create("Images/back2.png", room31, 0, 0);

	// ����ȭ��
		//1�� �׸� ����
	auto closed1 = true;
	button1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room2->enter();

		return true;
		});

	//2�� �׸� ����
	auto closed2 = true;
	button2->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room3->enter();

		return true;
		});

	//�׸�1
	//����Դٰ���
	button_a1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room21->enter();

		return true;
		});
	button_d1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room2->enter();

		return true;
		});




	//�׸�2
	//����Դٰ���
	button_a2->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room31->enter();

		return true;
		});
	button_d2->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {

		room1->enter();

		return true;
		});


	//300 + (i % 4) * 150, 470 - (i / 4) * 150
	//�׸�1 ����
	ObjectPtr game[16];
	ObjectPtr init_board[16];
	int blank = 15;
	for (int i = 0; i < 16; i++) {
		string filename = "Images/" + to_string(i + 1) + ".png";
		cout << filename << endl;

		//�׸���ȯ
		game[i] = Object::create(filename, room2, 300 + (i % 4) * 150, 470 - (i / 4) * 150);

		//�׸� ����
		/*
		Ptr ran[16];
		int temp[16];
		for (int i = 0; i < 15; i++) {
			int random = rand() % (16 - i) + i; // i ���� 16-i ������
			temp[i] = game[i];
			game[i] = game[random];
			game[random] = temp[i]; //����

		}*/

		//���� ���߱�
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
				// ���� �̵�
				game[j] = game[blank];
				game[blank] = object;
				blank = j;

				int k = 0;
				for (k = 0; k < 16; k++) {
					if (game[k] != init_board[k])
						break;
				}
				if (k == 16) {
					showMessage("�ϼ�!");
				}
			}

			return true;
			});
	}

	game[blank]->hide();


	//�ð����� ī���� �����
	/* 
	auto count = 0;
	auto timer = Timer::create(0.1f);
	timer->setOnTimerCallback([&](TimerPtr t)-> bool {
		cout << "timeout" << count << endl;
		int n = rand() % 4;
		int j = 0;
		do {
			switch (rand() % 4) {
			case 0: //��
				j = blank - 1; break;
			case 1: // ��
				j = blank + 1; break;
			case 2: j = blank - 4; break; //�Ʒ�
			case 3: j = blank + 4; break; //��
			}
		} while ((j < 0 || j>15) || !(j % 4 > 0 && j - 1 == blank) || (j % 4 < 3 && j + 1 == blank) || (j > 3 && j - 4 == blank) || (j < 12 && j + 4 == blank));

		if ((j > 0 && j < 16) && (j % 4 > 0 && j - 1 == blank) || (j % 4 < 3 && j + 1 == blank) || (j > 3 && j - 4 == blank) || (j < 12 && j + 4 == blank))
		{
			cout << "move " << blank << "<->" << j << endl;

			game_board[j]->locate(room2, 300 + (blank % 4) * 150, 470 - (blank / 4) * 150);
			game_board[blank]->locate(room2, 300 + (j % 4) * 150, 470 - (j / 4) * 150);
			// �迭 �ٲٱ�
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
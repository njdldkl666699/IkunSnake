#include"UISystem.h"

int main()
{
	srand((unsigned int)time(nullptr));
	UISystem* ui = new UISystem();
	delete ui;
	return 0;
}
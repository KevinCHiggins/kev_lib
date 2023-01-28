#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"



#ifdef _WIN32

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR args_str, int n_cmd_show)
{

	
	init(instance);

	while (1)
	{
		poll_event();
		redraw();
	

	}
	//MessageBox(NULL, "Loop finished", "Error", MB_ICONEXCLAMATION | MB_OK);

}


#endif

#ifdef __linux__


int main()
{
	printf("Linux");
	init();
	while (1)
	{
		poll_event();
		redraw();
	}
	return 0;
}
#endif

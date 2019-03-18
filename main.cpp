//#include <iostream>
//#include <string>
//
//#include "SDL.h"
//#include "SDL_image.h"
//
//#include "Sprite.h"
//
//SDL_Event event;
//
//void Draw( SDL_Surface* surface, SDL_Surface* destination, int x, int y )
//{
//	SDL_Rect rect;
//	rect.x = x;
//	rect.y = y;
//
//	SDL_BlitSurface( surface, NULL, destination, &rect );
//}
//
//int main( int argc, char* argv[] )
//{
//	std::cout << "Hello." << std::endl;
//
//	SDL_Init( SDL_INIT_EVERYTHING );
//
//	SDL_Surface* screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
//	bool running = true;
//
//	SDL_Surface* slot = IMG_Load( "data/slot.png" );
//
//	SDL_Surface* item = IMG_Load( "data/item.png" );
//	int item_x = 150;
//	int item_y = 150;
//	bool itemSelected = false;
//
//	Sprite obrazek;
//	obrazek.LoadSprite( "data/item.png" );
//
//	while( running )
//	{
//		while( SDL_PollEvent( &event ) )
//		{
//			if( event.type == SDL_QUIT )
//				running = false;
//
//			if( event.type == SDL_MOUSEBUTTONDOWN )
//			{
//				if( event.button.button == SDL_BUTTON_LEFT )
//				{
//					int mouse_x = event.motion.x;
//					int mouse_y = event.motion.y;
//
//					if( mouse_x > 150 && mouse_x < 200 && mouse_y > 150 && mouse_y < 200 )
//					{
//						if( !itemSelected )
//							itemSelected = true;
//
//						else
//						{
//							itemSelected = false;
//							item_x = 150;
//							item_y = 150;
//						}
//					}
//				}
//			}
//
//			if( event.type == SDL_MOUSEMOTION )
//			{
//				int mouse_x = event.motion.x;
//				int mouse_y = event.motion.y;
//
//				if( itemSelected )
//				{
//					item_x = mouse_x;
//					item_y = mouse_y;
//				}
//			}
//
//		}
//
//		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
//
//		Draw( slot, screen, 150, 150 );
//		Draw( item, screen, item_x, item_y );
//
//		SDL_Flip( screen );
//	}
//
//	SDL_Quit();
//
//	return 0;
//}
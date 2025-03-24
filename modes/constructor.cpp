#pragma once
#include"../window.cpp"
#include"../grid.cpp"
#include"constructor.h"

namespace ABALONCO{
	namespace GUI{
		namespace Cores{
			void Constructor::update(){
				auto&keys=window->keyDown;
				FPoint32<8>speed=.8;
				if(keys[SDL_SCANCODE_LALT]||keys[SDL_SCANCODE_RALT])speed/=3;
				if(keys[SDL_SCANCODE_LSHIFT]||keys[SDL_SCANCODE_RSHIFT])speed*=8;
				if(keys[SDL_SCANCODE_LCTRL]||keys[SDL_SCANCODE_RCTRL])speed*=4;
				speed/=camera.zoom;if(!speed)speed._=1;
				if(keys[SDL_SCANCODE_W]){camera.pos[1]-=speed;window->redraw=camera.moved=1;}
				if(keys[SDL_SCANCODE_A]){camera.pos[0]-=speed;window->redraw=camera.moved=1;}
				if(keys[SDL_SCANCODE_S]){camera.pos[1]+=speed;window->redraw=camera.moved=1;}
				if(keys[SDL_SCANCODE_D]){camera.pos[0]+=speed;window->redraw=camera.moved=1;}
			}
			#define HexToRGB(r)r>>16,(r>>8)&0xFF,r&0xFF
			#define HexToRGBA(r)r>>24,(r>>16)&0xFF,(r>>8)&0xFF,r&0xFF
			#define HexToRGB_F(r)(r>>16)/255.f,((r>>8)&0xFF)/255.f,((r)&0xFF)/255.f
			#define HexToRGBA_F(r)(r>>24)/255.f,((r>>16)&0xFF)/255.f,((r>>8)&0xFF)/255.f,(r&0xFF)/255.f
			//#define GridColor 0x1256bf
			#define GridColor 0x124590
			void Constructor::draw(){
				window->redraw=0;
				if(camera.moved)CalcPos();
				if(camera.resized)resized();
				SDL_SetRenderDrawColor(window->_.r,HexToRGB(0x1668e6),255);
				SDL_RenderClear(window->_.r);

				auto grid=camera.gridO;
				if(camera.grid>GridThick){
					if(grid[0]>0)
						window->ScheduleRect({0,0},{grid[0],(float)window->size[1]},{HexToRGB_F(GridColor),1});
					if(grid[1]>0)
						window->ScheduleRect({0,0},{(float)window->size[0],grid[1]},{HexToRGB_F(GridColor),1});
					grid-=Vector2(GridThick);
					for(grid[0]+=camera.grid;(int)grid[0]<window->size[0];grid[0]+=camera.grid){
						assert(grid[0]>=0);
						window->ScheduleRect({grid[0],0},{grid[0]+GridThick,(float)window->size[1]},{HexToRGB_F(GridColor),1});
					}
					for(grid[1]+=camera.grid;(int)grid[1]<window->size[1];grid[1]+=camera.grid){
						assert(grid[1]>=0);
						window->ScheduleRect({0,grid[1]},{(float)window->size[0],grid[1]+GridThick},{HexToRGB_F(GridColor),1});
					}
				}
				window->ScheduleRectSized((Vector<FPoint32<8>>{5,5}*GridSize-camera.posO)*camera.zoom,{camera.grid,camera.grid},{1,0,0,1});
				window->ScheduleRect(window->size/2-Vector<int>{1,1},window->size/2+Vector<int>{1,1},{1,1,1,1});
				window->Present();
			}
			void Constructor::close(){ABALONCO::close=1;}
			void Constructor::cursor_move(SDL_MouseMotionEvent&e){
				if(camera.click){
					throw"";
				}else if(window->cursor.down[SDL_BUTTON_LEFT]){
					camera.pos-=Vector<FPoint32<8>>{e.xrel,e.yrel}/camera.zoom;
					camera.moved=1;
					window->redraw=1;
				}
			}
			void Constructor::mouse_wheel(SDL_MouseWheelEvent&e){
				constexpr auto s=1.4;
				auto _=e.y>0?camera.zoom*s:camera.zoom/s;
				if(_&&_*s!=_){
					if(e.y>0&&camera.zoom>_)
						_._=-1;
					else if(e.y<0&&(decltype(camera.zoom))1/GridSize>_)_=(decltype(camera.zoom))1/GridSize;
					if(_==camera.zoom)return;
					if(camera.moved)CalcPos();
					//auto c=Vector<FPoint32<8>>{e.wheel.mouse_x,e.wheel.mouse_y}-window.size/2;
					//window.pos+=(c/window.zoom)-(c/_);
					auto cursor=ScreenToGlobal({e.mouse_x-window->size[0]/2,e.mouse_y-window->size[1]/2});
					camera.zoom=_;CalcZoom();
					camera.pos+=cursor-ScreenToGlobal({e.mouse_x-window->size[0]/2,e.mouse_y-window->size[1]/2});CalcPos();
					camera.moved=1;
					window->redraw=1;
				}
			}
			void Constructor::resize(SDL_WindowEvent&e){window->redraw=camera.moved=camera.resized=1;Mode::resize(e);}
			void Constructor::resized(){camera.resized=0;window->_.s=SDL_GetWindowSurface(window->_.w);if(camera.moved)CalcPos();}
		}
	}
}
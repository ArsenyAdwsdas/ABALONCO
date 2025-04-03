#pragma once
#include"../../window.cpp"
#include"grid.cpp"
#include"main.h"

namespace ABALONCO{
	namespace GUI{
		namespace Cores{
			void Constructor::window_set(){}
			void Constructor::update(){
				auto&keys=window->keyDown;
				FPoint32<8>speed=.8;
				if(keys[SDL_SCANCODE_LALT]||keys[SDL_SCANCODE_RALT])speed/=3;
				if(keys[SDL_SCANCODE_LSHIFT]||keys[SDL_SCANCODE_RSHIFT])speed*=8;
				if(keys[SDL_SCANCODE_LCTRL]||keys[SDL_SCANCODE_RCTRL])speed*=4;
				speed/=camera.zoom;if(!speed)speed._=1;
				DrawPosTV o{0,0};
				if(keys[SDL_SCANCODE_W]){o[1]-=speed;}
				if(keys[SDL_SCANCODE_A]){o[0]-=speed;}
				if(keys[SDL_SCANCODE_S]){o[1]+=speed;}
				if(keys[SDL_SCANCODE_D]){o[0]+=speed;}
				if(o[0]||o[1])camera_move(o);
				if(keys[SDL_SCANCODE_C]&&!grid->GetAt(camera.cursor)){visible.push_back(grid->Place({0},{camera.cursor}));window->redraw=1;}
			}
			#define HexToRGB(r)r>>16,(r>>8)&0xFF,r&0xFF
			#define HexToRGBA(r)r>>24,(r>>16)&0xFF,(r>>8)&0xFF,r&0xFF
			#define HexToRGB_F(r)(r>>16)/255.f,((r>>8)&0xFF)/255.f,((r)&0xFF)/255.f
			#define HexToRGBA_F(r)(r>>24)/255.f,((r>>16)&0xFF)/255.f,((r>>8)&0xFF)/255.f,(r&0xFF)/255.f
			//#define GridColor 0x1256bf
			#define GridColor 0x124590
			void Constructor::draw(){
				window->redraw=0;
				if(camera.resized)resized();
				if(camera.query){
					// Rect<CoordT>r={camera.posO,camera.rbcor};
					// r.upper+=PosT{!camera.rbcor[0].whole(),!camera.rbcor[1].whole()};
					visible=grid->query(camera.onGrid2());
					//printf("visible %d\n",(int)visible.size());
					camera.query=0;
				}
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
				if(camera.onGrid2().intersects({{5,5},{6,6}}))window->ScheduleRectSized((Vector<FPoint32<8>>{5,5}*GridSize-camera.posO)*camera.zoom,{camera.grid,camera.grid},{1,0,0,1});
				window->ScheduleRect(window->size/2-Vector<int>{1,1},window->size/2+Vector<int>{1,1},{1,1,1,1});
				for(unsigned i=0;i<visible.size();i++)window->ScheduleRectSized(GlobalToScreen(ExtraOf(visible[i]).pos),(DrawPosTV)Grid::bindings[visible[i]->id]->size*camera.zoom,{0,0,0,1});
				//window->ScheduleRectSized(window->cursor.pos,{7,7},{.5,0,1,1});
				//window->ScheduleRectSized(GlobalToScreen(camera.cursor),{5,5},{1,.5,0,1});
				//window->ScheduleLine(GlobalToScreen(camera.cursor),window->size/2,{1,.5,0,1},1);
				constexpr float ExtremeGridLimit=DrawPosT::RAW(1)*decltype(camera.zoom)::RAW(-1);
				if constexpr(ExtremeGridLimit>1)if(auto E=DrawPosT::RAW(1)*camera.zoom;E>ExtremeGridThick){//extreme grid for when you zoom in to the point of precision being too small
					SDL_FColor color{HexToRGB_F(0x4c0099),(float)E/ExtremeGridLimit*.38f};
					constexpr auto O=float(-(int)ExtremeGridThick/2);
					float o=O;
					for(;o<window->size[0];o+=(float)E)window->ScheduleRectSized({o,0},{ExtremeGridThick,(float)window->size[1]},color);
					for(o=O;o<window->size[1];o+=(float)E)window->ScheduleRectSized({0,o},{(float)window->size[0],ExtremeGridThick},color);
				}
				window->Present();
			}
			void Constructor::close(){ABALONCO::close=1;}
			void Constructor::cursor_move(SDL_MouseMotionEvent&e){
				if(window->cursor.down[SDL_BUTTON_LEFT]){
					if(!camera.click){
						auto o=camera.cursor-ScreenToGlobal({e.x,e.y});//Vector<FPoint32<8>>{e.xrel,e.yrel}/camera.zoom;
						camera.pos+=o;
						camera.posO+=o;
						camera.gridO=Vector2<DrawPosT>(GridThick)-(((camera.posO%GridThick)*camera.zoom+Vector2(((DrawPosT)GridThick>>1)))%camera.grid);
						window->redraw=camera.query=1;
						return;
					}
					if(camera.drag){
						camera.drag->drag(e);
					}
				}
				camera.cursor=ScreenToGlobal({e.x,e.y});
			}
			void Constructor::mouse_wheel(SDL_MouseWheelEvent&e){
				constexpr auto s=1.4;
				auto _=e.y>0?camera.zoom*s:camera.zoom/s;
					if(e.y>0&&camera.zoom>_)_._=-1;//caps zoom-in at the highest possible
					else if(e.y<0&&(decltype(camera.zoom))1/GridSize>_)_=(decltype(camera.zoom))1/GridSize;//caps zoom-out at 1 grid square per pixel
					if(_==camera.zoom)return;
					//auto c=Vector<FPoint32<8>>{e.wheel.mouse_x,e.wheel.mouse_y}-window.size/2;
					//window.pos+=(c/window.zoom)-(c/_);
					auto o=camera.cursor;
					camera.zoom=_;CalcZoom();
					camera_move(o-camera.cursor);
			}
			void Constructor::resize(SDL_WindowEvent&e){window->redraw=camera.query=camera.resized=1;CalcZoom();Mode::resize(e);}
			void Constructor::resized(){camera.resized=0;window->_.s=SDL_GetWindowSurface(window->_.w);}
			void Constructor::mouse_click(SDL_MouseButtonEvent&e){camera.cursor=ScreenToGlobal({e.x,e.y});}
		}
	}
}
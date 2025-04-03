#pragma once
#include"../../window.h"
#include"grid.h"
namespace ABALONCO{
	namespace GUI{
		namespace Cores{
			struct Constructor:Mode{
				constexpr static uint8_t GridSize=1/*6*/,GridThick=1,ExtremeGridThick=1;
				using DrawPosT=FPoint32<8>;
				using DrawPosTV=Vector<DrawPosT>;
				Grid*grid=new Grid();
				struct{
					DrawPosTV pos={0,0},cursor;
					FPointU16<5/*8*/>zoom=16/*1*/;
					ActionSupport::MouseClick*click=0;
					ActionSupport::Drag*drag=0;
					bool resized=1,query=1;
					DrawPosTV posO,gridO,size;
					DrawPosT grid=(DrawPosT)GridSize*zoom;
					constexpr inline Rect<DrawPosT>onGrid(){return{posO,posO+size};}
					constexpr inline Rect<CoordT>onGrid2(){auto e=posO+size;return{posO,e+PosT{!e[0].whole(),!e[1].whole()}};}
				}camera;
				inline void CalcZoom(){
					camera.grid=(DrawPosT)GridSize*camera.zoom;
					assert(camera.grid>0);
					camera.size=(DrawPosTV)window->size/camera.zoom;
					camera.posO=camera.pos-(camera.size>>1);
					camera.gridO=Vector2<DrawPosT>(GridThick)-(((camera.posO%GridThick)*camera.zoom+Vector2(((DrawPosT)GridThick>>1)))%camera.grid);
					camera.cursor=ScreenToGlobal(window->cursor.pos);
				}
				inline void camera_move(decltype(camera.pos)o){
					camera.pos+=o;
					camera.cursor+=o;
					camera.posO+=o;
					camera.gridO=Vector2<DrawPosT>(GridThick)-(((camera.posO%GridThick)*camera.zoom+Vector2(((DrawPosT)GridThick>>1)))%camera.grid);
					window->redraw=camera.query=1;
				}
				inline DrawPosTV ScreenToGlobal(DrawPosTV p){return camera.posO+p/camera.zoom;}
				inline DrawPosTV GlobalToScreen(DrawPosTV p){return(p-camera.posO)*camera.zoom;}

				decltype(Grid().query({}))visible;
				void window_set()override;
				void draw()override;
				void update()override;
				void close()override;
				void resize(SDL_WindowEvent&)override;
				virtual void resized();//called once before drawing after a resize call
				void cursor_move(SDL_MouseMotionEvent&)override;
				void mouse_wheel(SDL_MouseWheelEvent&)override;
				void mouse_click(SDL_MouseButtonEvent&)override;
			};
		}
	}
}
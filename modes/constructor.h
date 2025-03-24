#pragma once
#include"../window.h"
#include"../grid.h"
namespace ABALONCO{
	namespace GUI{
		namespace Cores{
			struct Constructor:Mode{
				constexpr static uint8_t GridSize=1/*6*/,GridThick=1;
				using DrawPosT=FPoint32<8>;
				using DrawPosTV=Vector<DrawPosT>;
				Grid*grid;
				struct{
					Vector<FPoint32<8>>pos={0,0};
					FPointU16<6/*8*/>zoom=16/*1*/;
					ActionSupport::MouseClick*click=0;
					bool resized=1,moved=1;
					DrawPosTV posO,gridO;
					DrawPosT grid=(DrawPosT)GridSize*zoom;
				}camera;
				inline void CalcPos(){
					camera.moved=0;
					camera.posO=camera.pos-((DrawPosTV)window->size>>1)/camera.zoom;
					camera.gridO=Vector2<DrawPosT>(GridThick)-(((camera.posO%GridThick)*camera.zoom+Vector2(((DrawPosT)GridThick>>1)))%camera.grid);
				}
				inline void CalcZoom(){
					camera.grid=(DrawPosT)GridSize*camera.zoom;
					assert(camera.grid>0);
					CalcPos();
				}
				inline Vector<FPoint32<8>>ScreenToGlobal(Vector<FPoint32<8>>p){return camera.pos+p/camera.zoom;}
				inline Vector<FPoint32<8>>GlobalToScreen(Vector<FPoint32<8>>p){return(p-camera.pos)*camera.zoom;}
				inline DrawPosTV GlobalToCamera(Vector<FPoint32<8>>p){
					return p-camera.posO;
				}
				inline Vector<FPoint32<8>>CameraToGlobal(DrawPosTV p){
					return p+camera.posO;
				}
				void draw()override;
				void update()override;
				void close()override;
				void resize(SDL_WindowEvent&)override;
				virtual void resized();//called once before drawing after a resize call
				void cursor_move(SDL_MouseMotionEvent&)override;
				void mouse_wheel(SDL_MouseWheelEvent&)override;
			};
		}
	}
}
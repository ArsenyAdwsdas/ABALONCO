#pragma once
#ifdef VSC_AntiSchizo_L//It hallucinates it somewhere on my Arch, so took this nuclear route
	#ifdef _WIN32
		#undef _WIN32
	#endif
	#ifdef __WIN32__
		#undef __WIN32__
	#endif
	#ifdef __WXMSW__
		#undef __WXMSW__
	#endif
#endif
#include<SDL3/SDL.h>
#if defined(__APPLE__)||defined(WIN32)||defined(_WIN32)
	#define SDL_RESIZE_BLOCK 1
#else
	#define SDL_RESIZE_BLOCK 0
#endif
#include"hidden/meth.h"
#include"hidden/list.h"
constexpr inline SDL_Point SDL_point(Vector<int>f){return{f.x[0],f.x[1]};}
constexpr inline SDL_FPoint SDL_Fpoint(Vector<float>f){return{f.x[0],f.x[1]};}
namespace ABALONCO{
	using TimeT=std::chrono::microseconds;
	inline auto getTime(){return duration_cast<TimeT>(std::chrono::system_clock::now().time_since_epoch());}
	constexpr auto MaxFPS=std::min<int64_t>(duration_cast<TimeT>(std::chrono::seconds(1)).count(),duration_cast<std::chrono::system_clock::duration>(std::chrono::seconds(1)).count());
	namespace GUI{
		#if SDL_RESIZE_BLOCK
			extern bool eventsInMain;
		#else
			constexpr bool eventsInMain=1;
		#endif
		namespace ActionSupport{
			struct MouseClick{virtual void mouse_click(SDL_MouseButtonEvent&);virtual void mouse_unclick(SDL_MouseButtonEvent&);};
			struct Keyboard{virtual void key_down(SDL_KeyboardEvent&);virtual void key_up(SDL_KeyboardEvent&);};
			struct CursorMove{virtual void cursor_move(SDL_MouseMotionEvent&);};
			struct CursorIO{virtual void cursor_in();virtual void cursor_out();};
			struct MouseWheel{virtual void mouse_wheel(SDL_MouseWheelEvent&);};
			struct Drag:MouseClick{virtual void drag(SDL_MouseMotionEvent&);};
		}
		struct Window;
		struct Mode:ActionSupport::MouseClick,ActionSupport::CursorMove,ActionSupport::CursorIO,ActionSupport::MouseWheel,ActionSupport::Keyboard{
			Window*window;
			virtual void window_set();//called after assignment to a window
			virtual void draw()=0;
			virtual void update()=0;
			virtual void resize(SDL_WindowEvent&);
			virtual void key_focus();
			virtual void nowResizing();
			virtual void noLongerResizing();
			virtual void key_unfocus();
			virtual void close();
			virtual~Mode();
		};
		struct Window{
			uint32_t id;static Dicto<uint32_t,Window*>ids;
			static Window*keyFocus,*cursorFocus;
			Vector<int>size={800,600};
			bool redraw=1;
			bool keyDown[SDL_SCANCODE_COUNT]={0};
			Mode*core=0;
			struct{
				bool down[256]={0};
				bool inside=0;
				Vector<float>pos;
			}cursor;
			struct{
				SDL_Window*w=0;
				SDL_Renderer*r=0;
				SDL_Surface*s=0;
				List<SDL_Vertex,int>vIed;
				List<int,int>vI;
			}_;
			inline Window(){
				if(!SDL_CreateWindowAndRenderer("ABALONCO",size[0],size[1],SDL_WINDOW_RESIZABLE|SDL_WINDOW_HIGH_PIXEL_DENSITY,&_.w,&_.r)){_.w=0;return;}
				SDL_SetRenderDrawBlendMode(_.r, SDL_BLENDMODE_BLEND);
				ids.set(id=SDL_GetWindowID(_.w),this);
			}
			inline Window(Mode*c):Window(){(core=c)->window=this;}
			inline~Window(){if(core){delete core;core=0;}if(!_.w)return;SDL_DestroyRenderer(_.r);SDL_DestroyWindow(_.w);ids[id]=0;}
			void DrawLine(Vector<int>a,Vector<int>b,SDL_FColor);
			void DrawLine(Vector<float>a,Vector<float>b,SDL_FColor c,float w);
			void ScheduleLine(Vector<float>a,Vector<float>b,SDL_FColor c,float w);
			void ScheduleRect(Vector<float>a,Vector<float>b,SDL_FColor c);
			void ScheduleRectSized(Vector<float>a,Vector<float>s,SDL_FColor c);
			void Unschedule();
			void Present();
		};
	}
}
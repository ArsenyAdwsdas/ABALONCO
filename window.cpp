#define StImplementation 1
#include"window.h"
namespace ABALONCO{
	namespace GUI{
		#if SDL_RESIZE_BLOCK
			bool eventsInMain=1;
		#endif
		decltype(Window::ids)Window::ids;
		decltype(Window::keyFocus)Window::keyFocus=0;
		decltype(Window::cursorFocus)Window::cursorFocus=0;
		void ActionSupport::MouseClick::mouse_click(SDL_MouseButtonEvent&e){}
		void ActionSupport::MouseClick::mouse_unclick(SDL_MouseButtonEvent&e){}
		void ActionSupport::MouseWheel::mouse_wheel(SDL_MouseWheelEvent&){}
		void ActionSupport::CursorMove::cursor_move(SDL_MouseMotionEvent&e){}
		void ActionSupport::CursorIO::cursor_in(){}
		void ActionSupport::CursorIO::cursor_out(){}
		void ActionSupport::Keyboard::key_down(SDL_KeyboardEvent&e){}
		void ActionSupport::Keyboard::key_up(SDL_KeyboardEvent&e){}
		void Mode::resize(SDL_WindowEvent&e){}
		void Mode::resizeWatch(SDL_WindowEvent&){}
		void Mode::key_focus(){}
		void Mode::key_unfocus(){}
		void Mode::close(){delete window;delete this;}
		Mode::~Mode(){}
		void Window::DrawLine(Vector<int>a,Vector<int>b,SDL_FColor c){
			SDL_SetRenderDrawColorFloat(_.r,c.r,c.g,c.b,c.a);
			SDL_RenderLine(_.r,a.x[0],a.x[1],b.x[0],b.x[1]);
		}
		void Window::DrawLine(Vector<float>a,Vector<float>b,SDL_FColor c,float w){
			auto o=(a-b).rot90().normal()*w;
			SDL_Vertex v[]={{SDL_Fpoint(a+o),c},{SDL_Fpoint(b+o),c},{SDL_Fpoint(a-o),c},{SDL_Fpoint(b-o),c}};
			static int i[]={0,1,2,1,2,3};
			if(!SDL_RenderGeometry(_.r,0,v,4,i,6))printf("DrawLine: %s\n",SDL_GetError());
		}
		void Window::ScheduleLine(Vector<float>a,Vector<float>b,SDL_FColor c,float w){
			_.vI%={_.vIed.cou,_.vIed.cou+1,_.vIed.cou+2,_.vIed.cou+3,_.vIed.cou+2,_.vIed.cou+1};
			auto o=(a-b).rot90().normal()*w;
			_.vIed%={ {SDL_Fpoint(a+o),c}, {SDL_Fpoint(b+o),c}, {SDL_Fpoint(a-o),c}, {SDL_Fpoint(b-o),c} };
		}
		inline void Window::ScheduleRect(Vector<float>a,Vector<float>b,SDL_FColor c){
			_.vI%={_.vIed.cou,_.vIed.cou+1,_.vIed.cou+2,_.vIed.cou+3,_.vIed.cou+2,_.vIed.cou+1};
			_.vIed%={ {{a.x[0],a.x[1]},c}, {{a.x[0],b.x[1]},c}, {{b.x[0],a.x[1]},c}, {{b.x[0],b.x[1]},c} };
		}
		inline void Window::ScheduleRectSized(Vector<float>a,Vector<float>s,SDL_FColor c){ScheduleRect(a,a+s,c);}
		inline void Window::Present(){
			SDL_RenderGeometry(_.r,0,_.vIed.raw,_.vIed.cou,_.vI.raw,_.vI.cou);
			_.vIed.cou=_.vI.cou=0;
			SDL_RenderPresent(_.r);
		}
	}
}
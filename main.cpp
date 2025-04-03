#include"modes/constructor/main.cpp"
#include"modes/constructor/blocks.cpp"
#include<Standlos/Defines.h>

using namespace ABALONCO;
using GUI::Window;
bool ABALONCO::close=0;

#define w Window::ids[e.window.windowID]
#define cor w->core
#define cfw Window::cursorFocus
#define kfw Window::keyFocus
#define cf cfw->core
#define kf kfw->core
inline void eventHandle(SDL_Event&e){
	switch(e.type){
		case SDL_EVENT_QUIT:ABALONCO::close=1;break;
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:cor->close();break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:cfw->cursor.down[e.button.button]=1;cf->mouse_click(e.button);break;
		case SDL_EVENT_MOUSE_BUTTON_UP:cfw->cursor.down[e.button.button]=0;cf->mouse_unclick(e.button);break;
		case SDL_EVENT_MOUSE_WHEEL:cf->mouse_wheel(e.wheel);break;
		case SDL_EVENT_MOUSE_MOTION:cf->cursor_move(e.motion);cfw->cursor.pos={e.motion.x,e.motion.y};break;
		case SDL_EVENT_WINDOW_MOUSE_ENTER:(cfw=w)->cursor.inside=1;cfw->core->cursor_in();break;
		case SDL_EVENT_WINDOW_MOUSE_LEAVE:cfw->cursor.inside=0;cf->cursor_out();break;
		case SDL_EVENT_WINDOW_FOCUS_GAINED:(kfw=w)->core->key_focus();break;
		case SDL_EVENT_WINDOW_FOCUS_LOST:kf->key_unfocus();kfw=0;break;
		case SDL_EVENT_KEY_DOWN:kfw->keyDown[e.key.scancode]=1;kf->key_down(e.key);break;
		case SDL_EVENT_KEY_UP:kfw->keyDown[e.key.scancode]=0;kf->key_up(e.key);break;
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:{auto W=w;W->size={e.window.data1,e.window.data2};W->core->resize(e.window);}
	}
}
#undef kf
#undef cf
#undef cor
#undef w
#if SDL_RESIZE_BLOCK
	SDLCALL void eventWatch(void*,SDL_Event*e){
		bool _=ABALONCO::GUI::eventsInMain;
		if(e->type==SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)ABALONCO::GUI::eventsInMain=0;
		if(ABALONCO::GUI::eventsInMain)return;
		eventHandle(*e);
		if(_){auto w=Window::ids[e->window.windowID];if(w->redraw)w->core->draw();w->core->nowResizing();}
		else if(e->type==SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED){auto w=Window::ids[e->window.windowID];if(w->redraw)w->core->draw();}
	}
#endif

int main(int argc, char const *argv[]){
	SDL_Init(SDL_INIT_VIDEO);atexit(SDL_Quit);
	try{
		ABALONCO::GridBlocks();
		auto window=new Window(new GUI::Cores::Constructor());{
			auto b=SDL_GetGlobalMouseState(&window->cursor.pos[0],&window->cursor.pos[1]);
			int l,t,x,y;
			SDL_GetWindowBordersSize(window->_.w,&l,&t,0,0);
			SDL_GetWindowPosition(window->_.w,&x,&y);
			window->cursor.pos-={float(l+x),float(t+y)};
			for(int i=0;i<sizeof(b)*8;i++)window->cursor.down[i]=b&SDL_BUTTON_MASK(i);
			window->core->window_set();
		}
		if(!window->_.w){printf("CameraErr: %s\n",SDL_GetError());return 1;}
		printf("%s\n",SDL_GetCurrentVideoDriver());
		#if SDL_RESIZE_BLOCK
			SDL_AddEventWatch((SDLCALL bool(*)(void*,SDL_Event*))eventWatch,0);
		#endif
		while(1){
			SDL_Event e;while(SDL_PollEvent(&e)){
				#if SDL_RESIZE_BLOCK
					if(e.type==SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED&&!ABALONCO::GUI::eventsInMain){Window::ids[e.window.windowID]->core->noLongerResizing();ABALONCO::GUI::eventsInMain=1;}
				#endif
				eventHandle(e);
			}
			for(uint16_t i=0;i<Window::ids.count;i++)if(auto w=Window::ids.raw[i].second){
				w->core->update();
				if(w->redraw)w->core->draw();
			}
			if(ABALONCO::close)break;
			SDL_Delay(2);
		}
	}catch(const char*s){printf("threw %s\n",s);}
	for(uint16_t i=0;i<Window::ids.count;i++)if(Window::ids.raw[i].second){Window::ids.raw[i].second->core->~Mode();Window::ids.raw[i].second->_.w=0;}
	return 0;
}
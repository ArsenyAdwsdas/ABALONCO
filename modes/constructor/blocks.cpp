#include"main.h"
#include"../../blocks.cpp"
namespace ABALONCO{
	void GridBlocks(){
		BaseBlocks();
		static GridBlockDef<1>o1{{1,1},{{0,0}}};
		Grid::bindings+=(GridBlockDef<>*)&o1;
	}
}
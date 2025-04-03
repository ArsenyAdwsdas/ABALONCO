#include"state.h"
namespace ABALONCO{
	void BaseBlocks(){
		static BlockDef<1>o1{{{0,0}}};
		Block::types+=(BlockDef<>*)&o1;
	}
}
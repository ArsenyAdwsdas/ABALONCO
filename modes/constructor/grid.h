#pragma once
#include"../../state.h"
#include"../../hidden/rtree.h"
namespace ABALONCO{
	using CoordT=int32_t;
	using PosT=Vector<CoordT>;
	//"n" must match bindings[id].nodeCount
	template<uint16_t n=1>struct GridBlockDef{
		constexpr static uint16_t N=n;
		Vector<uint16_t>size;
		Vector<uint16_t>nodePos[n];
	};
	//what each Block needs for the Grid
	struct GridBlockExtra{
		PosT pos;
	};
	inline GridBlockExtra&ExtraOf(Block*b){return*(GridBlockExtra*)((uintptr_t)b-ceil(sizeof(GridBlockExtra),alignof(Block)));}
	struct Grid{
		static List<GridBlockDef<>*,uint16_t>bindings;
		State*state=new State();
		RTree<Block*,CoordT>_;
		List<Block*>query(Rect<CoordT>);
		Block*GetAt(PosT);
		Block*Place(Block,GridBlockExtra);
		void DestroyAt(PosT);
		void Move(Block*,PosT);
	};
}
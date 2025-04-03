#include"../../state.cpp"
#include"grid.h"
namespace ABALONCO{
	decltype(Grid::bindings)Grid::bindings;
	List<Block*>Grid::query(Rect<CoordT> r){return _.query(r);}
	Block*Grid::GetAt(PosT p){auto b=_.get_one({p,p+PosT{1,1}});return b?*b:0;}
	Block*Grid::Place(Block b,GridBlockExtra e){
		char*d=(char*)malloc(ceil(sizeof(GridBlockExtra),alignof(Block))+sizeof(Block));
		new(d)GridBlockExtra(e);
		auto B=new(d+ceil(sizeof(GridBlockExtra),alignof(Block)))Block(b);
		if(b.id>=Grid::bindings.cou)throw"Grid::Place bad id";
		_.insert(B,{e.pos,e.pos+Grid::bindings[b.id]->size});
		return B;
	}
	void Grid::DestroyAt(PosT p){
		Block*b;
		if(!_.extract_one({p,p+PosT{1,1}},&b))return;
		Block::types[b->id]->die(*b);
		auto&d=ExtraOf(b);
		d.~GridBlockExtra();
		free(&d);
	}
	void Grid::Move(Block*b,PosT p){
		auto&e=ExtraOf(b);
		_.remove_one({e.pos,e.pos+PosT{1,1}});
		_.insert(b,{e.pos=p,p+Grid::bindings[b->id]->size});
	}
}
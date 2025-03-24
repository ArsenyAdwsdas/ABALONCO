#pragma once
#include<Standlos/Collies.h>
#include<Standlos/Templatery.h>
namespace ABALONCO{
	extern bool close;
	struct Node{
		enum Flags{
			isInput=1,
			multiConnect=2,//allow multiple connections here
			copies=4//the input value will be copied
		};
		uint8_t flags;
		uint16_t dataType;//(uint16_t)-1 to allow any
		const char*desc=0;
	};
	template<uint16_t n=1>struct BlockDef{
		constexpr static uint16_t N=n;
		uint16_t id;
		const uint16_t nodeCount=n;
		Node nodes[n];
		constexpr inline BlockDef(uint16_t i,Node(&&N)[n]){id=i;constexpr_for<0,n>([&](auto i){nodes[i]=(Node&&)N[i];});}
	};
	struct Block{
		static List2<BlockDef<>*>types;
		const uint16_t id;
		uint16_t d1,d2,d3;
		union{
			void*ptr;
			uint64_t d4;
		};
	};
	struct State{
	};
}
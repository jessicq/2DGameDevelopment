#ifndef EXPLODINGTWOWAYMULTI_H
#define EXPLODINGTWOWAYMULTI_H

#include <vector>
#include <list>
#include "chunk.h"
#include "twoWayMultiSprite.h"

class ExplodingTwoWayMultiSprite : TwoWayMultiSprite {
public:
	ExplodingTwoWayMultiSprite(const TwoWayMultiSprite& s);
 	~ExplodingTwoWayMultiSprite();
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	void makeChunks(unsigned int);
	unsigned int chunkCount() const {return chunks.size();}
	unsigned int freeCount() const {return freeList.size();}
private:
	std::list<Chunk*> chunks;
	std::list<Chunk*> freeList;

	ExplodingTwoWayMultiSprite(const ExplodingTwoWayMultiSprite&);
	ExplodingTwoWayMultiSprite& operator=(const ExplodingTwoWayMultiSprite&);
};

#endif

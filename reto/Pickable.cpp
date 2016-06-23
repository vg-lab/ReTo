#include "Pickable.h"

Pickable::Pickable( void )
{}

Pickable::~Pickable( void )
{}

unsigned int Pickable::sendId ( unsigned int currentOffset ) 
{
	// sendUniform
	return currentOffset += _numIds;
}
void Pickable::setNumIDs ( int numIds )
{
	_numIds = numIds;
}
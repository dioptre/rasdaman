#include "storagemgr/stgmddconfig.hh"

using namespace std;

/****************************************************************************
 *
 *
 * INCLUDE: stgmddconfig.hh
 *
 * MODULE:  qlparser
 * CLASS:   StgMddConfig
 *
 * CHANGE HISTORY (append further entries):
 * when         who        what
 * -----------------------------------------------------------------------
 * 17-Aug-09	aaiordachi	create class (for adding rasql tiling options)
 * COMMENTS:
 *
 ****************************************************************************/

StgMddConfig::StgMddConfig()
{}
StgMddConfig::~StgMddConfig()
{}

// Setter functions 
void StgMddConfig::setBBoxes(const vector<r_Minterval>& input)
{
	bboxes = input;
}

void StgMddConfig::setSubTiling()
{
	subTiling = true;
}

void StgMddConfig::resetSubTiling()
{
	subTiling = false;
}

void StgMddConfig::setInterestThreshold(double i)
{
	interestThreshold = i;
}

void StgMddConfig::setBorderThreshold(unsigned int b)
{
	borderThreshold = b;
}

void StgMddConfig::setCellSize(int size)
{
	cellSize = size;
}

void StgMddConfig::setDirDecompose(const vector<r_Dir_Decompose>& input)
{
	dirDecomp = input;
}

// Getter functions 
vector<r_Minterval>& StgMddConfig::getBBoxes()
{
	return bboxes;
}

bool StgMddConfig::getSubTiling()
{
	return subTiling;
}

double StgMddConfig::getInterestThreshold()
{
	return interestThreshold;
}

unsigned int StgMddConfig::getBorderThreshold()
{
	return borderThreshold;
}

int StgMddConfig::getCellSize()
{
	return cellSize;
}

vector<r_Dir_Decompose>& StgMddConfig::getDirDecompose()
{
	return dirDecomp;
}
	

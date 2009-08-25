#ifndef _STORAGE_MDD_CONFIG_HH_
#define _STORAGE_MDD_CONFIG_HH_

#include <vector>
#include "rasodmg/dirdecompose.hh"
#include "raslib/minterval.hh"

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

class StgMddConfig
	{
	private:
		std::vector<r_Minterval> bboxes;
		double interestThreshold;
		unsigned int borderThreshold;
		bool subTiling;
		int cellSize;
		std::vector<r_Dir_Decompose> dirDecomp;

	public:

		StgMddConfig();
		~StgMddConfig();

		// Setter functions 
		void setBBoxes(const std::vector<r_Minterval>& input);
		void setSubTiling();
		void resetSubTiling();
		void setInterestThreshold(double i);
		void setBorderThreshold(unsigned int b);
		void setCellSize(int size);
		void setDirDecompose(const std::vector<r_Dir_Decompose>& input);

		// Getter functions 
		std::vector<r_Minterval>& getBBoxes();
		bool getSubTiling();
		double getInterestThreshold();
		unsigned int getBorderThreshold();
		int getCellSize();
		std::vector<r_Dir_Decompose>& getDirDecompose();
                
	};
#endif

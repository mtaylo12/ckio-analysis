#include <fstream>

#include "reader.decl.h"

/*readonly*/ int numChares;
/*readonly*/ size_t fileSize; // in bytes
/*readonly*/ std::string filename;
/*readonly*/ size_t allocFloor; // size of chare allocation (floor)
/*readonly*/ CProxy_Main mainProxy;
  
class Main : public CBase_Main
{
    Main_SDAG_CODE
        CProxy_Reader reader;

public:
    Main(CkArgMsg *msg)
    {
        if (msg->argc != 4)
        {
            CkPrintf("<Main> %s: expecting three arguments:\n\t<N> number of chares\n\t<K> input file size (GB)\t<F> filename\n",
                     msg->argv[0]);
            CkExit();
        }

        numChares = atoi(msg->argv[1]);
        fileSize = (size_t) atoi(msg->argv[2]) * 1024 * 1024 * 1024;
        std::string fn(msg->argv[3]);
	
        filename = fn;
        allocFloor = (size_t) (fileSize / (double)numChares);
        mainProxy = thisProxy;

        CkPrintf("<Main> Reading %s (%jd bytes)\n", filename.c_str());

        CProxy_Reader reader = CProxy_Reader::ckNew(numChares);
        reader.readFile();

        thisProxy.collectResults();
	
    }
};

class Reader : public CBase_Reader
{
private:
  size_t offset;
  size_t my_buffer_size;
  char *buffer;

public:
    Reader()
    {

      int numOverflow = fileSize % numChares;
      if (thisIndex < numOverflow) {
	my_buffer_size = allocFloor + 1;
	offset = (size_t) thisIndex * (allocFloor + 1);
      }
      else {
	my_buffer_size = allocFloor;
	offset = (size_t) numOverflow * (allocFloor + 1) + (thisIndex - numOverflow) * allocFloor;
      }

      CkPrintf("Chare %d with alloc size %zu and offset %zu\n", thisIndex, my_buffer_size, offset);
      
      // setup buffer to read to
      buffer = (char *)malloc(my_buffer_size);
      if (buffer == NULL)
        {
	  CkPrintf("<Reader> Error: cannot allocate buffer.\n");
	  CkExit();
        }
    }
    void readFile()
    {
        // open file for reading
        double total_time = CkWallTimer();
        FILE *fp = fopen(filename.c_str(), "r");
        if (fp == NULL)
        {
            CkPrintf("<Reader> Error: cannot open file \"%s\"\n", filename.c_str());
            CkExit();
        }

        // fseek to start of section
        int fseek_err = fseeko(fp, offset, SEEK_SET);
        if (fseek_err)
        {
            CkPrintf("<Reader> Error: fseeko failed.\n");
        }

        // read allocation
	
        double fread_time = CkWallTimer();
	size_t bytes_read;
	size_t total_bytes_read = 0;

	
	bytes_read = fread(buffer, 1, my_buffer_size, fp);
	if (bytes_read != my_buffer_size)
	  {
	    CkPrintf("<Reader> Error: read failed - bytes read = %zu and bytes expected = %zu.\n",bytes_read, my_buffer_size);
	  }
	
	total_time = CkWallTimer() - total_time;
	fread_time = CkWallTimer() - fread_time;
	
	
        fclose(fp);
        free(buffer);

        double sums[2] = {total_time, fread_time};

        CkCallback sumCb(CkReductionTarget(Main, sumReduction), mainProxy);
        contribute(2 * sizeof(double), sums, CkReduction::sum_double, sumCb);

        CkCallback maxCb(CkReductionTarget(Main, maxReduction), mainProxy);
        contribute(2 * sizeof(double), sums, CkReduction::max_double, maxCb);
    }
};

#include "reader.def.h"

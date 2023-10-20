#include <fstream>

#include "reader.decl.h"

/*readonly*/ int numChares;
/*readonly*/ size_t fileSize; // in bytes
/*readonly*/ std::string filename;
/*readonly*/ size_t allocSize; // size of chare allocation
/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ size_t BUFF_SIZE;
  
class Main : public CBase_Main
{
    Main_SDAG_CODE
        CProxy_Reader reader;

public:
    Main(CkArgMsg *msg)
    {
        if (msg->argc != 5)
        {
            CkPrintf("<Main> %s: expecting three arguments:\n\t<N> number of chares\n\t<K> input file size (GB)\t<F> filename\n\t<B> buffer size (GB) ",
                     msg->argv[0]);
            CkExit();
        }

        numChares = atoi(msg->argv[1]);
        fileSize = (size_t) atoi(msg->argv[2]) * 1024 * 1024 * 1024;
        std::string fn(msg->argv[3]);
		BUFF_SIZE = (size_t) 1024*1024*1024 * atoi(msg->argv[4]);
	
        filename = fn;
        allocSize = (size_t) (fileSize / (double)numChares);
        mainProxy = thisProxy;

        CkPrintf("<Main> Reading %s (%jd bytes)\n", filename.c_str());
	CkPrintf("<Main> Using with %d chares (each chare reads %d bytes)\n", numChares, allocSize);
	CkPrintf("<Main> Buffer size: %zu\n", BUFF_SIZE);

        CProxy_Reader reader = CProxy_Reader::ckNew(numChares, numChares);
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
    Reader(size_t num_chares)
    {

      offset = (size_t) thisIndex * allocSize;
     	buffer_size = allocSize;
		if(thisIndex == (num_chares - 1){
			my_buffer_size = fileSize - offset; // I'm the last chare, read everything
		}
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
	while ( && total_bytes_read < my_buffer_size) {
	  size_t bytes_to_read = std::min(BUFF_SIZE, (my_buffer_size - total_bytes_read));
	  bytes_read = fread(buffer, 1, bytes_to_read, fp));
	  if (bytes_read != bytes_to_read)
	    {
	      CkPrintf("<Reader> Error: read failed - bytes read = %zu and bytes expected = %zu.\n",bytes_read, bytes_to_read);
	    }
	  total_bytes_read += (size_t) bytes_read;
	}
	total_time = CkWallTimer() - total_time;
	fread_time = CkWallTimer() - fread_time;
	
	if (total_bytes_read != allocSize) {
	  CkPrintf("<Reader> Error: total bytes read (%jd) doesn't match allocation size (%jd).\n", total_bytes_read, allocSize);
	}
	
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

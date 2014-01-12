#include <stdexcept>
#include <sys/stat.h>
#include "filesize.h"

off_t filesize(const int file_descriptor)
{
  struct stat stat;

  if ( fstat(file_descriptor, &stat) < 0 )
    throw std::runtime_error("Could not stat file");

  return stat.st_size;
}

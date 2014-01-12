#include <unistd.h>
#include <string>
#include <fcntl.h>

class File {
  int fd;
public:
  File(const std::string& file, const int flags):
    fd(open(file.c_str(), flags))
  {
    if ( fd < 0 )
      std::runtime_error("Could not open file: " + file);
  }

  ~File() {
    close(fd);
  }

  inline operator int() const {
    return fd;
  }
};


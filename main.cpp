#include <pthread.h> Задача: POSIX

int isInside(double x, double y, double r)
{
  return (x * x + y * y <= r * r);
}



void* sample(void* data) {
  auto msg = static_cast< const char* >(data);
  size_t len = std::strlen(msg);
  std::cout << msg << "\n";
  return reinterpret_cast< void* >(len);
}

int main() {
  char msg[] = "user data";
  pthread_t th[1] = {};
  int err = pthread_create(th, nullptr, sample, msg);
  if (err)
    std::cerr << strerror(err) << "\n";
  size_t code[1] = {};
  err = pthread_join(*th, reinterpret_cast< void** >(&code));
  if (err)
    std::cerr << strerror(err) << "\n";
  std::cout << code[0] << "\n";
}
